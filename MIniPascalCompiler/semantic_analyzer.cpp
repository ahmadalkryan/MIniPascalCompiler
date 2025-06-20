#include "semantic_analyzer.h"
#include "ast.h"
#include "symbol_table.h"

#include <iostream>
#include <sstream>

SemanticAnalyzer::SemanticAnalyzer() : hasErrors(false) {}

bool SemanticAnalyzer::analyze(ASTNode* root) {
    if (!root) return false;

    symbolTable.enterScope("global");
    checkProgram(root);
    symbolTable.exitScope();

    return !hasErrors;
}

void SemanticAnalyzer::checkProgram(ASTNode* node) {
    if (!node) return;

    // «· Õﬁﬁ „‰ «·≈⁄·«‰«  «·⁄«„…
    if (node->children.size() > 0 && node->children[0]) {
        checkDeclarations(node->children[0]);
    }

    // «· Õﬁﬁ „‰ «· ⁄—Ì›«  «·›—⁄Ì… („À· «·œÊ«·)
    if (node->children.size() > 1 && node->children[1]) {
        ASTNode* subprogs = node->children[1];
        for (ASTNode* subprog : subprogs->children) {
            checkSubprogram(subprog);
        }
    }

    // «· Õﬁﬁ „‰ ﬂ ·… «·»—‰«„Ã «·—∆Ì”Ì…
    if (node->children.size() > 2 && node->children[2]) {
        checkStatements(node->children[2]);
    }
}

void SemanticAnalyzer::checkDeclarations(ASTNode* node) {
    if (!node || node->type != NODE_DECLARATIONS) return;

    for (ASTNode* decl : node->children) {
        if (decl->type == NODE_DECLARATIONS) {
            Symbol symbol;
            ASTNode* ids = decl->children[0];       // ﬁ«∆„… «·„ €Ì—« 
            ASTNode* typeNode = decl->children[1];   // ‰Ê⁄ «·»Ì«‰« 

            TypeInfo typeInfo;

            if (typeNode->type == NODE_ARRAY_TYPE) {
                typeInfo.baseType = DataType::ARRAY;
                typeInfo.arrayStart = static_cast<int>(typeNode->int_val);
                typeInfo.arrayEnd = static_cast<int>(typeNode->real_val);

                ASTNode* baseTypeNode = typeNode->children[0];
                if (baseTypeNode->name == "integer")
                    typeInfo.elementType = DataType::INTEGER;
                else if (baseTypeNode->name == "real")
                    typeInfo.elementType = DataType::REAL;
                else if (baseTypeNode->name == "boolean")
                    typeInfo.elementType = DataType::BOOLEAN;
            }
            else if (typeNode->type == NODE_TYPE) {
                if (typeNode->name == "integer")
                    typeInfo.baseType = DataType::INTEGER;
                else if (typeNode->name == "real")
                    typeInfo.baseType = DataType::REAL;
                else if (typeNode->name == "boolean")
                    typeInfo.baseType = DataType::BOOLEAN;
                else
                    typeInfo.baseType = DataType::UNKNOWN;
            }

            // ≈÷«›… ﬂ· „ €Ì— ›Ì «·ﬁ«∆„…
            for (ASTNode* idNode : ids->children) {
                symbol.name = idNode->name;
                symbol.kind = SymbolKind::VARIABLE;
                symbol.typeInfo = typeInfo;

                if (!symbolTable.addSymbol(symbol)) {
                    std::cerr << "Semantic error: Redeclaration of '" << symbol.name << "'\n";
                    hasErrors = true;
                }
            }
        }
    }
}

