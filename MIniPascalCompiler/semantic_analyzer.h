#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include "ast.h"
#include "symbol_table.h"

class SemanticAnalyzer {
private:
    SymbolTable symbolTable;
    bool hasErrors;

    void checkProgram(ASTNode* node);
    void checkDeclarations(ASTNode* node);
    void checkSubprogram(ASTNode* node);
    void checkStatements(ASTNode* node);
    void checkExpression(ASTNode* node);
    void checkTypeCompatibility(ASTNode* type1, ASTNode* type2);

public:
    SemanticAnalyzer();
    bool analyze(ASTNode* root);
    bool hasSemanticErrors() const;
};

#endif // SEMANTIC_ANALYZER_H