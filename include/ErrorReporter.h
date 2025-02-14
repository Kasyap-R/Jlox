#pragma once

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

    auto report_error(const int line, const std::string &msg) -> void {
        std::cout << "[Line " << line << "]: " << msg << "\n";
    }

  private:
    ErrorReporter() {}
};
