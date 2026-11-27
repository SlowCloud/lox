#include <ios>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Lox.h"
#include "Expr.h"

class AstPrinter : public Expr::Visitor {
    std::string* parenthesize(std::string name, std::vector<Expr*> exprs) {
        std::stringstream ss;
        ss << '(' << name;
        for(Expr* const expr : exprs) {
            ss << ' ' << *((std::string*)expr->accept(this));
        }
        ss << ')';
        return new std::string(ss.str());
    }

    public:
        std::string print(Expr* expr) {
            return *(std::string*)expr->accept(this);
        }
        void* visitBinaryExpr(typename Expr::Binary *expr) override {
            return (void*) parenthesize(expr->op->lexeme, {expr->left, expr->right});
        };
        void* visitGroupingExpr(typename Expr::Grouping *expr)override {
            return (void*) parenthesize("group", {expr->expression});
        }
        void* visitLiteralExpr(typename Expr::Literal *expr)override {
            if(expr->value == nullptr) return (void*)(new std::string("nil"));
            return (void*) new std::string(std::to_string(*(long double*)expr->value));
        }
        void* visitUnaryExpr(typename Expr::Unary *expr)override {
            return parenthesize(expr->op->lexeme, {expr->right});
        }
};

void run(std::string&& line) {
    Scanner scanner(std::move(line));
    std::vector<Token> tokens = scanner.scanTokens();
    for(const Token& token : tokens) {
        std::cout << token.toString() << std::endl;
    }
}

// void runFile(std::string fileName) {
//
// }
void runPrompt() {
    std::string line;
    for(;;) {
        std::cout << "> ";
        getline(std::cin, line);
        run(std::move(line));
    }
}

int main(int argc, char** argv) {
    std::ios::sync_with_stdio(0);

    /*
    Expr* expression = new Expr::Binary(
        new Expr::Unary(
            new Token(MINUS, "-", nullptr, 1),
            new Expr::Literal((void*) new long double(123))),
        new Token(STAR, "*", nullptr, 1),
        new Expr::Grouping(
            new Expr::Literal((void*) new long double(45.67))));

    std::cout << AstPrinter().print(expression);
    */

    if(argc > 2) {
        std::cout << "Usage: cpplox [script]";
    }
    else if(argc == 2) {
        // runFile(argv[1]);
    }
    else {
        runPrompt();
    }
}
