#include "Scanner.cpp"
class Visitor;
class Expr {
public:
Expr(){}
class Binary;
class Grouping;
class Literal;
class Unary;
virtual void* accept(Visitor *visitor);
};
class Visitor {
public:
virtual void* visitBinaryExpr(typename Expr::Binary *expr);
virtual void* visitGroupingExpr(typename Expr::Grouping *expr);
virtual void* visitLiteralExpr(typename Expr::Literal *expr);
virtual void* visitUnaryExpr(typename Expr::Unary *expr);
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
