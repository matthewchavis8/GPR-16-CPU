
#include "gtest/gtest.h"
#include <filesystem>
#include <fstream>
#include <memory>
#include "../Modules/Tokenizer/tokenizer.h"

using namespace testing;

class TokenizerTestObject {
  protected:
    std::filesystem::path filepath;
    std::ifstream filestream;
    std::unique_ptr<Tokenizer>tokenizer;

    void SetUp() override {
      filepath = std::filesystem::temp_directory_path() / "tmp.jack";
      
      {
        std::ofstream file(filepath);
        file << "/** Tokenizer Torture Test: covers keywords, symbols, ints, strings, comments. */" << '\n';
        file << "// Symbols to see (ignored here because in comment): {}()[] . , ; + - * / & | < > = ~" << '\n';
        file << "/* Multi-line comment:" << '\n';
        file << "   Make sure the tokenizer skips all of this, even with symbols like [ ] { } ( ) . , ; + - * / & | < > = ~" << '\n';
        file << "*/" << '\n';
        file << "" << '\n';
        file << "class TokenZoo {" << '\n';
        file << "    static boolean FLAG;               // keyword 'static', type 'boolean', identifier, symbol ';'" << '\n';
        file << "    field int count, i;                // multiple field decls" << '\n';
        file << "    field Array arr;                   // user-defined type" << '\n';
        file << "" << '\n';
        file << "    constructor TokenZoo new(int size) {" << '\n';
        file << "        var int j;                     // 'var' decl" << '\n';
        file << "        let count = 0;                 // intConstant 0" << '\n';
        file << "        let i = -1;                    // unary '-'" << '\n';
        file << "        let arr = Array.new(size);     // subroutine call with dot" << '\n';
        file << "        return this;                   // 'this' keyword" << '\n';
        file << "    }" << '\n';
        file << "" << '\n';
        file << "    method void set(int index, int value) {" << '\n';
        file << "        // array write with arithmetic and symbols" << '\n';
        file << "        let arr[index] = value + 1;" << '\n';
        file << "        return;" << '\n';
        file << "    }" << '\n';
        file << "" << '\n';
        file << "    function int sum() {" << '\n';
        file << "        var int s;" << '\n';
        file << "        let s = 0;" << '\n';
        file << "        let i = 0;" << '\n';
        file << "        while (i < arr.length()) {     // 'while', relational '<', dot call, parens/braces" << '\n';
        file << "            let s = s + arr[i];" << '\n';
        file << "            let i = i + 1;" << '\n';
        file << "        }" << '\n';
        file << "        return s;" << '\n';
        file << "    }" << '\n';
        file << "" << '\n';
        file << "    method void demo() {" << '\n';
        file << "        var int a, b;" << '\n';
        file << "        var boolean ok;" << '\n';
        file << "        var char c;" << '\n';
        file << "" << '\n';
        file << "        let a = 32767;                 // max Jack int" << '\n';
        file << "        let b = 0;" << '\n';
        file << "        let ok = true;                 // 'true' keyword" << '\n';
        file << "        let c = 65;                    // 'A' as int (Jack char is int under the hood)" << '\n';
        file << "        let this.count = a;            // field via 'this'" << '\n';
        file << "        let TokenZoo.FLAG = false;     // static via class name" << '\n';
        file << "" << '\n';
        file << "        // if/else with unary '~', bitwise '|', and grouping" << '\n';
        file << "        if (~ok | (a > b)) {" << '\n';
        file << "            do Output.printString(\"hello, world <>&\");" << '\n';
        file << "            do Output.println();" << '\n';
        file << "        } else {" << '\n';
        file << "            do Output.printInt(a * (b + 2));" << '\n';
        file << "            do Output.println();" << '\n';
        file << "        }" << '\n';
        file << "" << '\n';
        file << "        // Use 'null' and array indexing read" << '\n';
        file << "        if (arr = null) {" << '\n';
        file << "            let arr = Array.new(3);" << '\n';
        file << "            let arr[0] = 10;" << '\n';
        file << "            let arr[1] = 20;" << '\n';
        file << "            let arr[2] = 30;" << '\n';
        file << "        }" << '\n';
        file << "" << '\n';
        file << "        // String with spaces and punctuation (no embedded quotes)" << '\n';
        file << "        do Output.printString(\"Tokenizer test: symbols .,;+-*/&|<>=~ and brackets []{}()\");" << '\n';
        file << "        do Output.println();" << '\n';
        file << "" << '\n';
        file << "        // Call some OS functions to create more tokens" << '\n';
        file << "        do Output.printChar(c);        // should print 'A'" << '\n';
        file << "        do Output.println();" << '\n';
        file << "        do Output.printInt(Math.multiply(2, 3));" << '\n';
        file << "        do Output.println();" << '\n';
        file << "" << '\n';
        file << "        return;" << '\n';
        file << "    }" << '\n';
        file << "}" << '\n';
        file << "" << '\n';
        file << "// trailing single-line comment with symbols: {}()[].,;+-*/&|<>=~ and spaces" << '\n';
      }
      filestream.open(filepath, std::ios::in);

      tokenizer = std::make_unique<Tokenizer>(filestream, filepath.string());
    }

    void TearDown() override {
      filestream.close();
      std::filesystem::remove(filepath);
    }
};
