#include "Lexer.h"
#include "ErrorReporter.h"
#include "Token.h"

std::map<std::string, TokenType> Lexer::string_to_ttype{
    {"and", TokenType::AND},       {"class", TokenType::CLASS},
    {"else", TokenType::ELSE},     {"false", TokenType::FALSE},
    {"for", TokenType::FOR},       {"fun", TokenType::FUN},
    {"if", TokenType::IF},         {"nil", TokenType::NIL},
    {"or", TokenType::OR},         {"print", TokenType::PRINT},
    {"return", TokenType::RETURN}, {"super", TokenType::SUPER},
    {"this", TokenType::THIS},     {"true", TokenType::TRUE},
    {"var", TokenType::VAR},       {"while", TokenType::WHILE}};

Lexer::Lexer(std::string source) : source(std::move(source)) {}

auto Lexer::scan_token() -> void {
    char c = advance();
    switch (c) {
    case '(':
        add_token(TokenType::LEFT_PAREN);
        break;
    case ')':
        add_token(TokenType::RIGHT_PAREN);
        break;
    case '{':
        add_token(TokenType::LEFT_BRACE);
        break;
    case '}':
        add_token(TokenType::RIGHT_BRACE);
        break;
    case ',':
        add_token(TokenType::COMMA);
        break;
    case '.':
        add_token(TokenType::DOT);
        break;
    case '-':
        add_token(TokenType::MINUS);
        break;
    case '+':
        add_token(TokenType::PLUS);
        break;
    case ';':
        add_token(TokenType::SEMICOLON);
        break;
    case '*':
        add_token(TokenType::STAR);
        break;
    case '/': {
        if (match('/')) {
            handle_comment();
        } else if (match('*')) {
            handle_comment_block();
        } else {
            add_token(TokenType::SLASH);
        }
        break;
    }
    case '!':
        optional_two_char('=', TokenType::BANG, TokenType::BANG_EQUAL);
        break;
    case '=':
        optional_two_char('=', TokenType::EQUAL, TokenType::EQUAL_EQUAL);
        break;
    case '<':
        optional_two_char('=', TokenType::LESS, TokenType::LESS_EQUAL);
        break;
    case '>':
        optional_two_char('=', TokenType::GREATER, TokenType::GREATER_EQUAL);
        break;
    case '"':

    case ' ':
    case '\r':
    case '\t':
        // Ignore whitespace.
        break;
    case '\n':
        line++;
        break;
    default:
        if (is_digit(c)) {
            handle_number();
        } else if (is_alpha(c)) {
            handle_identifier();
        } else {
            syntax_error(line, "Unexpected Character");
        }
        break;
    }
}

auto Lexer::handle_comment() -> void {
    while (peek() != '\n' && !is_at_end()) {
        advance();
    }
}

auto Lexer::handle_comment_block() -> void {
    int level_nested = 1;
    while (!is_at_end() && level_nested != 0) {
        char c = peek();
        advance();
        if (c == '/') {
            if (match('*')) {
                level_nested++;
            }
        } else if (c == '*') {
            if (match('\\')) {
                level_nested--;
            }
        }
    }
}
/*  */

auto Lexer::handle_identifier() -> void {
    while (is_alpha(peek())) {
        advance();
    }
    std::string identifier = source.substr(start, current - start);

    TokenType type = TokenType::IDENTIFIER;
    if (string_to_ttype.contains(identifier)) {
        type = string_to_ttype[identifier];
    }

    add_token(type);
}

auto Lexer::handle_number() -> void {
    while (is_digit(peek())) {
        advance();
    }
    if (peek() == '.' && is_digit(peek(1))) {
        advance();
        while (is_digit(peek())) {
            advance();
        }
    }

    add_token(TokenType::NUMBER);
}

auto Lexer::handle_string() -> void {
    while (peek() != '"' && !is_at_end()) {
        if (peek() == '\n') {
            line++;
        }
        advance();
    }

    if (is_at_end()) {
        syntax_error(line, "Unterminated String");
        return;
    }

    // Our pointers should be in proper position so that add_token() can use
    // them to get the substring corresponding to the string
    start++;
    add_token(TokenType::STRING);
    advance(); // Consume the last '"'
}

auto Lexer::optional_two_char(const char &optional, TokenType single_type,
                              TokenType duo_type) -> void {
    if (match(optional)) {
        add_token(duo_type);
    } else {
        add_token(single_type);
    }
}

// Conditionally advances if the char matches
auto Lexer::match(const char &expected) -> bool {
    if (is_at_end()) {
        return false;
    }

    if (source.at(current) != expected) {
        return false;
    }

    current++;
    return true;
}

auto Lexer::peek(int amount_to_peek) -> char {
    if (current + amount_to_peek >= source.length()) {
        return '\0';
    }
    return source.at(current + amount_to_peek);
}

auto Lexer::is_digit(char c) -> bool { return c >= '0' && c <= '9'; }

auto Lexer::is_alpha(char c) -> bool {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

auto Lexer::advance() -> char { return source.at(current++); }

auto Lexer::is_at_end() -> bool { return current >= source.length(); }

auto Lexer::add_token(TokenType type) -> void {
    std::string lexeme = source.substr(start, current - start);
    tokens.emplace_back(type, lexeme, line);
}

auto Lexer::syntax_error(int line, const std::string &msg) -> void {
    ErrorReporter &reporter = ErrorReporter::get_instance();
    reporter.has_error = true;
    reporter.report_error(line, msg);
}

auto Lexer::scan_tokens() -> std::vector<Token> {
    while (!is_at_end()) {
        start = current;
        scan_token();
    }
    tokens.emplace_back(TokenType::EoF, "", line);
    return tokens;
}
