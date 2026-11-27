#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

namespace Lox {
    void report(size_t line, std::string where, std::string message);
    void error(size_t line, std::string message);
}

enum TokenType {
  // Single-character tokens.
  LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
  COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

  // One or two character tokens.
  BANG, BANG_EQUAL,
  EQUAL, EQUAL_EQUAL,
  GREATER, GREATER_EQUAL,
  LESS, LESS_EQUAL,

  // Literals.
  IDENTIFIER, STRING, NUMBER,

  // Keywords.
  AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
  PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

  END_OF_FILE
};

struct Token {
    TokenType type;
    std::string lexeme;
    void* literal;
    size_t line;
    std::string toString() const;
    Token(TokenType type, std::string lexeme, void* literal, size_t line) : type(type), lexeme(std::move(lexeme)), literal(literal), line(line) {}
};

inline std::string Token::toString() const {
    std::string res;
    std::stringstream ss(res);
    ss << "type: " << type << ", lexeme: " << lexeme << ", line: " << line;
    std::string now = ss.str();
    return now;
}

class Scanner {
    private:
    const std::string& src;
    std::vector<Token> tokens;
    size_t start, current, line;

    std::string *substr(int s, int e);
    bool isAtEnd();
    char advance();
    void addToken(TokenType type);
    void addToken(TokenType type, void *literal);
    bool match(char expected);
    char peek();
    char peekNext();
    void String();
    bool isDigit(char c);
    void number();
    bool isAlpha(char c);
    bool isAlphaNumeric(char c);
    void identifier();
    void scanToken();
  public:
    Scanner(std::string &src);
    std::vector<Token> &&scanTokens();
};
