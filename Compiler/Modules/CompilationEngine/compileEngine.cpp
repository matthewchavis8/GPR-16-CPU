#include "compileEngine.h"
#include "../Tokenizer/tokenizer.h"
#include <fstream>
#include <initializer_list>
#include <stdexcept>
#include <string_view>

CompilationEngine::CompilationEngine(Tokenizer& tokenizer, std::ofstream& output_file)
  : m_tokenizer(tokenizer)
  , m_output_file(output_file)
{}

std::string xmlEscape(std::string_view str) {
  std::string res;
  res.reserve(str.size() + 16);

  for (char c : str) {
    switch (c) {
      case '<':
        res += "&lt;";
        break;
      case '>':
        res += "&gt;";
        break;
      case '&':
        res += "&amp;";
        break;
      default:
        res += c;
        break;
    }
  }
  return res;
}

std::string_view kwToStr(Keyword k) {
  switch (k) {
    case Keyword::Class:
      return "class";
    case Keyword::Method:
      return "method";
    case Keyword::Function:
      return "function";
    case Keyword::Constructor:
      return "constructor";
    case Keyword::Int:
      return "int";
    case Keyword::Boolean:
      return "boolean";
    case Keyword::Char:
      return "char";
    case Keyword::Var:
      return "var";
    case Keyword::Static:
      return "static";
    case Keyword::Field:
      return "field";
    case Keyword::Let:
      return "let";
    case Keyword::Do:
      return "do";
    case Keyword::If:
      return "if";
    case Keyword::Else:
      return "else";
    case Keyword::While:
      return "while";
    case Keyword::Return:
      return "return";
    case Keyword::True:
      return "true";
    case Keyword::False:
      return "false";
    case Keyword::Null:
      return "null";
    case Keyword::This:
      return "this";
    case Keyword::Void:
      return "void";
  }

  return "?";
}

void CompilationEngine::openTag(std::string_view name) {
  m_output_file << "<" << name << ">" << '\n';
}

void CompilationEngine::closeTag(std::string_view name) {
  m_output_file << "</" << name << ">" << '\n';
}

bool CompilationEngine::isTypeToken() const {
  if (m_tokenizer.tokenType() != Token::Keyword)
    return false;

  switch (m_tokenizer.keyWord()) {
    case Keyword::Int:
      return true;
    case Keyword::Char:
      return true;
    case Keyword::Boolean:
      return true;
    default:
      return false;
  }
  return m_tokenizer.tokenType() == Token::Identifier;
}

bool CompilationEngine::isKeywordConstant() const {
  if (m_tokenizer.tokenType() != Token::Keyword)
    return false;

  switch (m_tokenizer.keyWord()) {
    case Keyword::True:
      return true;
    case Keyword::False:
      return true;
    case Keyword::Null:
      return true;
    case Keyword::This:
      return true;
    default:
      return false;
  }
  return m_tokenizer.tokenType() == Token::Identifier;
}

bool CompilationEngine::isUnaryOp() const {
  if (m_tokenizer.tokenType() != Token::Symbol)
    return false;

  return (m_tokenizer.symbol() == '-' || m_tokenizer.symbol() == '~');
}

bool CompilationEngine::isOp(char c) {
  switch (c) {
    case '+': case '-': case '*':
    case '/': case '&': case '|':
    case '<': case '>': case '=':
      return true;

    default:
      return false;
  }
}

void CompilationEngine::emitKeyWordAndAdvance() {
  m_output_file << "<keyword> " << kwToStr(m_tokenizer.keyWord()) << " </keyword>" << '\n';
  if (m_tokenizer.hasMoreTokens())
    m_tokenizer.advance();
}

void CompilationEngine::emitSymbolAndAdvance() {
  std::string str(1, m_tokenizer.symbol());
  m_output_file << "<symbol> " << xmlEscape(str) << " </symbol>" << '\n';
  if (m_tokenizer.hasMoreTokens())
    m_tokenizer.advance();
}

void CompilationEngine::emitIdentifierAndAdvance() {
  m_output_file << "<identifier> " << m_tokenizer.identifier() << " </identifier>" << '\n';
  if (m_tokenizer.hasMoreTokens())
    m_tokenizer.advance();
}

void CompilationEngine::emitIntAndAdvance() {
  m_output_file << "<intConstant> " << m_tokenizer.intVal() << " </intConstant>" << '\n';
  if (m_tokenizer.hasMoreTokens())
    m_tokenizer.advance();
}

