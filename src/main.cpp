#include "ErrorReporter.h"
#include "ExprVisitor.h"
#include "Lexer.h"
#include "Parser.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>

auto run(const std::string &source) -> void {
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scan_tokens();
    Parser parser = Parser(tokens);
    std::optional<Expr> parser_result = parser.parse_input();

    if (ErrorReporter::get_instance().has_error) {
        return;
    }

    std::cout << AstPrinter().print(parser_result.value());
}

auto run_file(const std::filesystem::path &path) -> void {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open provided file");
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();

    run(source);

    if (ErrorReporter::get_instance().has_error) {
        exit(1);
    }
}

auto run_prompt() -> void {
    std::string line;
    std::cout << ">";
    while (std::getline(std::cin, line)) {
        run(line);
        std::cout << ">";
        ErrorReporter::get_instance().has_error = false;
    }
}

auto main(int argc, char *argv[]) -> int {
    std::cout << argv[0] << "\n";

    // Note: argc counts the program itself (i.e. "./jlox") as an argument
    if (argc > 2) {
        throw std::runtime_error("Expected Usage: ./jlox [script]");
    }
    if (argc == 2) {
        // run jlox from the provided file
    } else if (argc == 1) {
        // run jlox as repl
        run_prompt();
    }
    return 0;
}
