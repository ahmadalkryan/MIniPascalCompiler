#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include "ast.h"
#include "symbol_table.h"
#include "semantic_types.h"  // ÷—Ê—Ì ·«” Œœ«„ TypeInfo Ê DataType

class SemanticAnalyzer {
private:
    SymbolTable symbolTable;
    bool hasErrors;

    // «· Õ·Ì· Õ”» ‰Ê⁄ «·⁄ﬁœ…
    void checkProgram(ASTNode* node);
    void checkDeclarations(ASTNode* node);
    void checkSubprogram(ASTNode* node);
    void checkStatements(ASTNode* node);

    // «· Õﬁﬁ „‰ «· ⁄»Ì—«  Ê√‰Ê«⁄ «·»Ì«‰« 
    TypeInfo checkExpression(ASTNode* node);

    // «· Õﬁﬁ „‰  Ê«›ﬁ «·√‰Ê«⁄
    void checkTypeCompatibility(const TypeInfo& type1, const TypeInfo& type2);

public:
    SemanticAnalyzer();
    bool analyze(ASTNode* root);
    bool hasSemanticErrors() const;
};

#endif // SEMANTIC_ANALYZER_H