#pragma once
#include "Lox.h"
class Visitor;
class Expr {
public:
Expr(){}
class Binary;
class Grouping;
class Literal;
class Unary;
class Visitor;
virtual void* accept(Visitor *visitor) = 0;
};
class Expr::Visitor {
public:
virtual void* visitBinaryExpr(typename Expr::Binary *expr) = 0;
virtual void* visitGroupingExpr(typename Expr::Grouping *expr) = 0;
virtual void* visitLiteralExpr(typename Expr::Literal *expr) = 0;
virtual void* visitUnaryExpr(typename Expr::Unary *expr) = 0;
};

class Expr::Binary: public Expr {
public:
Binary(Expr *left, Token *op, Expr *right): left(left), op(op), right(right){}
Expr *left;
Token *op;
Expr *right;
void* accept(Visitor *visitor) override {
return visitor->visitBinaryExpr(this);
}
};
class Expr::Grouping: public Expr {
public:
Grouping(Expr *expression): expression(expression){}
Expr *expression;
void* accept(Visitor *visitor) override {
return visitor->visitGroupingExpr(this);
}
};
class Expr::Literal: public Expr {
public:
Literal(void* value): value(value){}
void* value;
void* accept(Visitor *visitor) override {
return visitor->visitLiteralExpr(this);
}
};
class Expr::Unary: public Expr {
public:
Unary(Token *op, Expr *right): op(op), right(right){}
Token *op;
Expr *right;
void* accept(Visitor *visitor) override {
return visitor->visitUnaryExpr(this);
}
};
