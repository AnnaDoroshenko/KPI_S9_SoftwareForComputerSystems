#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cctype>
#include <stdlib.h>


enum TokenType {INTEGER, FLOAT, VARIABLE, ADDITION, SUBTRACTION, MODULO, MULTIPLICATION,
    DIVISION, SEMICOLON, ASSIGNMENT, LEFT_BRACE, RIGHT_BRACE};


std::ostream& operator<<(std::ostream& os, TokenType type)
{
    switch(type)
    {
        case INTEGER : os << "integer"; break;
        case FLOAT : os << "float"; break;
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
        unsigned int posion;

    public:
        Token(std::string value, TokenType type, unsigned int posion) {
            this->value = value;
            this->type = type;
            this->posion = posion;
        };

        friend std::ostream& operator<<(std::ostream& os, const Token& token) {
            os << token.value << " - " << token.type << " [" << token.posion << "]";
            return os;
        }
};


bool isToken (char& ch) {
    return ((ch == '=') || (ch == '+') || (ch == '-') || (ch == '%') ||
            (ch == '*') || (ch == '/') || (ch == '(') || (ch == ')') ||
            (ch == ';') || (ch == ' ') || (ch == '\n'));
}


std::vector<Token> getTokens (std::string& input) {
    unsigned int inputSize = input.size();
    std::vector<Token> tokens;

    const auto pushToken = [&tokens](char c, TokenType type, unsigned int pos) {
        tokens.push_back({std::string(1, c), type, pos});
    };
    for (unsigned int i = 0; i < inputSize; i++) {
        char currentChar = input[i];
        if (currentChar == '=') pushToken(currentChar, TokenType::ASSIGNMENT, i);
        else if (currentChar == '+') pushToken(currentChar, TokenType::ADDITION, i);
        else if (currentChar == '-') pushToken(currentChar, TokenType::SUBTRACTION, i);
        else if (currentChar == '%') pushToken(currentChar, TokenType::MODULO, i);
        else if (currentChar == '*') pushToken(currentChar, TokenType::MULTIPLICATION, i);
        else if (currentChar == '/') pushToken(currentChar, TokenType::DIVISION, i);
        else if (currentChar == '(') pushToken(currentChar, TokenType::RIGHT_BRACE, i);
        else if (currentChar == ')') pushToken(currentChar, TokenType::LEFT_BRACE, i);
        else if (currentChar == ';') pushToken(currentChar, TokenType::SEMICOLON, i);
        else if ((currentChar == ' ') || (currentChar == '\n')) continue;
        else if (isalpha(currentChar)) {
            std::string var = std::string(1, currentChar);
            unsigned int count = 0;
            for (unsigned int pos = i; pos < inputSize; pos++) {
                if ((pos+1 == inputSize) || isToken(input[pos+1])) {
                    tokens.push_back({var, TokenType::VARIABLE, pos-count});
                    break;
                } else if (isalpha(input[pos+1]) || isdigit(input[pos+1])) {
                    var.push_back(input[pos+1]);
                    count++;
                } else {
                    var.push_back(input[pos+1]);
                    count++;
                    std::cerr << "Invalid variable name at " << pos+1-count << std::endl;
                    exit(0);
                }
            }
            i += count;
        } else if (isdigit(currentChar)) {
            bool isDot = false;
            std::string num = std::string(1, currentChar);
            unsigned int count = 0;
            for (unsigned int pos = i; pos < inputSize; pos++) {
                if ((pos+1 == inputSize) || isToken(input[pos+1])) {
                    TokenType currType = (isDot) ? TokenType::FLOAT : TokenType::INTEGER;
                    tokens.push_back({num, currType, pos-count});
                    break;
                } else if (isdigit(input[pos+1])) {
                    num.push_back(input[pos+1]);
                    count++;
                } else if (input[pos+1] == '.') {
                    if (!isDot) isDot = true;
                    else {
                        std::cerr << "Invalid number format at " << pos-count << std::endl;
                        exit(0);
                    }
                    num.push_back(input[pos+1]);
                    count++;
                } else {
                    num.push_back(input[pos+1]);
                    count++;
                    std::cerr << "Invalid number format at " << pos+1-count << std::endl;
                    exit(0);
                }
            }
            i += count;
        }
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
