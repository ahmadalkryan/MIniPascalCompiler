#include "ast.h"
#include <iostream>
#include <iomanip>
#include <memory>

ASTNode* createProgramNode(const std::string& name, ASTNode* decls, ASTNode* subprogs, ASTNode* compound) {
    ASTNode* node = new ASTNode(NODE_PROGRAM);
    node->name = name;
    if (decls) node->children.push_back(decls);
    if (subprogs) node->children.push_back(subprogs);
    if (compound) node->children.push_back(compound);
    return node;
}

ASTNode* createDeclarationsNode(ASTNode* prev, ASTNode* ids, ASTNode* type) {
    ASTNode* node = new ASTNode(NODE_DECLARATIONS);
    if (prev) node->children.push_back(prev);
    if (ids) node->children.push_back(ids);
    if (type) node->children.push_back(type);
    return node;
}

ASTNode* createTypeNode(const std::string& type_name) {
    ASTNode* node = new ASTNode(NODE_TYPE);
    node->name = type_name;
    return node;
}

ASTNode* createArrayTypeNode(int start, int end, ASTNode* base_type) {
    ASTNode* node = new ASTNode(NODE_ARRAY_TYPE);
    node->int_val = start;
    node->real_val = end;
    if (base_type) node->children.push_back(base_type);
    return node;
}

ASTNode* createSubprogramDeclarationsNode(ASTNode* prev, ASTNode* subprog) {
    ASTNode* node = new ASTNode(NODE_SUBPROGRAM_DECLS);
    if (prev) node->children.push_back(prev);
    if (subprog) node->children.push_back(subprog);
    return node;
}

ASTNode* createSubprogramNode(ASTNode* head, ASTNode* body) {
    ASTNode* node = new ASTNode(NODE_SUBPROGRAM);
    if (head) node->children.push_back(head);
    if (body) node->children.push_back(body);
    return node;
}

ASTNode* createFunctionHeadNode(const std::string& name, ASTNode* params, ASTNode* return_type) {
    ASTNode* node = new ASTNode(NODE_FUNCTION_HEAD);
    node->name = name;
    if (params) node->children.push_back(params);
    if (return_type) node->children.push_back(return_type);
    return node;
}

ASTNode* createProcedureHeadNode(const std::string& name, ASTNode* params) {
    ASTNode* node = new ASTNode(NODE_PROCEDURE_HEAD);
    node->name = name;
    if (params) node->children.push_back(params);
    return node;
}

ASTNode* createParameterListNode(ASTNode* ids, ASTNode* type) {
    ASTNode* node = new ASTNode(NODE_PARAMETER_LIST);
    if (ids) node->children.push_back(ids);
    if (type) node->children.push_back(type);
    return node;
}

ASTNode* appendParameterListNode(ASTNode* prev, ASTNode* ids, ASTNode* type) {
    if (!prev) return createParameterListNode(ids, type);

    ASTNode* new_node = createParameterListNode(ids, type);
    prev->children.push_back(new_node);
    return prev;
}

ASTNode* createIdentifierListNode(const std::string& id) {
    ASTNode* node = new ASTNode(NODE_IDENTIFIER_LIST);
    node->name = id;
    return node;
}

ASTNode* appendIdentifierListNode(ASTNode* prev, const std::string& id) {
    if (!prev) return createIdentifierListNode(id);

    ASTNode* new_node = createIdentifierListNode(id);
    prev->children.push_back(new_node);
    return prev;
}

ASTNode* createCompoundStatementNode(ASTNode* stmts) {
    ASTNode* node = new ASTNode(NODE_COMPOUND_STMT);
    if (stmts) node->children.push_back(stmts);
    return node;
}

ASTNode* createAssignmentNode(ASTNode* var, ASTNode* expr) {
    ASTNode* node = new ASTNode(NODE_ASSIGNMENT);
    node->left = var;
    node->right = expr;
    return node;
}

ASTNode* createIfNode(ASTNode* cond, ASTNode* then_stmt, ASTNode* else_stmt) {
    ASTNode* node = new ASTNode(NODE_IF);
    node->left = cond;
    node->right = then_stmt;
    if (else_stmt) {
        ASTNode* else_node = new ASTNode(NODE_IF);
        else_node->right = else_stmt;
        node->children.push_back(else_node);
    }
    return node;
}

ASTNode* createWhileNode(ASTNode* cond, ASTNode* body) {
    ASTNode* node = new ASTNode(NODE_WHILE);
    node->left = cond;
    node->right = body;
    return node;
}

ASTNode* createProcedureCallNode(const std::string& name, ASTNode* params) {
    ASTNode* node = new ASTNode(NODE_PROCEDURE_CALL);
    node->name = name;
    if (params) node->children.push_back(params);
    return node;
}

ASTNode* createFunctionCallNode(const std::string& name, ASTNode* params) {
    ASTNode* node = new ASTNode(NODE_FUNCTION_CALL);
    node->name = name;
    if (params) node->children.push_back(params);
    return node;
}

ASTNode* createVariableNode(const std::string& name, ASTNode* index) {
    ASTNode* node = new ASTNode(NODE_VARIABLE);
    node->name = name;
    if (index) node->children.push_back(index);
    return node;
}

