#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cctype>


enum TokenType {NUMBER, VARIABLE, ADDITION, SUBTRACTION, MODULO, MULTIPLICATION,
    DIVISION, SEMICOLON, ASSIGNMENT, LEFT_BRACE, RIGHT_BRACE};


std::ostream& operator<<(std::ostream& os, TokenType type)
{
    switch(type)
    {
        case NUMBER : os << "number"; break;
        case VARIABLE : os << "variable"; break;
        case ADDITION : os << "addition"; break;
        case SUBTRACTION : os << "subtraction"; break;
        case MODULO : os << "modulo"; break;
        case MULTIPLICATION : os << "multiplication"; break;
        case DIVISION : os << "division"; break;
        case SEMICOLON : os << "semicolon"; break;
        case ASSIGNMENT : os << "assignment"; break;
        case LEFT_BRACE : os << "left brace"; break;
        case RIGHT_BRACE : os << "right brace"; break;
    }
    return os;
}

struct Token {
    private:
        std::string value;
        TokenType type;

    public:
        Token(std::string value, TokenType type) {
            this->value = value;
            this->type = type;
        };

        friend std::ostream& operator<<(std::ostream& os, const Token& token) {
            os << token.value << " - " << token.type;
            return os;
        }
};


std::vector<Token> getTokens (std::string& input) {
    unsigned int inputSize = input.size();
    std::vector<Token> tokens;

    for (unsigned int i = 0; i < inputSize; i++) {
        char currentChar = input[i];
        if (currentChar == '=') tokens.push_back({std::string(1, currentChar), TokenType::ASSIGNMENT});
        else if (currentChar == '+') tokens.push_back({std::string(1, currentChar), TokenType::ADDITION});
        else if (currentChar == '-') tokens.push_back({std::string(1, currentChar), TokenType::SUBTRACTION});
        else if (currentChar == '%') tokens.push_back({std::string(1, currentChar), TokenType::MODULO});
        else if (currentChar == '*') tokens.push_back({std::string(1, currentChar), TokenType::MULTIPLICATION});
        else if (currentChar == '/') tokens.push_back({std::string(1, currentChar), TokenType::DIVISION});
        else if (currentChar == '(') tokens.push_back({std::string(1, currentChar), TokenType::RIGHT_BRACE});
        else if (currentChar == ')') tokens.push_back({std::string(1, currentChar), TokenType::LEFT_BRACE});
        else if (currentChar == ';') tokens.push_back({std::string(1, currentChar), TokenType::SEMICOLON});
        else if (isalpha(currentChar)) tokens.push_back({std::string(1, currentChar), TokenType::VARIABLE});
    }

    for (unsigned int t = 0; t < tokens.size(); t++) {
        std::cout << tokens[t] << std::endl;
    }

    return tokens;
}


int main() {
    std::ifstream input("input.txt");
    std::stringstream ss;
    ss << input.rdbuf();

    // std::cout << ss.str() << std::endl;
    std::string str = ss.str();
    getTokens(str);

    return 0;
}
