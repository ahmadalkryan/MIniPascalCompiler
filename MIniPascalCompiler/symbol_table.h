#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <vector>
#include <unordered_map>

enum SymbolType {
    SYM_VARIABLE,
    SYM_FUNCTION,
    SYM_PROCEDURE,
    SYM_ARRAY,
    SYM_PARAMETER
};

struct Symbol {
    std::string name;
    SymbolType type;
    std::string dataType;
    int arrayStart;
    int arrayEnd;
    std::vector<std::string> parameters;
};

class SymbolTable {
private:
    std::vector<std::unordered_map<std::string, Symbol>> scopes;

public:
    SymbolTable();
    void enterScope(const std::string& scopeName);
    void exitScope();
    bool addSymbol(const Symbol& symbol);
    Symbol* findSymbol(const std::string& name);
    bool isInCurrentScope(const std::string& name);
    void printCurrentScope() const;
};

#endif // SYMBOL_TABLE_H