
#include "codeWriter.h"
#include <stdexcept>

CodeWriter::CodeWriter(const std::string& fileName) {
  setFileName(fileName);
}


void CodeWriter::setFileName(const std::string& fileName) {
  std::size_t n = fileName.length();

  if (fileName.substr(n - 4) != ".asm")
    std::runtime_error("[ERROR] Invalid filename format must have .vm in the end\n");

  m_file_name = std::move(fileName);

  m_output_file.open(m_file_name);
}

void CodeWriter::writeArithmetic(const std::string& command) {
    m_output_file << "// " << command << "\n";

    if (command == "add" || command == "sub" || command == "and" || command == "or") {
        m_output_file <<
            "@SP\n"
            "AM=M-1\n"
            "D=M\n"
            "A=A-1\n";

        if (command == "add")
         m_output_file << "M=M+D\n";
        else if (command == "sub")
         m_output_file << "M=M-D\n";
        else if (command == "and")
         m_output_file << "M=M&D\n";
        else 
         m_output_file << "M=M|D\n";
    }

    else if (command == "neg" || command == "not") {
        m_output_file <<
            "@SP\n"
            "A=M-1\n";

        if (command == "neg")
         m_output_file << "M=-M\n";
        else
         m_output_file << "M=!M\n";
    }

    else if (command == "eq" || command == "gt" || command == "lt") {
        std::string trueLabel = "TRUE" + std::to_string(m_labelCounter);
        std::string endLabel  = "END"  + std::to_string(m_labelCounter);
        ++m_labelCounter;

        m_output_file <<
            "@SP\n"
            "AM=M-1\n"
            "D=M\n"
            "A=A-1\n"
            "D=M-D\n"
            "@" << trueLabel << "\n";

        if (command == "eq")      
          m_output_file << "D;JEQ\n";
        else if (command == "gt") 
          m_output_file << "D;JGT\n";
        else 
          m_output_file << "D;JLT\n";

        m_output_file <<
            "@SP\n"
            "A=M-1\n"
            "M=0\n"
            "@" << endLabel << "\n"
            "0;JMP\n"
            "(" << trueLabel << ")\n"
            "@SP\n"
            "A=M-1\n"
            "M=-1\n"
            "(" << endLabel << ")\n";
    }

    else
      throw std::runtime_error("Unknown arithmetic command: " + command);
}

void CodeWriter::writePushPop(CommandType cmdType, const std::string& segment, int idx) {
    const std::string i = std::to_string(idx);

    if (cmdType == C_PUSH) {
        if (segment == "constant") {
            m_output_file <<
                "@" << i << "\n"
                "D=A\n"
                "@SP\n"
                "A=M\n"
                "M=D\n"
                "@SP\n"
                "M=M+1\n";
        }
        else if (segment == "local" || segment == "argument" || segment == "this" || segment == "that") {
            const std::string base = (segment == "local") ? "LCL" : (segment == "argument") ? "ARG" : (segment == "this") ? "THIS" : "THAT";
            m_output_file <<
                "@" << base << "\n"
                "D=M\n"
                "@" << i << "\n"
                "A=D+A\n"
                "D=M\n"
                "@SP\n"
                "A=M\n"
                "M=D\n"
                "@SP\n"
                "M=M+1\n";
        }
        else if (segment == "temp") {
            m_output_file <<
                "@R" << (5 + idx) << "\n"
                "D=M\n"
                "@SP\n"
                "A=M\n"
                "M=D\n"
                "@SP\n"
                "M=M+1\n";
        }
        else if (segment == "pointer") {
            const std::string ptr = (idx == 0) ? "THIS" : "THAT";
            m_output_file <<
                "@" << ptr << "\n"
                "D=M\n"
                "@SP\n"
                "A=M\n"
                "M=D\n"
                "@SP\n"
                "M=M+1\n";
        }
        else if (segment == "static") {
            m_output_file <<
                "@Static." << idx << "\n"
                "D=M\n"
                "@SP\n"
                "A=M\n"
                "M=D\n"
                "@SP\n"
                "M=M+1\n";
        }
        else {
            throw std::runtime_error("Unknown segment in push: " + segment);
        }
    }
    else if (cmdType == C_POP) {
        if (segment == "local" || segment == "argument" || segment == "this" || segment == "that") {
            const std::string base = (segment == "local") ? "LCL" : (segment == "argument") ? "ARG" : (segment == "this") ? "THIS" : "THAT";
            m_output_file <<
                "@" << base << "\n"
                "D=M\n"
                "@" << i << "\n"
                "D=D+A\n"
                "@R13\n"
                "M=D\n"
                "@SP\n"
                "AM=M-1\n"
                "D=M\n"
                "@R13\n"
                "A=M\n"
                "M=D\n";
        }
        else if (segment == "temp") {
            m_output_file <<
                "@SP\n"
                "AM=M-1\n"
                "D=M\n"
                "@R" << (5 + idx) << "\n"
                "M=D\n";
        }
        else if (segment == "pointer") {
            const std::string ptr = (idx == 0) ? "THIS" : "THAT";
            m_output_file <<
                "@SP\n"
                "AM=M-1\n"
                "D=M\n"
                "@" << ptr << "\n"
                "M=D\n";
        }
        else if (segment == "static") {
            m_output_file <<
                "@SP\n"
                "AM=M-1\n"
                "D=M\n"
                "@Static." << idx << "\n"
                "M=D\n";
        }
        else {
            throw std::runtime_error("Unknown segment in pop: " + segment);
        }
    }
    else {
        throw std::logic_error("writePushPop called with non push/pop CommandType");
    }
}

void CodeWriter::close() { m_output_file.close(); }
