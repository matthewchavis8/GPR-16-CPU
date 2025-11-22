#include "compileEngine.h"
#include "../Tokenizer/tokenizer.h"
#include "../VMWriter/vmWriter.h"
#include "../Utils/identifier.h"
#include "../Utils/keyword.h"
#include "../Utils/tokenType.h"
#include "../Utils/segment.h"
#include "../Utils/command.h"
#include "../Utils/log.h"
#include <stdexcept>
#include <string>

CompilationEngine::CompilationEngine(Tokenizer& tokenizer, VmWriter& vmWriter)
  : m_tokenizer(tokenizer)
  , m_VmWriter(vmWriter)
{}

void CompilationEngine::expectKeyword(Keyword kw) {
  if (m_tokenizer.tokenType() != Token::Keyword || m_tokenizer.keyWord() != kw)
    log<std::runtime_error>("Unexpected keyword in CompilationEngine");
  if (m_tokenizer.hasMoreTokens())
    m_tokenizer.advance();
}

void CompilationEngine::expectSymbol(char ch) {
  if (m_tokenizer.tokenType() != Token::Symbol || m_tokenizer.symbol() != ch)
    log<std::runtime_error>("Unexpected symbol in CompilationEngine");
  if (m_tokenizer.hasMoreTokens())
    m_tokenizer.advance();
}

std::string CompilationEngine::expectIdentifier() {
  if (m_tokenizer.tokenType() != Token::Identifier)
    log<std::runtime_error>("Expected identifier in CompilationEngine");
  std::string name = m_tokenizer.identifier();
  m_tokenizer.advance();
  return name;
}

bool CompilationEngine::isSymbol(char ch) const {
  return (m_tokenizer.tokenType() == Token::Symbol) && (m_tokenizer.symbol() == ch);
}

bool CompilationEngine::isKeyword(Keyword kw) const {
  return (m_tokenizer.tokenType() == Token::Keyword) && (m_tokenizer.keyWord() == kw);
}

Segment CompilationEngine::kindToSegment(IdentifierKind kind) const {
  switch (kind) {
    case IdentifierKind::Static: return Segment::Static;
    case IdentifierKind::Field:  return Segment::This;
    case IdentifierKind::Arg:    return Segment::Argument;
    case IdentifierKind::Var:    return Segment::Local;
    case IdentifierKind::None:   break;
  }
  log<std::runtime_error>("Invalid identifier kind for segment mapping");
  return Segment::Constant;
}

std::string CompilationEngine::makeLabel(const std::string& base, uint32_t index) const {
  return m_className + "." + m_subroutineName + "$" + base + std::to_string(index);
}

void CompilationEngine::compileClass() {
  expectKeyword(Keyword::Class);

  m_classTable.reset();
  m_subroutineTable.reset();
  m_className = expectIdentifier();

  expectSymbol('{');

  while (m_tokenizer.tokenType() == Token::Keyword &&
         (m_tokenizer.keyWord() == Keyword::Static ||
          m_tokenizer.keyWord() == Keyword::Field)) {
    compileClassVarDec();
  }

  while (m_tokenizer.tokenType() == Token::Keyword &&
         (m_tokenizer.keyWord() == Keyword::Constructor ||
          m_tokenizer.keyWord() == Keyword::Function ||
          m_tokenizer.keyWord() == Keyword::Method)) {
    compileSubroutine();
  }

  expectSymbol('}');
}

void CompilationEngine::compileClassVarDec() {
  bool isStatic { isKeyword(Keyword::Static) };
  if (!isStatic && !isKeyword(Keyword::Field))
    log<std::runtime_error>("Expected static/field in classVarDec");

  IdentifierKind kind = isStatic ? IdentifierKind::Static : IdentifierKind::Field;
  m_tokenizer.advance();

  std::string type;
  if (m_tokenizer.tokenType() == Token::Keyword &&
      (m_tokenizer.keyWord() == Keyword::Int ||
       m_tokenizer.keyWord() == Keyword::Char ||
       m_tokenizer.keyWord() == Keyword::Boolean)) {
    type = std::string(m_tokenizer.getCurrentToken());
    m_tokenizer.advance();
  } else {
    type = expectIdentifier();
  }

  std::string name = expectIdentifier();
  m_classTable.define(name, type, kind);

  while (isSymbol(',')) {
    m_tokenizer.advance();
    name = expectIdentifier();
    m_classTable.define(name, type, kind);
  }

  expectSymbol(';');
}