void SemanticAnalyzer::checkSubprogram(ASTNode* node) {
    if (!node || node->type != NODE_SUBPROGRAM) return;

    ASTNode* head = node->children[0]; // —√” «·œ«·… √Ê «·≈Ã—«¡
    ASTNode* body = node->children[1]; // Ã”„ «·œ«·…

    Symbol subprogSymbol;
    if (head->type == NODE_FUNCTION_HEAD) {
        subprogSymbol.kind = SymbolKind::FUNCTION;
        subprogSymbol.name = head->name;

        //  ÕœÌœ ‰Ê⁄ «·ﬁÌ„ «·⁄«∆œ…
        if (head->children.size() >= 2) {
            ASTNode* returnTypeNode = head->children[1];
            if (returnTypeNode->type == NODE_TYPE) {
                if (returnTypeNode->name == "integer")
                    subprogSymbol.typeInfo.baseType = DataType::INTEGER;
                else if (returnTypeNode->name == "real")
                    subprogSymbol.typeInfo.baseType = DataType::REAL;
                else if (returnTypeNode->name == "boolean")
                    subprogSymbol.typeInfo.baseType = DataType::BOOLEAN;
                else
                    subprogSymbol.typeInfo.baseType = DataType::UNKNOWN;
            }
        }
    }
    else if (head->type == NODE_PROCEDURE_HEAD) {
        subprogSymbol.kind = SymbolKind::PROCEDURE;
        subprogSymbol.name = head->name;
        subprogSymbol.typeInfo.baseType = DataType::UNKNOWN;
    }

    // ≈÷«›… «·œ«·… ≈·Ï ÃœÊ· «·—„Ê“
    if (!symbolTable.addSymbol(subprogSymbol)) {
        std::cerr << "Semantic error: Redeclaration of subprogram '" << subprogSymbol.name << "'\n";
        hasErrors = true;
    }

    // œŒÊ· ‰ÿ«ﬁ ÃœÌœ
    symbolTable.enterScope(subprogSymbol.name);

    // ≈÷«›… «·„⁄«„·«  ≈‰ ÊÃœ 
    if (head->children.size() >= 1 && head->children[0]) {
        ASTNode* params = head->children[0];
        for (ASTNode* paramList : params->children) {
            ASTNode* ids = paramList->children[0];
            ASTNode* type = paramList->children[1];

            TypeInfo paramType;
            if (type->type == NODE_TYPE) {
                if (type->name == "integer")
                    paramType.baseType = DataType::INTEGER;
                else if (type->name == "real")
                    paramType.baseType = DataType::REAL;
                else if (type->name == "boolean")
                    paramType.baseType = DataType::BOOLEAN;
                else
                    paramType.baseType = DataType::UNKNOWN;
            }

            for (ASTNode* idNode : ids->children) {
                Symbol paramSymbol;
                paramSymbol.name = idNode->name;
                paramSymbol.kind = SymbolKind::PARAMETER;
                paramSymbol.typeInfo = paramType;
                subprogSymbol.paramTypes.push_back(paramType);

                if (!symbolTable.addSymbol(paramSymbol)) {
                    std::cerr << "Semantic error: Duplicate parameter '" << paramSymbol.name
                        << "' in subprogram '" << subprogSymbol.name << "'\n";
                    hasErrors = true;
                }
            }
        }
    }

    // «· Õﬁﬁ „‰  ⁄·Ì„«  «·Ã”„ «·„Õ·Ì
    if (body && body->children.size() >= 2) {
        checkDeclarations(body->children[0]); // «·≈⁄·«‰«  «·„Õ·Ì…
        checkStatements(body->children[1]);   // «· ⁄·Ì„« 
    }

    symbolTable.exitScope();
}

TypeInfo SemanticAnalyzer::checkExpression(ASTNode* node) {
    if (!node) return TypeInfo(DataType::UNKNOWN);

    switch (node->type) {
    case NODE_INT_NUM:
        return TypeInfo(DataType::INTEGER);
    case NODE_REAL_NUM:
        return TypeInfo(DataType::REAL);
    case NODE_BOOLEAN:
        return TypeInfo(DataType::BOOLEAN);
    case NODE_VARIABLE: {
        Symbol* sym = symbolTable.findSymbol(node->name);
        if (!sym) {
            std::cerr << "Semantic error: Undeclared identifier '" << node->name << "'\n";
            hasErrors = true;
            return TypeInfo(DataType::UNKNOWN);
        }
        return sym->typeInfo;
    }
    case NODE_ARRAY_ACCESS: {
        Symbol* sym = symbolTable.findSymbol(node->children[0]->name);
        if (!sym || sym->typeInfo.baseType != DataType::ARRAY) {
            std::cerr << "Semantic error: Undeclared array '" << node->children[0]->name << "'\n";
            hasErrors = true;
            return TypeInfo(DataType::UNKNOWN);
        }

        TypeInfo indexType = checkExpression(node->children[1]);
        if (indexType.baseType != DataType::INTEGER) {
            std::cerr << "Semantic error: Array index must be integer\n";
            hasErrors = true;
        }

        return TypeInfo(sym->typeInfo.elementType); // ‰Ê⁄ «·⁄‰«’— œ«Œ· «·„’›Ê›…
    }
    case NODE_BINARY_OP: {
        TypeInfo left = checkExpression(node->children[0]);
        TypeInfo right = checkExpression(node->children[1]);

        if (left.baseType == DataType::UNKNOWN || right.baseType == DataType::UNKNOWN)
            return TypeInfo(DataType::UNKNOWN);

        if (left != right) {
            std::cerr << "Semantic error: Type mismatch in binary operation\n";
            hasErrors = true;
        }

        return left;
    }
    case NODE_UNARY_OP: {
        TypeInfo expr = checkExpression(node->children[0]);
        if (expr.baseType == DataType::UNKNOWN)
            return TypeInfo(DataType::UNKNOWN);

        if (node->op == "not" && expr.baseType != DataType::BOOLEAN) {
            std::cerr << "Semantic error: NOT operator requires boolean operand\n";
            hasErrors = true;
        }
        else if ((node->op == "+" || node->op == "-") &&
            expr.baseType != DataType::INTEGER &&
            expr.baseType != DataType::REAL) {
            std::cerr << "Semantic error: Unary minus/plus requires numeric operand\n";
            hasErrors = true;
        }

        return expr;
    }
    case NODE_FUNCTION_CALL: {
        Symbol* sym = symbolTable.findSymbol(node->name);
        if (!sym || sym->kind != SymbolKind::FUNCTION) {
            std::cerr << "Semantic error: Undeclared function '" << node->name << "'\n";
            hasErrors = true;
            return TypeInfo(DataType::UNKNOWN);
        }

        if (node->children.size() > 0) {
            ASTNode* args = node->children[0];
            if (args->children.size() != sym->paramTypes.size()) {
                std::cerr << "Semantic error: Function '" << sym->name << "' expects "
                    << sym->paramTypes.size() << " arguments but got "
                    << args->children.size() << "\n";
                hasErrors = true;
            }
            else {
                for (size_t i = 0; i < args->children.size(); ++i) {
                    TypeInfo argType = checkExpression(args->children[i]);
                    if (argType != sym->paramTypes[i]) {
                        std::cerr << "Semantic error: Argument " << i + 1 << " of function '"
                            << sym->name << "' expects type " << sym->paramTypes[i].toString()
                            << ", got " << argType.toString() << "\n";
                        hasErrors = true;
                    }
                }
            }
        }

        return sym->typeInfo;
    }
    default:
        return TypeInfo(DataType::UNKNOWN);
    }
}

