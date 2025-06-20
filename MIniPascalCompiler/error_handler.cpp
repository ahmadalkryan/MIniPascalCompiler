#include "error_handler.h"
#include <iostream>

void ErrorHandler::add_error(const std::string& message, int line, int column) {
    errors.push_back({ message, line, column });
}

void ErrorHandler::print_errors() const {
    for (const auto& error : errors) {
        std::cerr << "Error at line " << error.line
            << ", column " << error.column
            << ": " << error.message << std::endl;
    }
}

bool ErrorHandler::has_errors() const {
    return !errors.empty();
}

void ErrorHandler::clear() {
    errors.clear();
}