void CompilationEngine::emitStringAndAdvance() {
  m_output_file << "<stringConstant> " << m_tokenizer.intVal() << " </stringConstant>" << '\n';
  if (m_tokenizer.hasMoreTokens())
    m_tokenizer.advance();
}

void CompilationEngine::compileClass() {
  openTag("class");

  if (m_tokenizer.tokenType() != Token::Keyword)
    throw std::runtime_error("[ERROR] Expected Class");
  emitKeyWordAndAdvance();
  
  if (m_tokenizer.tokenType() != Token::Identifier)
    throw std::runtime_error("[ERROR] Expected identifier");
  emitIdentifierAndAdvance();
  
  if (m_tokenizer.tokenType() != Token::Symbol)
    throw std::runtime_error("[ERROR] Expected Symbol");
  emitSymbolAndAdvance();

  while (m_tokenizer.tokenType() == Token::Keyword) {
    Keyword k { m_tokenizer.keyWord() };
    if (k == Keyword::Static || k == Keyword::Field)
      compileClassVarDec();
    else
      break;
  }
  
  while (m_tokenizer.tokenType() == Token::Keyword) {
    Keyword k { m_tokenizer.keyWord() };
    if (k == Keyword::Constructor || k == Keyword::Function)
      compileSubroutine();
    else
      break;
  }
  
  if (m_tokenizer.tokenType() != Token::Symbol)
    throw std::runtime_error("[ERROR] Expected Symbol");
  emitSymbolAndAdvance();

  
  closeTag("class");
}

void CompilationEngine::compileClassVarDec() {
  openTag("classVarDec");

  if (m_tokenizer.tokenType() != Token::Keyword)
    throw std::runtime_error("[ERROR] expected 'static' or 'field'");
  if (m_tokenizer.keyWord() != Keyword::Static && m_tokenizer.keyWord() != Keyword::Field)
    throw std::runtime_error("[ERROR] expected 'static' or 'field'");
  emitKeyWordAndAdvance();

  if (!isTypeToken()) 
    throw std::runtime_error("[ERROR] expected type");
  if (m_tokenizer.tokenType() == Token::Keyword)
    emitKeyWordAndAdvance();
  else 
    emitIdentifierAndAdvance();

  if (m_tokenizer.tokenType() != Token::Identifier)
    throw std::runtime_error("[ERROR] expected varName");
  emitIdentifierAndAdvance();

  while (m_tokenizer.tokenType() == Token::Symbol && m_tokenizer.symbol() == ',') {
    emitSymbolAndAdvance();
    if (m_tokenizer.tokenType() != Token::Identifier)
      throw std::runtime_error("[ERROR] expected varName after ','");
    emitIdentifierAndAdvance();
  }

  if (m_tokenizer.tokenType() != Token::Symbol || m_tokenizer.symbol() != ';')
    throw std::runtime_error("[ERROR] expected ';' after classVarDec");
  emitSymbolAndAdvance();

  closeTag("classVarDec");
}

void CompilationEngine::compileSubroutine() {
  openTag("subroutineDec");

  if (m_tokenizer.tokenType() != Token::Keyword) 
    throw std::runtime_error("[ERROR] expected subroutine keyword");
  {
    Keyword k = m_tokenizer.keyWord();
    if (k != Keyword::Constructor && k != Keyword::Function && k != Keyword::Method)
      throw std::runtime_error("[ERROR] expected constructor|function|method");
  }
  emitKeyWordAndAdvance();

  if (m_tokenizer.tokenType() == Token::Keyword && m_tokenizer.keyWord() == Keyword::Void)
    emitKeyWordAndAdvance();
  else if (isTypeToken()) {
    if (m_tokenizer.tokenType() == Token::Keyword) 
      emitKeyWordAndAdvance();
    else
      emitIdentifierAndAdvance();
  } 
  else
    throw std::runtime_error("[ERROR] expected return type");

  if (m_tokenizer.tokenType() != Token::Identifier)
    throw std::runtime_error("[ERROR] expected subroutineName");
  emitIdentifierAndAdvance();

  if (m_tokenizer.tokenType() != Token::Symbol || m_tokenizer.symbol() != '(')
    throw std::runtime_error("[ERROR] expected '(' after subroutineName");
  emitSymbolAndAdvance();

  compileParameterList();

  if (m_tokenizer.tokenType() != Token::Symbol || m_tokenizer.symbol() != ')')
    throw std::runtime_error("[ERROR] expected ')' after parameterList");
  emitSymbolAndAdvance();

  compileSubroutineBody();

  closeTag("subroutineDec");
}

