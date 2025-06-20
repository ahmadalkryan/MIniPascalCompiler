#include "symbol_table.h"
#include <iostream>

SymbolTable::SymbolTable() {
    // Start with global scope
    enterScope("global");
}

void SymbolTable::enterScope(const std::string& scopeName) {
    scopes.push_back(std::unordered_map<std::string, Symbol>());
}

void SymbolTable::exitScope() {
    if (!scopes.empty()) {
        scopes.pop_back();
    }
}

bool SymbolTable::addSymbol(const Symbol& symbol) {
    if (scopes.empty() || isInCurrentScope(symbol.name)) {
        return false;
    }

    scopes.back()[symbol.name] = symbol;
    return true;
}

Symbol* SymbolTable::findSymbol(const std::string& name) {
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        auto found = it->find(name);
        if (found != it->end()) {
            return &found->second;
        }
    }
    return nullptr;
}

bool SymbolTable::isInCurrentScope(const std::string& name) {
    if (scopes.empty()) return false;
    return scopes.back().find(name) != scopes.back().end();
}

void SymbolTable::printCurrentScope() const {
    if (scopes.empty()) return;

    for (const auto& pair : scopes.back()) {
        std::cout << pair.first << " : " << pair.second.dataType << std::endl;
    }
}