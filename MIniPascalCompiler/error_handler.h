#pragma once
#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include <string>
#include <vector>

struct Error {
    std::string message;
    int line;
    int column;
};

class ErrorHandler {
public:
    void add_error(const std::string& message, int line, int column);
    void print_errors() const;
    bool has_errors() const;
    void clear();

private:
    std::vector<Error> errors;
};

#endif