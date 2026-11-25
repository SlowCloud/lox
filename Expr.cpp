#include "main.cpp"
template <typename T>
class Visitor;
template <typename T>
class Expr {
public:
Expr(){}
class Binary;
class Grouping;
class Literal;
class Unary;
virtual T accept(Visitor<T> &visitor);
};
template <typename T>
class Expr<T>::Binary: Expr {
public:
Binary(Expr left, Token op, Expr right): left(left), op(op), right(right){}
Expr left;
Token op;
Expr right;
T accept(Visitor<T> &visitor) override {
return visitor.acceptBinaryExpr(*this);
}
};
template <typename T>
class Expr<T>::Grouping: Expr {
public:
Grouping(Expr expression): expression(expression){}
Expr expression;
T accept(Visitor<T> &visitor) override {
return visitor.acceptGroupingExpr(*this);
}
};
template <typename T>
class Expr<T>::Literal: Expr {
public:
Literal(void* value): value(value){}
void* value;
T accept(Visitor<T> &visitor) override {
return visitor.acceptLiteralExpr(*this);
}
};
template <typename T>
class Expr<T>::Unary: Expr {
public:
Unary(Token op, Expr right): op(op), right(right){}
Token op;
Expr right;
T accept(Visitor<T> &visitor) override {
return visitor.acceptUnaryExpr(*this);
}
};
template <typename T>
class Visitor {
virtual T visitBinaryExpr(typename Expr<T>::Binary &binary);
virtual T visitGroupingExpr(typename Expr<T>::Grouping &grouping);
virtual T visitLiteralExpr(typename Expr<T>::Literal &literal);
virtual T visitUnaryExpr(typename Expr<T>::Unary &unary);
};