void CompilationEngine::compileParameterList() {
  openTag("parameterList");

  if (m_tokenizer.tokenType() == Token::Symbol && m_tokenizer.symbol() == ')') {
    closeTag("parameterList");
    return;
  }

  if (!isTypeToken()) 
    throw std::runtime_error("[ERROR] expected parameter type");
  if (m_tokenizer.tokenType() == Token::Keyword) 
    emitKeyWordAndAdvance();
  else 
    emitIdentifierAndAdvance();

  if (m_tokenizer.tokenType() != Token::Identifier)
    throw std::runtime_error("[ERROR] expected parameter name");
  emitIdentifierAndAdvance();

  while (m_tokenizer.tokenType() == Token::Symbol && m_tokenizer.symbol() == ',') {
    emitSymbolAndAdvance();

    if (!isTypeToken()) 
      throw std::runtime_error("[ERROR] expected parameter type");
    if (m_tokenizer.tokenType() == Token::Keyword) 
      emitKeyWordAndAdvance();
    else 
      emitIdentifierAndAdvance();

    if (m_tokenizer.tokenType() != Token::Identifier)
      throw std::runtime_error("[ERROR] expected parameter name");
    emitIdentifierAndAdvance();
  }

  closeTag("parameterList");
}

void CompilationEngine::compileSubroutineBody() {
  openTag("subroutineBody");

  if (m_tokenizer.tokenType() != Token::Symbol || m_tokenizer.symbol() != '{')
    throw std::runtime_error("[ERROR] expected '{' to start subroutine body");
  emitSymbolAndAdvance();

  while (m_tokenizer.tokenType() == Token::Keyword && m_tokenizer.keyWord() == Keyword::Var)
    compileVarDec();

  compileStatements();

  if (m_tokenizer.tokenType() != Token::Symbol || m_tokenizer.symbol() != '}')
    throw std::runtime_error("[ERROR] expected '}' to close subroutine body");
  emitSymbolAndAdvance();

  closeTag("subroutineBody");
}

void CompilationEngine::compileVarDec() {
  openTag("varDec");

  if (m_tokenizer.tokenType() != Token::Keyword || m_tokenizer.keyWord() != Keyword::Var)
    throw std::runtime_error("[ERROR] expected 'var'");
  emitKeyWordAndAdvance();

  if (!isTypeToken()) 
    throw std::runtime_error("[ERROR] expected var type");
  if (m_tokenizer.tokenType() == Token::Keyword) 
    emitKeyWordAndAdvance();
  else 
    emitIdentifierAndAdvance();

  if (m_tokenizer.tokenType() != Token::Identifier)
    throw std::runtime_error("[ERROR] expected varName");
  emitIdentifierAndAdvance();

  while (m_tokenizer.tokenType() == Token::Symbol && m_tokenizer.symbol() == ',') {
    emitSymbolAndAdvance();
    if (m_tokenizer.tokenType() != Token::Identifier)
      throw std::runtime_error("[ERROR] expected varName after ','");
    emitIdentifierAndAdvance();
  }

  if (m_tokenizer.tokenType() != Token::Symbol || m_tokenizer.symbol() != ';')
    throw std::runtime_error("[ERROR] expected ';' after varDec");
  emitSymbolAndAdvance();

  closeTag("varDec");
}

void CompilationEngine::compileStatements() {
  openTag("statements");

  for (;;) {
    if (m_tokenizer.tokenType() != Token::Keyword)
      break;
    switch (m_tokenizer.keyWord()) {
      case Keyword::Let:
        compileLet();
        continue;
      case Keyword::If:
        compileIf();
        continue;
      case Keyword::While:
        compileWhile();
        continue;
      case Keyword::Do:
        compileDo();
        continue;
      case Keyword::Return:
        compileReturn();
        continue;
      default:
        break;
    }
    break;
  }
  closeTag("statements");
}

void CompilationEngine::compileLet() {
  openTag("letStatement");

  if (m_tokenizer.tokenType() != Token::Keyword || m_tokenizer.keyWord() != Keyword::Let)
    throw std::runtime_error("[ERROR] expected 'let'");
  emitKeyWordAndAdvance();

  if (m_tokenizer.tokenType() != Token::Identifier)
    throw std::runtime_error("[ERROR] expected varName");
  emitIdentifierAndAdvance();

  if (m_tokenizer.tokenType() == Token::Symbol && m_tokenizer.symbol() == '[') {
    emitSymbolAndAdvance();
    compileExpression();
    if (m_tokenizer.tokenType() != Token::Symbol || m_tokenizer.symbol() != ']')
      throw std::runtime_error("[ERROR] expected ']' in array assignment");
    emitSymbolAndAdvance();
  }

  if (m_tokenizer.tokenType() != Token::Symbol || m_tokenizer.symbol() != '=')
    throw std::runtime_error("[ERROR] expected '=' in let");
  emitSymbolAndAdvance();

  compileExpression();

  if (m_tokenizer.tokenType() != Token::Symbol || m_tokenizer.symbol() != ';')
    throw std::runtime_error("[ERROR] expected ';' after let");
  emitSymbolAndAdvance();

  closeTag("letStatement");
}