void CompilationEngine::compileSubroutine() {
  m_isMethod = false;
  m_isConstructor = false;

  if (isKeyword(Keyword::Constructor))
    m_isConstructor = true;
  else if (isKeyword(Keyword::Method))
    m_isMethod = true;
  else if (!isKeyword(Keyword::Function))
    log<std::runtime_error>("Expected subroutine keyword");

  m_tokenizer.advance();

  if (m_tokenizer.tokenType() == Token::Keyword &&
      (m_tokenizer.keyWord() == Keyword::Void ||
       m_tokenizer.keyWord() == Keyword::Int  ||
       m_tokenizer.keyWord() == Keyword::Char ||
       m_tokenizer.keyWord() == Keyword::Boolean)
      )
    m_tokenizer.advance();
  else
    expectIdentifier();

  m_subroutineName = expectIdentifier();

  m_subroutineTable.reset();
  if (m_isMethod)
    m_subroutineTable.define("this", m_className, IdentifierKind::Arg);

  expectSymbol('(');
  compileParameterList();
  expectSymbol(')');

  compileSubroutineBody();
}

void CompilationEngine::compileParameterList() {
  if (m_tokenizer.tokenType() != Token::Keyword &&
      m_tokenizer.tokenType() != Token::Identifier)
    return;

  if (!(m_tokenizer.tokenType() == Token::Keyword &&
        (m_tokenizer.keyWord() == Keyword::Int ||
         m_tokenizer.keyWord() == Keyword::Char ||
         m_tokenizer.keyWord() == Keyword::Boolean)) &&
      m_tokenizer.tokenType() != Token::Identifier)
    return;

  while (true) {
    std::string type;
    if (m_tokenizer.tokenType() == Token::Keyword &&
        (m_tokenizer.keyWord() == Keyword::Int ||
         m_tokenizer.keyWord() == Keyword::Char ||
         m_tokenizer.keyWord() == Keyword::Boolean)) {
      type = std::string(m_tokenizer.getCurrentToken());
      m_tokenizer.advance();
    } else {
      type = expectIdentifier();
    }

    std::string name = expectIdentifier();
    m_subroutineTable.define(name, type, IdentifierKind::Arg);

    if (!isSymbol(','))
      break;
    m_tokenizer.advance();
  }
}

void CompilationEngine::compileSubroutineBody() {
  expectSymbol('{');

  while (isKeyword(Keyword::Var)) {
    compileVarDec();
  }

  uint32_t nLocals = m_subroutineTable.varCount(IdentifierKind::Var);
  std::string fullName = m_className + "." + m_subroutineName;
  m_VmWriter.writeFunction(fullName, nLocals);

  if (m_isConstructor) {
    uint32_t nFields = m_classTable.varCount(IdentifierKind::Field);
    m_VmWriter.writePush(Segment::Constant, nFields);
    m_VmWriter.writeCall("Memory.alloc", 1);
    m_VmWriter.writePop(Segment::Pointer, 0);
  } else if (m_isMethod) {
    m_VmWriter.writePush(Segment::Argument, 0);
    m_VmWriter.writePop(Segment::Pointer, 0);
  }

  compileStatements();
  expectSymbol('}');
}

