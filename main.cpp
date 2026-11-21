#include <cstddef>
#include <ios>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

namespace Lox {
    static void report(size_t line, string where, string message) {
        cerr << "[line " << line << "] Error" << where << ": " << message << endl;
    }

    void error(size_t line, std::string message) {
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

static map<string, TokenType> keywords = {
  {"and",    AND},
  {"class",  CLASS},
  {"else",   ELSE},
  {"false",  FALSE},
  {"for",    FOR},
  {"fun",    FUN},
  {"if",     IF},
  {"nil",    NIL},
  {"or",     OR},
  {"print",  PRINT},
  {"return", RETURN},
  {"super",  SUPER},
  {"this",   THIS},
  {"true",   TRUE},
  {"var",    VAR},
  {"while",  WHILE},
};

class Token {
    TokenType type;
    std::string lexeme;
    void* literal;
    size_t line;

public:
    Token(TokenType type, std::string lexeme, void* literal, size_t line):
        type(type), lexeme(lexeme), literal(literal), line(line) {}

    string toString() const {
        string res;
        stringstream ss(res);
        ss << "type: " << type << ", lexeme: " << lexeme << ", line: " << line;
        string now = ss.str();
        return now;
    }
};

class Scanner {
    const string& src;
    vector<Token> tokens;

    size_t start, current, line;

    string* substr(int s, int e) {
        std::string* v = new std::string();
        *v = src.substr(s, e - s);
        return v;
    }

    bool isAtEnd() {
        return current >= src.size();
    }

    char advance() {
        return src.at(current++);
    }

    void addToken(TokenType type) {
        addToken(type, nullptr);
    }

    void addToken(TokenType type, void* literal) {
        // 성능에 안 좋겠지만 일단 진행
        std::string text = src.substr(start, current - start);
        tokens.push_back(Token(type, text, literal, line));
    }

    bool match(char expected) {
        if(isAtEnd()) return false;
        if(src.at(current) != expected) return false;
        current++;
        return true;
    }

    char peek() {
        if(isAtEnd()) return '\0';
        return src.at(current);
    }

    char peekNext() {
        if(current + 1 >= src.size()) return '\0';
        return src.at(current + 1);
    }

    void string() {
        while(peek() != '"' && !isAtEnd()) {
            if(peek() == '\n') line++;
            advance();
        }

        if(isAtEnd()) {
            Lox::error(line, "Unterminated string.");
            return;
        }

        advance();
        std::string* val = substr(start + 1, current - start - 1);
        addToken(STRING, val);
    }

    bool isDigit(char c) {
        return '0' <= c && c <= '9';
    }

    void number() {
        while(isDigit(peek())) advance();
        // 소수점 아래
        if(peek() == '.' && isDigit(peekNext())) {
            advance();
            while(isDigit(peek())) advance();
        }
        double* v = new double;
        std::string now = src.substr(start, current - start);
        *v = stod(now);
        addToken(NUMBER, v);
    }

    bool isAlpha(char c) {
        return isAlpha(c) || c == '_';
    }

    bool isAlphaNumeric(char c) {
        return isAlpha(c) || isDigit(c);
    }

    void identifier() {
        while(isAlphaNumeric(peek())) advance();
        std::string* text = substr(start, current);
        TokenType type = keywords.at(*text);
        if(type == NULL) type = IDENTIFIER;
        addToken(type);
    }

    void scanToken() {
        char c = advance();
        switch (c) {
          case '(': addToken(LEFT_PAREN); break;
          case ')': addToken(RIGHT_PAREN); break;
          case '{': addToken(LEFT_BRACE); break;
          case '}': addToken(RIGHT_BRACE); break;
          case ',': addToken(COMMA); break;
          case '.': addToken(DOT); break;
          case '-': addToken(MINUS); break;
          case '+': addToken(PLUS); break;
          case ';': addToken(SEMICOLON); break;
          case '*': addToken(STAR); break;
          case '!':
            addToken(match('=') ? BANG_EQUAL : BANG);
            break;
          case '=':
            addToken(match('=') ? EQUAL_EQUAL : EQUAL);
            break;
          case '<':
            addToken(match('=') ? LESS_EQUAL : LESS);
            break;
          case '>':
            addToken(match('=') ? GREATER_EQUAL : GREATER);
            break;
          case '/':
            if (match('/')) {
                // 주석 처리
                while (peek() != '\n' && !isAtEnd()) advance();
            } else {
              addToken(SLASH);
            }
            break;
          case '"': string(); break;

          case '\n': line++;
          case ' ':
          case '\r':
          case '\t':
            break;

          default:
              if(isDigit(c)) {
                  number();
              } else if(isAlpha(c)) {
                  identifier();
              } else {
                  Lox::error(line, "Unexpected character.");
              }
              break;
        }
    }

public:
    Scanner(std::string& src): src(src), tokens() {
        start = 0;
        current = 0;
        line = 1;
    }

    vector<Token>&& scanTokens() {
        while(!isAtEnd()) {
            start = current;
            scanToken();
        }
        tokens.push_back(Token(END_OF_FILE, "", nullptr, line));
        return std::move(tokens);
    }
};

void run(std::string line) {
    Scanner scanner(line);

    vector<Token> tokens = scanner.scanTokens();

    for(const Token& token : tokens) {
        cout << token.toString() << endl;
    }
}

// void runFile(std::string fileName) {
//
// }
void runPrompt() {
    string line;
    for(;;) {
        cout << "> ";
        getline(cin, line);
        run(line);
    }
}

int main(int argc, char** argv) {
    std::ios::sync_with_stdio(0);

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
