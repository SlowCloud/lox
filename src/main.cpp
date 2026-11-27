#include <ios>
#include <iostream>
#include <string>

#include "Lox.h"

using namespace std;

void run(std::string&& line) {
    Scanner scanner(std::move(line));
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
        run(std::move(line));
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