void CompilationEngine::compileVarDec() {
  expectKeyword(Keyword::Var);

  std::string type;
  if (m_tokenizer.tokenType() == Token::Keyword &&
      (m_tokenizer.keyWord() == Keyword::Int ||
       m_tokenizer.keyWord() == Keyword::Char ||
       m_tokenizer.keyWord() == Keyword::Boolean)) {
    type = std::string(m_tokenizer.getCurrentToken());
    m_tokenizer.advance();
  } else {
    type = expectIdentifier();
  }

  std::string name = expectIdentifier();
  m_subroutineTable.define(name, type, IdentifierKind::Var);

  while (isSymbol(',')) {
    m_tokenizer.advance();
    name = expectIdentifier();
    m_subroutineTable.define(name, type, IdentifierKind::Var);
  }

  expectSymbol(';');
}

void CompilationEngine::compileStatements() {
  while (m_tokenizer.tokenType() == Token::Keyword) {
    switch (m_tokenizer.keyWord()) {
      case Keyword::Let:    compileLet();    break;
      case Keyword::If:     compileIf();     break;
      case Keyword::While:  compileWhile();  break;
      case Keyword::Do:     compileDo();     break;
      case Keyword::Return: compileReturn(); break;
      default: return;
    }
  }
}

void CompilationEngine::compileLet() {
  expectKeyword(Keyword::Let);

  std::string varName = expectIdentifier();
  IdentifierKind kind = m_subroutineTable.kindOf(varName);
  uint32_t idx;
  if (kind != IdentifierKind::None) {
    idx = m_subroutineTable.indexOf(varName);
  } else {
    kind = m_classTable.kindOf(varName);
    idx = m_classTable.indexOf(varName);
  }
  Segment seg = kindToSegment(kind);

  bool isArray { false };
  if (isSymbol('[')) {
    isArray = true;
    m_tokenizer.advance();

    m_VmWriter.writePush(seg, idx);
    compileExpression();
    expectSymbol(']');
    m_VmWriter.writeArithmetic(Command::Add);
  }

  expectSymbol('=');
  compileExpression();
  expectSymbol(';');

  if (isArray) {
    m_VmWriter.writePop(Segment::Temp, 0);
    m_VmWriter.writePop(Segment::Pointer, 1);
    m_VmWriter.writePush(Segment::Temp, 0);
    m_VmWriter.writePop(Segment::That, 0);
  } else {
    m_VmWriter.writePop(seg, idx);
  }
}

void CompilationEngine::compileIf() {
  uint32_t idx { m_ifLabelIdx++ };
  std::string labelTrue  = makeLabel("IF_TRUE", idx);
  std::string labelFalse = makeLabel("IF_FALSE", idx);
  std::string labelEnd   = makeLabel("IF_END", idx);

  expectKeyword(Keyword::If);
  expectSymbol('(');
  compileExpression();
  expectSymbol(')');

  m_VmWriter.writeIf(labelTrue);
  m_VmWriter.writeGoto(labelFalse);
  m_VmWriter.writeLabel(labelTrue);

  expectSymbol('{');
  compileStatements();
  expectSymbol('}');

  if (isKeyword(Keyword::Else)) {
    m_VmWriter.writeGoto(labelEnd);
    m_VmWriter.writeLabel(labelFalse);

    m_tokenizer.advance();
    expectSymbol('{');
    compileStatements();
    expectSymbol('}');

    m_VmWriter.writeLabel(labelEnd);
  } else {
    m_VmWriter.writeLabel(labelFalse);
  }
}

void CompilationEngine::compileWhile() {
  uint32_t idx = m_whileLabelIdx++;
  std::string labelExp = makeLabel("WHILE_EXP", idx);
  std::string labelEnd = makeLabel("WHILE_END", idx);

  m_VmWriter.writeLabel(labelExp);

  expectKeyword(Keyword::While);
  expectSymbol('(');
  compileExpression();
  expectSymbol(')');

  m_VmWriter.writeArithmetic(Command::Not);
  m_VmWriter.writeIf(labelEnd);

  expectSymbol('{');
  compileStatements();
  expectSymbol('}');

  m_VmWriter.writeGoto(labelExp);
  m_VmWriter.writeLabel(labelEnd);
}

