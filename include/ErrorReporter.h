#pragma once

#include "Token.h"
#include <iostream>
#include <string>

class ErrorReporter {
  public:
    bool has_error = false;

    // Delete copy and assign operator to enforce singleton
    ErrorReporter(const ErrorReporter &) = delete;
    ErrorReporter &operator=(const ErrorReporter &) = delete;

    static auto get_instance() -> ErrorReporter & {
        static ErrorReporter instance; // Guaranteed to be initialized once
        return instance;
    }

    auto report_lexer_error(const int line, const std::string &msg) {
        std::cout << "Syntax Error [Line " << line << "]: " << msg << "\n";
    }

    auto report_parser_error(const Token token, const std::string &msg) {
        std::string where;
        if (token.type == TokenType::EoF) {
            where = " at end";
        } else {
            where = " at ";
        }
        std::cout << "[line " << token.line_num << "] Error" << where << ": "
                  << msg << "\n";
    }

  private:
    ErrorReporter() {}
};
