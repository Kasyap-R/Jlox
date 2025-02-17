#pragma once

#include "Expr.h"
#include "Token.h"
#include <exception>
#include <initializer_list>
#include <optional>
#include <vector>

class Parser {
  public:
    auto parse_input() -> std::optional<Expr>;
    Parser(const std::vector<Token> &tokens) : tokens(tokens) {}

  private:
    class ParseError : public std::exception {};

    std::vector<Token> tokens;
    int current = 0;

    auto parse_expression() -> Expr;
    auto parse_equality() -> Expr;
    auto parse_comparison() -> Expr;
    auto parse_term() -> Expr;
    auto parse_factor() -> Expr;
    auto parse_unary() -> Expr;
    auto parse_primary() -> Expr;

    // Util
    auto match(std::initializer_list<TokenType> types) -> bool;
    auto peek() -> Token;
    auto check_type(TokenType type) -> bool;
    auto advance() -> Token;
    auto is_at_end() -> bool;
    auto previous() -> Token;

    // Error Handling
    auto consume(TokenType type, const std::string &msg) -> Token;
    auto error(Token token, const std::string &msg) -> ParseError;
    auto synchronize() -> void;
};
