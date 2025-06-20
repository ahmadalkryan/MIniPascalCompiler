#include "code_generation.h"
#include "semantic_analyzer.h"
#include "ast.h"
#include "symbol_table.h"
#include <iostream>

CodeGenerator::CodeGenerator(const std::string& outputFilename) {
    outFile.open(outputFilename);
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open output file: " << outputFilename << std::endl;
        exit(1);
    }
}

void CodeGenerator::generate(ASTNode* root) {
    if (!root) return;

    outFile << "#include <iostream>\n";
    outFile << "#include <string>\n";
    outFile << "using namespace std;\n\n";

    visitProgram(root);

    outFile.close();
}

void CodeGenerator::visitProgram(ASTNode* node) {
    outFile << "int main() {\n";

    // Local variables
    if (node->children.size() > 0 && node->children[0]) {
        visitDeclarations(node->children[0]);
    }

    // Main compound statement
    if (node->children.size() > 2 && node->children[2]) {
        visitCompoundStatement(node->children[2]);
    }

    outFile << "    return 0;\n}\n";
}

void CodeGenerator::visitDeclarations(ASTNode* node) {
    if (!node || node->type != NODE_DECLARATIONS) return;

    for (ASTNode* decl : node->children) {
        if (decl->type == NODE_DECLARATIONS) {
            ASTNode* ids = decl->children[0];
            ASTNode* typeNode = decl->children[1];

            std::string cType;
            if (typeNode->type == NODE_TYPE) {
                if (typeNode->name == "integer") cType = "int";
                else if (typeNode->name == "real") cType = "double";
                else if (typeNode->name == "boolean") cType = "bool";
                else cType = "unknown";
            }
            else if (typeNode->type == NODE_ARRAY_TYPE) {
                ASTNode* baseTypeNode = typeNode->children[0];
                std::string baseType;
                if (baseTypeNode->name == "integer") baseType = "int";
                else if (baseTypeNode->name == "real") baseType = "double";
                else if (baseTypeNode->name == "boolean") baseType = "bool";
                else baseType = "unknown";

                int size = static_cast<int>(typeNode->real_val) - typeNode->int_val + 1;
                for (ASTNode* idNode : ids->children) {
                    outFile << "    " << baseType << " " << idNode->name << "[" << size << "];\n";
                }
                continue;
            }

            for (ASTNode* idNode : ids->children) {
                outFile << "    " << cType << " " << idNode->name << ";\n";
            }
        }
    }

    outFile << "\n";
}

void CodeGenerator::visitCompoundStatement(ASTNode* node) {
    for (ASTNode* stmt : node->children) {
        visitStatement(stmt);
    }
}

void CodeGenerator::visitStatement(ASTNode* node) {
    switch (node->type) {
    case NODE_ASSIGNMENT:
        visitAssignment(node);
        break;
    case NODE_IF:
        visitIfStatement(node);
        break;
    case NODE_WHILE:
        visitWhileLoop(node);
        break;
    case NODE_PROCEDURE_CALL:
        visitProcedureCall(node);
        break;
    case NODE_FUNCTION_CALL:
        visitFunctionCall(node);
        break;
    default:
        break;
    }
}

void CodeGenerator::visitAssignment(ASTNode* node) {
    ASTNode* var = node->left;
    ASTNode* expr = node->right;

    outFile << "    ";
    visitVariable(var);
    outFile << " = ";
    visitExpression(expr);
    outFile << ";\n";
}

void CodeGenerator::visitIfStatement(ASTNode* node) {
    outFile << "    if (";
    visitExpression(node->left);
    outFile << ") {\n";

    visitCompoundStatement(node->right);
    outFile << "    }\n";

    if (!node->children.empty()) {
        outFile << "    else {\n";
        visitCompoundStatement(node->children[0]->right);
        outFile << "    }\n";
    }
}

void CodeGenerator::visitWhileLoop(ASTNode* node) {
    outFile << "    while(";
    visitExpression(node->left);
    outFile << ") {\n";

    visitCompoundStatement(node->right);
    outFile << "    }\n";
}

void CodeGenerator::visitProcedureCall(ASTNode* node) {
    outFile << "    " << node->name << "(";
    if (node->children.size() > 0) {
        ASTNode* args = node->children[0];
        for (size_t i = 0; i < args->children.size(); ++i) {
            if (i > 0) outFile << ", ";
            visitExpression(args->children[i]);
        }
    }
    outFile << ");\n";
}

void CodeGenerator::visitFunctionCall(ASTNode* node) {
    outFile << "    " << node->name << "(";
    if (node->children.size() > 0) {
        ASTNode* args = node->children[0];
        for (size_t i = 0; i < args->children.size(); ++i) {
            if (i > 0) outFile << ", ";
            visitExpression(args->children[i]);
        }
    }
    outFile << ");\n";
}

void CodeGenerator::visitVariable(ASTNode* node) {
    outFile << node->name;
    if (node->type == NODE_ARRAY_ACCESS) {
        outFile << "[";
        visitExpression(node->children[1]);
        outFile << "]";
    }
}

void CodeGenerator::visitArrayAccess(ASTNode* node) {
    outFile << node->children[0]->name << "[";
    visitExpression(node->children[1]);
    outFile << "]";
}

void CodeGenerator::visitExpression(ASTNode* node) {
    if (!node) return;

    switch (node->type) {
    case NODE_INT_NUM:
        outFile << node->int_val;
        break;
    case NODE_REAL_NUM:
        outFile << node->real_val;
        break;
    case NODE_BOOLEAN:
        outFile << (node->bool_val ? "true" : "false");
        break;
    case NODE_BINARY_OP:
        outFile << "(";
        visitExpression(node->children[0]);
        outFile << " " << node->op << " ";
        visitExpression(node->children[1]);
        outFile << ")";
        break;
    case NODE_UNARY_OP:
        outFile << node->op << "(";
        visitExpression(node->children[0]);
        outFile << ")";
        break;
    case NODE_VARIABLE:
        visitVariable(node);
        break;
    case NODE_FUNCTION_CALL:
        visitFunctionCall(node);
        break;
    case NODE_ARRAY_ACCESS:
        visitArrayAccess(node);
        break;
    default:
        outFile << "/* UNKNOWN_EXPR */";
        break;
    }
}

void CodeGenerator::visitBinaryOp(ASTNode* node) {
    visitExpression(node->children[0]);
    outFile << " " << node->op << " ";
    visitExpression(node->children[1]);
}

void CodeGenerator::visitUnaryOp(ASTNode* node) {
    outFile << node->op << "(";
    visitExpression(node->children[0]);
    outFile << ")";
}