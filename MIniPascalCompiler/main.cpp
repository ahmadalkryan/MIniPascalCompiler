#include <iostream>
#include <fstream>
#include "ast.h"
#include "semantic_analyzer.h"
#include "symbol_table.h"

extern int yyparse();
extern FILE *yyin;
extern ASTNode* root;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <hello.pas.pas>" << std::endl;
        return 1;
    }

    // Open input file
    yyin = fopen(argv[1], "r");
    if (!yyin) {
        std::cerr << "Error: Cannot open file " << argv[1] << std::endl;
        return 1;
    }

    // Parse the input file
    std::cout << "Parsing " << argv[1] << "..." << std::endl;
    if (yyparse() != 0) {
        std::cerr << "Error: Parsing failed" << std::endl;
        fclose(yyin);
        return 1;
    }
    fclose(yyin);

    // Print AST if parsing succeeded
    if (root) {
        std::cout << "\nAbstract Syntax Tree (AST):" << std::endl;
        printAST(root);
        
        // Semantic analysis
        std::cout << "\nPerforming semantic analysis..." << std::endl;
        SemanticAnalyzer semanticAnalyzer;
        if (!semanticAnalyzer.analyze(root)) {
            std::cerr << "Error: Semantic analysis failed" << std::endl;
            freeAST(root);
            return 1;
        }
        std::cout << "Semantic analysis completed successfully!" << std::endl;
        
        // Free AST memory
        freeAST(root);
    } else {
        std::cerr << "Error: No AST generated" << std::endl;
        return 1;
    }

    return 0;
}