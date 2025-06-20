#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include "ast.h"
#include <string>
#include <fstream>

class CodeGenerator {
public:
    CodeGenerator(const std::string& outputFilename);

    void generate(ASTNode* root);

private:
    std::ofstream outFile;

    void visitProgram(ASTNode* node);
    void visitDeclarations(ASTNode* node);
    void visitSubprogram(ASTNode* node);
    void visitCompoundStatement(ASTNode* node);
    void visitStatement(ASTNode* node);
    void visitAssignment(ASTNode* node);
    void visitIfStatement(ASTNode* node);
    void visitWhileLoop(ASTNode* node);
    void visitProcedureCall(ASTNode* node);
    void visitFunctionCall(ASTNode* node);
    void visitExpression(ASTNode* node);
    void visitVariable(ASTNode* node);
    void visitArrayAccess(ASTNode* node);
    void visitBinaryOp(ASTNode* node);
    void visitUnaryOp(ASTNode* node);
    void visitLiteral(ASTNode* node);
};

#endif // CODE_GENERATOR_H#pragma once
