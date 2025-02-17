#include "Parser.h"
#include "ErrorReporter.h"
#include "Expr.h"
#include "Token.h"
#include <initializer_list>
#include <memory>
#include <optional>

using std::make_unique;

auto Parser::parse_input() -> std::optional<Expr> {
    try {
        return parse_expression();
    } catch (...) {
        return std::nullopt;
    }
}

auto Parser::parse_expression() -> Expr { return parse_equality(); }

auto Parser::parse_equality() -> Expr {
    /*equality → comparison ( ( "!=" | "==" ) comparison )* ;*/
    Expr expr = parse_comparison();
    while (match({TokenType::BANG_EQUAL, TokenType::BANG})) {
        Token op = previous();
        Expr right = parse_comparison();
        expr = make_unique<Binary>(std::move(expr), op, std::move(right));
    }
    return expr;
}

auto Parser::parse_comparison() -> Expr {
    /*comparison → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;*/
    Expr expr = parse_term();
    while (match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS,
                  TokenType::LESS_EQUAL})) {
        Token op = previous();
        Expr right = parse_term();
        expr = make_unique<Binary>(std::move(expr), op, std::move(right));
    }
    return expr;
}

auto Parser::parse_term() -> Expr {
    /*term → factor ( ( "-" | "+" ) factor )* ;*/
    Expr expr = parse_factor();
    while (match({TokenType::PLUS, TokenType::MINUS})) {
        Token op = previous();
        Expr right = parse_factor();
        expr = make_unique<Binary>(std::move(expr), op, std::move(right));
    }
    return expr;
}

auto Parser::parse_factor() -> Expr {
    /*factor → unary ( ( "/" | "*" ) unary )* ;*/
    Expr expr = parse_unary();
    while (match({TokenType::SLASH, TokenType::STAR})) {
        Token op = previous();
        Expr right = parse_unary();
        expr = make_unique<Binary>(std::move(expr), op, std::move(right));
    }
    return expr;
}

// This function has to return Exprs of the type Unary
auto Parser::parse_unary() -> Expr {
    /*unary → ( "!" | "-" ) unary | primary ;*/
    if (match({TokenType::BANG, TokenType::MINUS})) {
        Token op = previous();
        auto unary_expr = std::get<unique_ptr<Unary>>(parse_unary());
        auto unary = std::make_pair(op, std::move(unary_expr));
        return make_unique<Unary>(std::move(unary));
    }

    auto primary_expr = std::get<unique_ptr<Primary>>(parse_primary());
    return make_unique<Unary>(std::move(primary_expr));
}

auto Parser::parse_primary() -> Expr {
    /*primary → NUMBER | STRING | "true" | "false" | "nil" | "(" expression ")"
     */
    Token token = advance();
    if (match({TokenType::NUMBER, TokenType::STRING, TokenType::TRUE,
               TokenType::FALSE, TokenType::NIL})) {
        return make_unique<Primary>(previous());
    }

    if (match({TokenType::LEFT_PAREN})) {
        Expr expr = parse_expression();
        consume(TokenType::RIGHT_PAREN, "Expected ')' after expression");
        return expr;
    }

    throw error(peek(), "Expect expression.");
}

/* Error Handling */
auto Parser::consume(TokenType type, const std::string &msg) -> Token {
    if (check_type(type)) {
        return advance();
    }
    throw error(peek(), msg);
}

auto Parser::error(Token token, const std::string &msg) -> ParseError {
    ErrorReporter &reporter = ErrorReporter::get_instance();
    reporter.has_error = true;
    reporter.report_parser_error(token, msg);
    return ParseError();
}

// When we encounter an error, go forward to the start of the next statement, so
// we can keep parsing to find further errors
auto Parser::synchronize() -> void {
    advance();

    while (!is_at_end()) {
        if (previous().type == TokenType::SEMICOLON) {
            return;
        }
        switch (peek().type) {
        case TokenType::CLASS:
        case TokenType::FUN:
        case TokenType::VAR:
        case TokenType::FOR:
        case TokenType::IF:
        case TokenType::WHILE:
        case TokenType::PRINT:
        case TokenType::RETURN:
            return;
        default:
            advance();
            break;
        }
    }
}

/* Util Functions */
// Checks if the next token matches ANY of the given types
// and consumes if it does
auto Parser::match(std::initializer_list<TokenType> types) -> bool {
    for (TokenType type : types) {
        if (check_type(type)) {
            advance();
            return true;
        }
    }
    return false;
}

auto Parser::peek() -> Token {
    if (is_at_end()) {
        return Token::create_eof();
    }
    return tokens.at(current);
}

auto Parser::check_type(TokenType type) -> bool { return peek().type == type; }

auto Parser::advance() -> Token {
    if (is_at_end()) {
        return Token::create_eof();
    }
    current++;
    return previous();
}

auto Parser::is_at_end() -> bool { return current >= tokens.size(); }

auto Parser::previous() -> Token { return tokens.at(current - 1); }
