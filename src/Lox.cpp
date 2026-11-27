#include "Lox.h"


namespace Lox {
    void report(size_t line, std::string where, std::string message) {
        std::cerr << "[line " << line << "] Error" << where << ": " << message << std::endl;
    }

    void error(size_t line, std::string message) {
        Lox::report(line, "", message);
    }
}
