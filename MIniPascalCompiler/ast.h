#ifndef AST_H
#define AST_H

#include <string>
#include <vector>

enum NodeType {
    NODE_PROGRAM,
    NODE_DECLARATIONS,
    NODE_TYPE,
    NODE_ARRAY_TYPE,
    NODE_SUBPROGRAM_DECLS,
    NODE_SUBPROGRAM,
    NODE_FUNCTION_HEAD,
    NODE_PROCEDURE_HEAD,
    NODE_PARAMETER_LIST,
    NODE_IDENTIFIER_LIST,
    NODE_COMPOUND_STMT,
    NODE_STATEMENT_LIST,
    NODE_ASSIGNMENT,
    NODE_IF,
    NODE_WHILE,
    NODE_PROCEDURE_CALL,
    NODE_FUNCTION_CALL,
    NODE_VARIABLE,
    NODE_ARRAY_ACCESS,
    NODE_EXPRESSION_LIST,
    NODE_INT_NUM,
    NODE_REAL_NUM,
    NODE_BOOLEAN,
    NODE_BINARY_OP,
    NODE_UNARY_OP
};

struct ASTNode {
    NodeType type;
    std::string name;
    int int_val;
    double real_val;
    bool bool_val;
    std::string op;
    ASTNode* left;
    ASTNode* right;
    std::vector<ASTNode*> children;

    ASTNode(NodeType t) : type(t), left(nullptr), right(nullptr) {}
};

// Function declarations for creating AST nodes
ASTNode* createProgramNode(const char* name, ASTNode* decls, ASTNode* subprogs, ASTNode* compound);
ASTNode* createDeclarationsNode(ASTNode* prev, ASTNode* ids, ASTNode* type);
ASTNode* createTypeNode(const char* type_name);
ASTNode* createArrayTypeNode(int start, int end, ASTNode* base_type);
ASTNode* createSubprogramDeclarationsNode(ASTNode* prev, ASTNode* subprog);
ASTNode* createSubprogramNode(ASTNode* head, ASTNode* body);
ASTNode* createFunctionHeadNode(const char* name, ASTNode* params, ASTNode* return_type);
ASTNode* createProcedureHeadNode(const char* name, ASTNode* params);
ASTNode* createParameterListNode(ASTNode* ids, ASTNode* type);
ASTNode* appendParameterListNode(ASTNode* prev, ASTNode* ids, ASTNode* type);
ASTNode* createIdentifierListNode(const char* id);
ASTNode* appendIdentifierListNode(ASTNode* prev, const char* id);
ASTNode* createCompoundStatementNode(ASTNode* stmts);
ASTNode* createAssignmentNode(ASTNode* var, ASTNode* expr);
ASTNode* createIfNode(ASTNode* cond, ASTNode* then_stmt, ASTNode* else_stmt);
ASTNode* createWhileNode(ASTNode* cond, ASTNode* body);
ASTNode* createProcedureCallNode(const char* name, ASTNode* params);
ASTNode* createFunctionCallNode(const char* name, ASTNode* params);
ASTNode* createVariableNode(const char* name, ASTNode* index);
ASTNode* createArrayAccessNode(const char* name, ASTNode* index);
ASTNode* createExpressionListNode(ASTNode* expr);
ASTNode* appendExpressionListNode(ASTNode* prev, ASTNode* expr);
ASTNode* createIntNumNode(int val);
ASTNode* createRealNumNode(double val);
ASTNode* createBooleanNode(bool val);
ASTNode* createBinaryOpNode(ASTNode* left, ASTNode* right, const char* op);
ASTNode* createUnaryOpNode(ASTNode* expr, const char* op);
ASTNode* appendStatementNode(ASTNode* prev, ASTNode* stmt);

void printAST(ASTNode* node, int indent = 0);
void freeAST(ASTNode* node);

#endif // AST_H