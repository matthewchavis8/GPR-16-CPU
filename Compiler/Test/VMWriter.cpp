#include "gtest/gtest.h"
#include <cassert>
#include <filesystem>
#include <fstream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include "../Modules/VMWriter/vmWriter.h"


class VmWriter_F : public testing::Test {
 protected:
  std::filesystem::path filePath;
  std::ofstream file;
  std::unique_ptr<VmWriter> vmWriter;


  void SetUp() override {
    filePath = std::filesystem::temp_directory_path() / "test.vm";
    file.open(filePath);
    vmWriter = std::make_unique<VmWriter>(file);
  }

  void TearDown() override {
    vmWriter->close();
    std::filesystem::remove(filePath);
  }

  std::string fetchFileContent() {
    std::ifstream input(filePath);
    std::stringstream buff;
    buff << input.rdbuf();

    return buff.str();
  }
};

TEST_F(VmWriter_F, exception_is_thrown_when_file_is_not_open) {
  std::filesystem::path tmp_file_path = std::filesystem::temp_directory_path() / "tmp.vm";
  std::ofstream tmpFile(tmp_file_path);
  tmpFile.close();

  ASSERT_THROW(VmWriter tmpVmWriter(tmpFile), std::runtime_error);
}
TEST_F(VmWriter_F, can_write_push_and_pop) {
  vmWriter->writePush(Segment::Argument, 2);
  vmWriter->writePop(Segment::Local, 5);

  vmWriter->close();
  std::string res { fetchFileContent() };
  std::string expected { "push argument 2\npop local 5\n" };

  ASSERT_EQ(res, expected);
}

TEST_F(VmWriter_F, can_write_arithmetic) {
  vmWriter->writeArithmetic(Command::Not);

  vmWriter->close();
  std::string res { fetchFileContent() };
  std::string expected { "not\n" };

  ASSERT_EQ(res, expected);
}

TEST_F(VmWriter_F, can_write_label) {
  vmWriter->writeLabel("foo");
  
  vmWriter->close();
  std::string res { fetchFileContent() };
  std::string expected { "label foo\n" };

  ASSERT_EQ(res, expected);
}

TEST_F(VmWriter_F, can_write_goto) {
  vmWriter->writeGoto("foo");
  
  vmWriter->close();
  std::string res { fetchFileContent() };
  std::string expected { "goto foo\n" };

  ASSERT_EQ(res, expected);
}

TEST_F(VmWriter_F, can_write_call) {
  vmWriter->writeCall("foo.hello()", 2);
  
  vmWriter->close();
  std::string res { fetchFileContent() };
  std::string expected { "call foo.hello() 2\n" };

  ASSERT_EQ(res, expected);
}

TEST_F(VmWriter_F, can_write_if) {
  vmWriter->writeIf("LOOP");
  
  vmWriter->close();
  std::string res { fetchFileContent() };
  std::string expected { "if-goto LOOP\n" };

  ASSERT_EQ(res, expected);
}

TEST_F(VmWriter_F, can_write_function) {
  vmWriter->writeFunction("foo()", 2);

  vmWriter->close();
  std::string res { fetchFileContent() };
  std::string expected { "function foo() 2\n" };

  ASSERT_EQ(res, expected);
}

TEST_F(VmWriter_F, can_write_return) {
  vmWriter->writeReturn();
  
  vmWriter->close();
  std::string res { fetchFileContent() };
  std::string expected { "return\n" };

  ASSERT_EQ(res, expected);
}