void SemanticAnalyzer::checkStatements(ASTNode* node) {
    if (!node) return;

    for (ASTNode* stmt : node->children) {
        switch (stmt->type) {
        case NODE_ASSIGNMENT: {
            ASTNode* var = stmt->left;
            ASTNode* expr = stmt->right;

            TypeInfo varType, exprType;

            if (var->type == NODE_VARIABLE) {
                Symbol* sym = symbolTable.findSymbol(var->name);
                if (!sym) {
                    std::cerr << "Semantic error: Undeclared variable '" << var->name << "'\n";
                    hasErrors = true;
                    break;
                }
                varType = sym->typeInfo;
            }
            else if (var->type == NODE_ARRAY_ACCESS) {
                ASTNode* arrName = var->children[0];
                Symbol* sym = symbolTable.findSymbol(arrName->name);
                if (!sym || sym->typeInfo.baseType != DataType::ARRAY) {
                    std::cerr << "Semantic error: Undeclared array '" << arrName->name << "'\n";
                    hasErrors = true;
                    break;
                }
                varType = TypeInfo(sym->typeInfo.elementType);
            }

            exprType = checkExpression(expr);

            if (varType != exprType) {
                std::cerr << "Semantic error: Type mismatch in assignment\n";
                hasErrors = true;
            }

            break;
        }
        case NODE_IF: {
            checkExpression(stmt->left); // «·‘—ÿ
            checkStatements(stmt->right); // «·›—⁄ ≈–« ﬂ«‰ ’ÕÌÕ«
            if (stmt->children.size() > 0)
                checkStatements(stmt->children[0]->right); // «·›—⁄ ≈–« ﬂ«‰ Œ«ÿ∆«
            break;
        }
        case NODE_WHILE: {
            checkExpression(stmt->left); // «·‘—ÿ
            checkStatements(stmt->right); // Ã”„ «·Õ·ﬁ…
            break;
        }
        case NODE_PROCEDURE_CALL:
        case NODE_FUNCTION_CALL: {
            Symbol* sym = symbolTable.findSymbol(stmt->name);
            if (!sym || (sym->kind != SymbolKind::FUNCTION && sym->kind != SymbolKind::PROCEDURE)) {
                std::cerr << "Semantic error: Undeclared subprogram '" << stmt->name << "'\n";
                hasErrors = true;
                break;
            }

            if (stmt->children.size() > 0) {
                ASTNode* args = stmt->children[0];
                if (args->children.size() != sym->paramTypes.size()) {
                    std::cerr << "Semantic error: Subprogram '" << sym->name << "' expects "
                        << sym->paramTypes.size() << " arguments but got "
                        << args->children.size() << "\n";
                    hasErrors = true;
                }
                else {
                    for (size_t i = 0; i < args->children.size(); ++i) {
                        TypeInfo argType = checkExpression(args->children[i]);
                        if (argType != sym->paramTypes[i]) {
                            std::cerr << "Semantic error: Argument " << i + 1 << " of subprogram '"
                                << sym->name << "' expects type " << sym->paramTypes[i].toString()
                                << ", got " << argType.toString() << "\n";
                            hasErrors = true;
                        }
                    }
                }
            }
            break;
        }
        default:
            break;
        }
    }
}

bool SemanticAnalyzer::hasSemanticErrors() const {
    return hasErrors;
}