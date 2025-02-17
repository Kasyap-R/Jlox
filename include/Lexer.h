#pragma once

#include "Token.h"
#include <map>
#include <string>
#include <vector>

class Lexer {
  public:
    Lexer(std::string source);
    auto scan_tokens() -> std::vector<Token>;

  private:
    const std::string source;
    static std::map<std::string, TokenType> string_to_ttype;
    std::vector<Token> tokens;
    int start = 0;
    int current = 0;
    int line = 1;

    static auto syntax_error(const int line, const std::string &msg) -> void;

    auto is_at_end() -> bool;
    auto scan_token() -> void;
    auto advance() -> char;
    auto add_token(TokenType type) -> void;
    auto match(const char &expected) -> bool;
    auto peek(int amount_to_peek = 0) -> char;

    auto optional_two_char(const char &optional, TokenType single_type,
                           TokenType duo_type) -> void;

    auto handle_comment() -> void;
    auto handle_comment_block() -> void;
    auto handle_string() -> void;
    auto handle_number() -> void;
    auto handle_identifier() -> void;

    static auto is_digit(char c) -> bool;
    static auto is_alpha(char c) -> bool;
};