void CompilationEngine::compileDo() {
  expectKeyword(Keyword::Do);

  std::string name = expectIdentifier();

  uint32_t nArgs = 0;
  std::string callName;

  if (isSymbol('.')) {
    m_tokenizer.advance();
    std::string subName = expectIdentifier();

    IdentifierKind kind = m_subroutineTable.kindOf(name);
    if (kind == IdentifierKind::None)
      kind = m_classTable.kindOf(name);

    if (kind != IdentifierKind::None) {
      Segment seg = kindToSegment(kind);
      uint32_t idx = (m_subroutineTable.kindOf(name) != IdentifierKind::None)
                       ? m_subroutineTable.indexOf(name)
                       : m_classTable.indexOf(name);
      m_VmWriter.writePush(seg, idx);
      std::string type = (m_subroutineTable.kindOf(name) != IdentifierKind::None)
                           ? m_subroutineTable.typeOf(name)
                           : m_classTable.typeOf(name);
      callName = type + "." + subName;
      nArgs = 1;
    } else {
      callName = name + "." + subName;
    }
  } else {
    callName = m_className + "." + name;
    m_VmWriter.writePush(Segment::Pointer, 0);
    nArgs = 1;
  }

  expectSymbol('(');
  nArgs += compileExpressionList();
  expectSymbol(')');

  m_VmWriter.writeCall(callName, nArgs);
  expectSymbol(';');

  m_VmWriter.writePop(Segment::Temp, 0);
}

void CompilationEngine::compileReturn() {
  expectKeyword(Keyword::Return);

  if (!isSymbol(';')) {
    compileExpression();
  } else {
    m_VmWriter.writePush(Segment::Constant, 0);
  }

  expectSymbol(';');
  m_VmWriter.writeReturn();
}

void CompilationEngine::compileExpression() {
  compileTerm();

  while (m_tokenizer.tokenType() == Token::Symbol) {
    char op = m_tokenizer.symbol();
    if (op != '+' && op != '-' && op != '*' && op != '/' &&
        op != '&' && op != '|' && op != '<' && op != '>' && op != '=') {
      break;
    }
    m_tokenizer.advance();

    compileTerm();

    switch (op) {
      case '+': m_VmWriter.writeArithmetic(Command::Add); break;
      case '-': m_VmWriter.writeArithmetic(Command::Sub); break;
      case '&': m_VmWriter.writeArithmetic(Command::And); break;
      case '|': m_VmWriter.writeArithmetic(Command::Or);  break;
      case '<': m_VmWriter.writeArithmetic(Command::Lt);  break;
      case '>': m_VmWriter.writeArithmetic(Command::Gt);  break;
      case '=': m_VmWriter.writeArithmetic(Command::Eq);  break;
      case '*':
        m_VmWriter.writeCall("Math.multiply", 2);
        break;
      case '/':
        m_VmWriter.writeCall("Math.divide", 2);
        break;
    }
  }
}

