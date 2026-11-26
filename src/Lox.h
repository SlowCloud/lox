#include <iostream>
#include <string>
#include <sstream>
#include <vector>

namespace Lox {
    static void report(size_t line, std::string where, std::string message) {
        std::cerr << "[line " << line << "] Error" << where << ": " << message << std::endl;
    }

    static void error(size_t line, std::string message) {
        Lox::report(line, "", message);
    }
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
};

inline std::string Token::toString() const {
    std::string res;
    std::stringstream ss(res);
    ss << "type: " << type << ", lexeme: " << lexeme << ", line: " << line;
    std::string now = ss.str();
    return now;
}