ASTNode* createArrayAccessNode(const std::string& name, ASTNode* index) {
    ASTNode* node = new ASTNode(NODE_ARRAY_ACCESS);
    node->name = name;
    if (index) node->children.push_back(index);
    return node;
}

ASTNode* createExpressionListNode(ASTNode* expr) {
    ASTNode* node = new ASTNode(NODE_EXPRESSION_LIST);
    if (expr) node->children.push_back(expr);
    return node;
}

ASTNode* appendExpressionListNode(ASTNode* prev, ASTNode* expr) {
    if (!prev) return createExpressionListNode(expr);

    if (expr) prev->children.push_back(expr);
    return prev;
}

ASTNode* createIntNumNode(int val) {
    ASTNode* node = new ASTNode(NODE_INT_NUM);
    node->int_val = val;
    return node;
}

ASTNode* createRealNumNode(double val) {
    ASTNode* node = new ASTNode(NODE_REAL_NUM);
    node->real_val = val;
    return node;
}

ASTNode* createBooleanNode(bool val) {
    ASTNode* node = new ASTNode(NODE_BOOLEAN);
    node->bool_val = val;
    return node;
}

ASTNode* createBinaryOpNode(ASTNode* left, ASTNode* right, const std::string& op) {
    ASTNode* node = new ASTNode(NODE_BINARY_OP);
    node->left = left;
    node->right = right;
    node->op = op;
    return node;
}

ASTNode* createUnaryOpNode(ASTNode* expr, const std::string& op) {
    ASTNode* node = new ASTNode(NODE_UNARY_OP);
    node->left = expr;
    node->op = op;
    return node;
}

ASTNode* appendStatementNode(ASTNode* prev, ASTNode* stmt) {
    if (!prev) return stmt;

    ASTNode* node = new ASTNode(NODE_STATEMENT_LIST);
    node->children.push_back(prev);
    node->children.push_back(stmt);
    return node;
}

void printAST(ASTNode* node, int indent) {
    if (!node) return;

    std::cout << std::setw(indent) << "";

    switch (node->type) {
    case NODE_PROGRAM:
        std::cout << "Program: " << node->name << std::endl;
        break;
    case NODE_DECLARATIONS:
        std::cout << "Declarations" << std::endl;
        break;
    case NODE_TYPE:
        std::cout << "Type: " << node->name << std::endl;
        break;
    case NODE_ARRAY_TYPE:
        std::cout << "Array[" << node->int_val << ".." << node->real_val << "]" << std::endl;
        break;
    case NODE_SUBPROGRAM_DECLS:
        std::cout << "Subprogram Declarations" << std::endl;
        break;
    case NODE_SUBPROGRAM:
        std::cout << "Subprogram" << std::endl;
        break;
    case NODE_FUNCTION_HEAD:
        std::cout << "Function: " << node->name << std::endl;
        break;
    case NODE_PROCEDURE_HEAD:
        std::cout << "Procedure: " << node->name << std::endl;
        break;
    case NODE_PARAMETER_LIST:
        std::cout << "Parameter List" << std::endl;
        break;
    case NODE_IDENTIFIER_LIST:
        std::cout << "Identifier: " << node->name << std::endl;
        break;
    case NODE_COMPOUND_STMT:
        std::cout << "Compound Statement" << std::endl;
        break;
    case NODE_ASSIGNMENT:
        std::cout << "Assignment" << std::endl;
        break;
    case NODE_IF:
        std::cout << "If Statement" << std::endl;
        break;
    case NODE_WHILE:
        std::cout << "While Loop" << std::endl;
        break;
    case NODE_PROCEDURE_CALL:
        std::cout << "Procedure Call: " << node->name << std::endl;
        break;
    case NODE_FUNCTION_CALL:
        std::cout << "Function Call: " << node->name << std::endl;
        break;
    case NODE_VARIABLE:
        std::cout << "Variable: " << node->name << std::endl;
        break;
    case NODE_ARRAY_ACCESS:
        std::cout << "Array Access: " << node->name << "[]" << std::endl;
        break;
    case NODE_EXPRESSION_LIST:
        std::cout << "Expression List" << std::endl;
        break;
    case NODE_INT_NUM:
        std::cout << "Integer: " << node->int_val << std::endl;
        break;
    case NODE_REAL_NUM:
        std::cout << "Real: " << node->real_val << std::endl;
        break;
    case NODE_BOOLEAN:
        std::cout << "Boolean: " << (node->bool_val ? "true" : "false") << std::endl;
        break;
    case NODE_BINARY_OP:
        std::cout << "Binary Op: " << node->op << std::endl;
        break;
    case NODE_UNARY_OP:
        std::cout << "Unary Op: " << node->op << std::endl;
        break;
    default:
        std::cout << "Unknown node type" << std::endl;
    }

    if (node->left) printAST(node->left, indent + 4);
    if (node->right) printAST(node->right, indent + 4);
    for (ASTNode* child : node->children) {
        printAST(child, indent + 4);
    }
}

void freeAST(ASTNode* node) {
    if (!node) return;
    delete node; // Destructor will handle children recursively
}