void CompilationEngine::compileIf() {
  openTag("ifStatement");

  if (m_tokenizer.tokenType() != Token::Keyword || m_tokenizer.keyWord() != Keyword::If)
    throw std::runtime_error("[ERROR] expected 'if'");
  emitKeyWordAndAdvance();

  if (m_tokenizer.tokenType() != Token::Symbol || m_tokenizer.symbol() != '(')
    throw std::runtime_error("[ERROR] expected '(' after if");
  emitSymbolAndAdvance();

  compileExpression();

  if (m_tokenizer.tokenType() != Token::Symbol || m_tokenizer.symbol() != ')')
    throw std::runtime_error("[ERROR] expected ')' after condition");
  emitSymbolAndAdvance();

  if (m_tokenizer.tokenType() != Token::Symbol || m_tokenizer.symbol() != '{')
    throw std::runtime_error("[ERROR] expected '{' after if()");
  emitSymbolAndAdvance();

  compileStatements();

  if (m_tokenizer.tokenType() != Token::Symbol || m_tokenizer.symbol() != '}')
    throw std::runtime_error("[ERROR] expected '}' to close if-block");
  emitSymbolAndAdvance();

  // optional else
  if (m_tokenizer.tokenType() == Token::Keyword && m_tokenizer.keyWord() == Keyword::Else) {
    emitKeyWordAndAdvance();
    if (m_tokenizer.tokenType() != Token::Symbol || m_tokenizer.symbol() != '{')
      throw std::runtime_error("[ERROR] expected '{' after else");
    emitSymbolAndAdvance();
    compileStatements();
    if (m_tokenizer.tokenType() != Token::Symbol || m_tokenizer.symbol() != '}')
      throw std::runtime_error("[ERROR] expected '}' to close else-block");
    emitSymbolAndAdvance();
  }

  closeTag("ifStatement");
}

void CompilationEngine::compileWhile() {
  openTag("whileStatement");

  if (m_tokenizer.tokenType() != Token::Keyword || m_tokenizer.keyWord() != Keyword::While)
    throw std::runtime_error("[ERROR] expected 'while'");
  emitKeyWordAndAdvance();

  if (m_tokenizer.tokenType() != Token::Symbol || m_tokenizer.symbol() != '(')
    throw std::runtime_error("[ERROR] expected '(' after while");
  emitSymbolAndAdvance();

  compileExpression();

  if (m_tokenizer.tokenType() != Token::Symbol || m_tokenizer.symbol() != ')')
    throw std::runtime_error("[ERROR] expected ')' after condition");
  emitSymbolAndAdvance();

  if (m_tokenizer.tokenType() != Token::Symbol || m_tokenizer.symbol() != '{')
    throw std::runtime_error("[ERROR] expected '{' to start while-body");
  emitSymbolAndAdvance();

  compileStatements();

  if (m_tokenizer.tokenType() != Token::Symbol || m_tokenizer.symbol() != '}')
    throw std::runtime_error("[ERROR] expected '}' to close while-body");
  emitSymbolAndAdvance();

  closeTag("whileStatement");
}

void CompilationEngine::compileDo() {
  openTag("doStatement");

  if (m_tokenizer.tokenType() != Token::Keyword || m_tokenizer.keyWord() != Keyword::Do)
    throw std::runtime_error("[ERROR] expected 'do'");
  emitKeyWordAndAdvance();

  if (m_tokenizer.tokenType() != Token::Identifier)
    throw std::runtime_error("[ERROR] expected subroutine or receiver name");
  emitIdentifierAndAdvance();

  if (m_tokenizer.tokenType() == Token::Symbol && m_tokenizer.symbol() == '.') {
    emitSymbolAndAdvance();
    if (m_tokenizer.tokenType() != Token::Identifier)
      throw std::runtime_error("[ERROR] expected subroutineName after '.'");
    emitIdentifierAndAdvance();
  }

  if (m_tokenizer.tokenType() != Token::Symbol || m_tokenizer.symbol() != '(')
    throw std::runtime_error("[ERROR] expected '(' after subroutine name");
  emitSymbolAndAdvance();

  compileExpressionList();

  if (m_tokenizer.tokenType() != Token::Symbol || m_tokenizer.symbol() != ')')
    throw std::runtime_error("[ERROR] expected ')' after arg list");
  emitSymbolAndAdvance();

  if (m_tokenizer.tokenType() != Token::Symbol || m_tokenizer.symbol() != ';')
    throw std::runtime_error("[ERROR] expected ';' after do call");
  emitSymbolAndAdvance();

  closeTag("doStatement");
}