void CompilationEngine::compileTerm() {
  Token tt = m_tokenizer.tokenType();

  if (tt == Token::IntConst) {
    uint32_t val = m_tokenizer.intVal();
    m_VmWriter.writePush(Segment::Constant, val);
    m_tokenizer.advance();
  } else if (tt == Token::StringConst) {
    std::string s = m_tokenizer.stringVal();
    m_VmWriter.writePush(Segment::Constant, static_cast<uint32_t>(s.size()));
    m_VmWriter.writeCall("String.new", 1);
    for (char c : s) {
      m_VmWriter.writePush(Segment::Constant, static_cast<uint32_t>(static_cast<unsigned char>(c)));
      m_VmWriter.writeCall("String.appendChar", 2);
    }
    m_tokenizer.advance();
  } else if (tt == Token::Keyword) {
    Keyword kw = m_tokenizer.keyWord();
    switch (kw) {
      case Keyword::True:
        m_VmWriter.writePush(Segment::Constant, 0);
        m_VmWriter.writeArithmetic(Command::Not);
        break;
      case Keyword::False:
      case Keyword::Null:
        m_VmWriter.writePush(Segment::Constant, 0);
        break;
      case Keyword::This:
        m_VmWriter.writePush(Segment::Pointer, 0);
        break;
      default:
        log<std::runtime_error>("Invalid keyword term");
    }
    m_tokenizer.advance();
  } else if (tt == Token::Symbol && isSymbol('(')) {
    m_tokenizer.advance();
    compileExpression();
    expectSymbol(')');
  } else if (tt == Token::Symbol &&
             (m_tokenizer.symbol() == '-' || m_tokenizer.symbol() == '~')) {
    char op = m_tokenizer.symbol();
    m_tokenizer.advance();
    compileTerm();
    if (op == '-')      m_VmWriter.writeArithmetic(Command::Neg);
    else                m_VmWriter.writeArithmetic(Command::Not);
  } else if (tt == Token::Identifier) {
    std::string name = m_tokenizer.identifier();
    m_tokenizer.advance();

    if (isSymbol('[')) {
      m_tokenizer.advance();

      IdentifierKind kind = m_subroutineTable.kindOf(name);
      uint32_t idx;
      if (kind != IdentifierKind::None) {
        idx = m_subroutineTable.indexOf(name);
      } else {
        kind = m_classTable.kindOf(name);
        idx = m_classTable.indexOf(name);
      }
      Segment seg = kindToSegment(kind);

      m_VmWriter.writePush(seg, idx);
      compileExpression();
      expectSymbol(']');
      m_VmWriter.writeArithmetic(Command::Add);
      m_VmWriter.writePop(Segment::Pointer, 1);
      m_VmWriter.writePush(Segment::That, 0);
    } else if (isSymbol('(') || isSymbol('.')) {
      uint32_t nArgs = 0;
      std::string callName;

      if (isSymbol('.')) {
        m_tokenizer.advance();
        std::string subName = expectIdentifier();

        IdentifierKind kind = m_subroutineTable.kindOf(name);
        if (kind == IdentifierKind::None)
          kind = m_classTable.kindOf(name);

        if (kind != IdentifierKind::None) {
          Segment seg = kindToSegment(kind);
          uint32_t idx = (m_subroutineTable.kindOf(name) != IdentifierKind::None)
                           ? m_subroutineTable.indexOf(name)
                           : m_classTable.indexOf(name);
          m_VmWriter.writePush(seg, idx);
          std::string type = (m_subroutineTable.kindOf(name) != IdentifierKind::None)
                               ? m_subroutineTable.typeOf(name)
                               : m_classTable.typeOf(name);
          callName = type + "." + subName;
          nArgs = 1;
        } else {
          callName = name + "." + subName;
        }
      } else {
        callName = m_className + "." + name;
        m_VmWriter.writePush(Segment::Pointer, 0);
        nArgs = 1;
      }

      expectSymbol('(');
      nArgs += compileExpressionList();
      expectSymbol(')');
      m_VmWriter.writeCall(callName, nArgs);
    } else {
      IdentifierKind kind = m_subroutineTable.kindOf(name);
      uint32_t idx;
      if (kind != IdentifierKind::None) {
        idx = m_subroutineTable.indexOf(name);
      } else {
        kind = m_classTable.kindOf(name);
        idx = m_classTable.indexOf(name);
      }
      Segment seg = kindToSegment(kind);
      m_VmWriter.writePush(seg, idx);
    }
  } else {
    log<std::runtime_error>("Invalid term");
  }
}

uint32_t CompilationEngine::compileExpressionList() {
  uint32_t count{};

  if (m_tokenizer.tokenType() == Token::Symbol && m_tokenizer.symbol() == ')')
    return 0;

  compileExpression();
  ++count;

  while (isSymbol(',')) {
    m_tokenizer.advance();
    compileExpression();
    ++count;
  }

  return count;
}

