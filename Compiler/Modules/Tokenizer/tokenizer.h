#pragma once

class Tokenizer {
  public:
    Tokenizer();

    Tokenizer(const Tokenizer&) = delete;
    Tokenizer& operator=(const Tokenizer&) = delete;

    bool hasMoreTokens();

    void advance();
};
