#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include "ast.h"
#include "symbol_table.h"
#include "semantic_types.h"  // ����� �������� TypeInfo � DataType

class SemanticAnalyzer {
private:
    SymbolTable symbolTable;
    bool hasErrors;

    // ������� ��� ��� ������
    void checkProgram(ASTNode* node);
    void checkDeclarations(ASTNode* node);
    void checkSubprogram(ASTNode* node);
    void checkStatements(ASTNode* node);

    // ������ �� ��������� ������ ��������
    TypeInfo checkExpression(ASTNode* node);

    // ������ �� ����� �������
    void checkTypeCompatibility(const TypeInfo& type1, const TypeInfo& type2);

public:
    SemanticAnalyzer();
    bool analyze(ASTNode* root);
    bool hasSemanticErrors() const;
};

#endif // SEMANTIC_ANALYZER_H