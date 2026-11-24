#include "main.cpp"
class Expr {
public:
Expr(){}
class Binary;
class Grouping;
class Literal;
class Unary;
};
class Binary: Expr {
public:
Binary(Expr left, Token op, Expr right): left(left), op(op), right(right){}
Expr left;
Token op;
Expr right;
};
class Grouping: Expr {
public:
Grouping(Expr expression): expression(expression){}
Expr expression;
};
class Literal: Expr {
public:
Literal(void* value): value(value){}
void* value;
};
class Unary: Expr {
public:
Unary(Token op, Expr right): op(op), right(right){}
Token op;
Expr right;
};