void CompilationEngine::compileReturn() {
  openTag("returnStatement");

  if (m_tokenizer.tokenType() != Token::Keyword || m_tokenizer.keyWord() != Keyword::Return)
    throw std::runtime_error("[ERROR] expected 'return'");
  emitKeyWordAndAdvance();

  if (!(m_tokenizer.tokenType() == Token::Symbol && m_tokenizer.symbol() == ';'))
    compileExpression();

  if (m_tokenizer.tokenType() != Token::Symbol || m_tokenizer.symbol() != ';')
    throw std::runtime_error("[ERROR] expected ';' after return");
  emitSymbolAndAdvance();

  closeTag("returnStatement");
}

void CompilationEngine::compileExpression() {
  openTag("expression");

  compileTerm();

  while (m_tokenizer.tokenType() == Token::Symbol && isOp(m_tokenizer.symbol())) {
    emitSymbolAndAdvance();    // operator
    compileTerm();
  }

  closeTag("expression");
}

void CompilationEngine::compileTerm() {
  openTag("term");

  switch (m_tokenizer.tokenType()) {
    case Token::IntConst:
      emitIntAndAdvance();
      break;

    case Token::StringConst:
      emitStringAndAdvance();
      break;

    case Token::Keyword:
      if (!isKeywordConstant())
        throw std::runtime_error("[ERROR] unexpected keyword in term: " + std::string(m_tokenizer.getCurrentToken()));
      emitKeyWordAndAdvance();
      break;

    case Token::Symbol:
      if (m_tokenizer.symbol() == '(') {
        emitSymbolAndAdvance();
        compileExpression();
        if (m_tokenizer.tokenType() != Token::Symbol || m_tokenizer.symbol() != ')')
          throw std::runtime_error("[ERROR] expected ')' after (expression");
        emitSymbolAndAdvance();
      } else if (isUnaryOp()) {
        emitSymbolAndAdvance();
        compileTerm();
      } else {
        throw std::runtime_error("[ERROR] unexpected symbol in term");
      }
      break;

    case Token::Identifier: {
      emitIdentifierAndAdvance();

      if (m_tokenizer.tokenType() == Token::Symbol && m_tokenizer.symbol() == '[') {
        emitSymbolAndAdvance();
        compileExpression();
        if (m_tokenizer.tokenType() != Token::Symbol || m_tokenizer.symbol() != ']')
          throw std::runtime_error("[ERROR] expected ']' after array index expression");
        emitSymbolAndAdvance();
      } else if (m_tokenizer.tokenType() == Token::Symbol &&
                (m_tokenizer.symbol() == '(' || m_tokenizer.symbol() == '.')) {
        if (m_tokenizer.symbol() == '.') {
          emitSymbolAndAdvance();
          if (m_tokenizer.tokenType() != Token::Identifier)
            throw std::runtime_error("[ERROR] expected subroutineName after '.'");
          emitIdentifierAndAdvance();
        }
        if (m_tokenizer.tokenType() != Token::Symbol || m_tokenizer.symbol() != '(')
          throw std::runtime_error("[ERROR] expected '(' in subroutine call");
        emitSymbolAndAdvance();
        compileExpressionList();
        if (m_tokenizer.tokenType() != Token::Symbol || m_tokenizer.symbol() != ')')
          throw std::runtime_error("[ERROR] expected ')' after argument list");
        emitSymbolAndAdvance();
      }
      break;
    }
  }

  closeTag("term");
}

uint32_t CompilationEngine::compileExpressionList() {
  openTag("expressionList");

  uint32_t n = 0;
  if (!(m_tokenizer.tokenType() == Token::Symbol && m_tokenizer.symbol() == ')')) {
    compileExpression(); ++n;
    while (m_tokenizer.tokenType() == Token::Symbol && m_tokenizer.symbol() == ',') {
      emitSymbolAndAdvance();
      compileExpression(); ++n;
    }
  }

  closeTag("expressionList");
  return n;
}
