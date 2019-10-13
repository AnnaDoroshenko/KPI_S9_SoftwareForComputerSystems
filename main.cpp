#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cctype>
#include <stdlib.h>
#include <unordered_map>
#include <optional>


enum class TokenType {INTEGER, FLOAT, VARIABLE, ADDITION, SUBTRACTION, MODULO,
    MULTIPLICATION, DIVISION, SEMICOLON, ASSIGNMENT, LEFT_BRACE, RIGHT_BRACE};


std::ostream& operator<<(std::ostream& os, TokenType type) {
    switch(type)
    {
        case TokenType::INTEGER : os << "integer"; break;
        case TokenType::FLOAT : os << "float"; break;
        case TokenType::VARIABLE : os << "variable"; break;
        case TokenType::ADDITION : os << "addition"; break;
        case TokenType::SUBTRACTION : os << "subtraction"; break;
        case TokenType::MODULO : os << "modulo"; break;
        case TokenType::MULTIPLICATION : os << "multiplication"; break;
        case TokenType::DIVISION : os << "division"; break;
        case TokenType::SEMICOLON : os << "semicolon"; break;
        case TokenType::ASSIGNMENT : os << "assignment"; break;
        case TokenType::LEFT_BRACE : os << "left brace"; break;
        case TokenType::RIGHT_BRACE : os << "right brace"; break;
    }

    return os;
}


struct Token {
    private:
        std::string value;
        TokenType type;
        unsigned int position;

    public:
        Token(const std::string& value, TokenType type, unsigned int position)
                : value(value), type(type), position(position) {}

        std::string getValue() const {
            return value;
        }

        TokenType getType() const {
            return type;
        }

        unsigned int getPosition() const {
            return position;
        }

        friend std::ostream& operator<<(std::ostream& os, const Token& token);
};


std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << token.value << " - " << token.type << " [" << token.position << "]";
    return os;
}


bool isToken (char ch) {
    return ((ch == '=') || (ch == '+') || (ch == '-') || (ch == '%') ||
            (ch == '*') || (ch == '/') || (ch == '(') || (ch == ')') ||
            (ch == ';') || (ch == ' ') || (ch == '\n'));
}


std::optional<std::vector<Token>> getTokens (const std::string& input) {
    const unsigned int inputSize = input.size();
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
        else if (currentChar == '(') pushToken(currentChar, TokenType::LEFT_BRACE, i);
        else if (currentChar == ')') pushToken(currentChar, TokenType::RIGHT_BRACE, i);
        else if (currentChar == ';') pushToken(currentChar, TokenType::SEMICOLON, i);
        else if ((currentChar == ' ') || (currentChar == '\n')) continue;
        else if (isalpha(currentChar)) {
            std::string var(1, currentChar);
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
                    std::cout << "Invalid variable name at " << (pos+1-count) << std::endl;
                    return {};
                }
            }
            i += count;
        } else if (isdigit(currentChar)) {
            bool isDot = false;
            std::string num(1, currentChar);
            unsigned int count = 0;
            for (unsigned int pos = i; pos < inputSize; pos++) {
                if ((pos+1 == inputSize) || isToken(input[pos+1])) {
                    TokenType currType = isDot ? TokenType::FLOAT : TokenType::INTEGER;
                    tokens.push_back({num, currType, pos-count});
                    break;
                } else if (isdigit(input[pos+1])) {
                    num.push_back(input[pos+1]);
                    count++;
                } else if (input[pos+1] == '.') {
                    if (!isDot) isDot = true;
                    else {
                        std::cout << "Invalid number format at ["
                            << (pos-count) << "]" << std::endl;
                        return {};
                    }
                    num.push_back(input[pos+1]);
                    count++;
                } else {
                    num.push_back(input[pos+1]);
                    count++;
                    std::cout << "Invalid number format at ["
                        << (pos+1-count) << "]" << std::endl;
                    return {};
                }
            }
            i += count;
        }
    }

    if (tokens.back().getType() != TokenType::SEMICOLON) {
        std::cout << "Missed semicolon in the end of sequence" << std::endl;
        return {};
    }


    return tokens;
}


bool checkBraces (const std::vector<Token>& tokens) {
    using Position = unsigned int;
    std::vector<Position> braces;
    for (const auto& currentToken : tokens) {
        if (currentToken.getValue() == "(") braces.push_back(currentToken.getPosition());
        else if (currentToken.getValue() == ")") {
            if (braces.empty()) {
                std::cout << "Missed left brace for ["
                    << currentToken.getPosition() << "]" << std::endl;
                return false;
            }
            braces.pop_back();
        }
    }
    if (!braces.empty()) {
        std::cout << "Missed right brace for ["
            << braces.back() << "]" << std::endl;
        return false;
    }

    return true;
}


using TokenList = std::vector<TokenType>;

bool isValid(std::unordered_map<TokenType, TokenList>& tokenMap,
        const TokenType& current, const TokenType& previous) {
    const std::vector<TokenType>& validTokens = tokenMap[current];
    return (std::find(validTokens.cbegin(), validTokens.cend(), previous)
            != validTokens.cend());
}


bool checkOrder (const std::vector<Token>& tokens) {
    std::unordered_map<TokenType, TokenList> tokenMap;
    tokenMap[TokenType::INTEGER] = {TokenType::ADDITION, TokenType::SUBTRACTION,
        TokenType::MODULO, TokenType::MULTIPLICATION, TokenType::DIVISION,
        TokenType::ASSIGNMENT, TokenType::LEFT_BRACE};
    tokenMap[TokenType::FLOAT] = {TokenType::ADDITION, TokenType::SUBTRACTION,
        TokenType::MODULO, TokenType::MULTIPLICATION, TokenType::DIVISION,
        TokenType::ASSIGNMENT, TokenType::LEFT_BRACE};
    tokenMap[TokenType::VARIABLE] = {TokenType::ADDITION, TokenType::SUBTRACTION,
        TokenType::MODULO, TokenType::MULTIPLICATION, TokenType::DIVISION,
        TokenType::ASSIGNMENT, TokenType::LEFT_BRACE};
    tokenMap[TokenType::ADDITION] = {TokenType::INTEGER, TokenType::FLOAT,
        TokenType::VARIABLE, TokenType::ADDITION, TokenType::SUBTRACTION,
        TokenType::LEFT_BRACE, TokenType::RIGHT_BRACE};
    tokenMap[TokenType::SUBTRACTION] = {TokenType::INTEGER, TokenType::FLOAT,
        TokenType::VARIABLE, TokenType::ADDITION, TokenType::SUBTRACTION,
        TokenType::LEFT_BRACE, TokenType::RIGHT_BRACE};
    tokenMap[TokenType::MODULO] = {TokenType::INTEGER, TokenType::FLOAT,
        TokenType::VARIABLE, TokenType::RIGHT_BRACE};
    tokenMap[TokenType::MULTIPLICATION] = {TokenType::INTEGER, TokenType::FLOAT,
        TokenType::VARIABLE, TokenType::RIGHT_BRACE};
    tokenMap[TokenType::DIVISION] = {TokenType::INTEGER, TokenType::FLOAT,
        TokenType::VARIABLE, TokenType::RIGHT_BRACE};
    tokenMap[TokenType::SEMICOLON] = {TokenType::INTEGER, TokenType::FLOAT,
        TokenType::VARIABLE, TokenType::RIGHT_BRACE};
    tokenMap[TokenType::ASSIGNMENT] = {TokenType::VARIABLE};
    tokenMap[TokenType::LEFT_BRACE] = {TokenType::ADDITION, TokenType::SUBTRACTION,
        TokenType::MODULO, TokenType::MULTIPLICATION, TokenType::DIVISION,
        TokenType::ASSIGNMENT, TokenType::LEFT_BRACE};
    tokenMap[TokenType::RIGHT_BRACE] = {TokenType::INTEGER, TokenType::FLOAT,
        TokenType::VARIABLE, TokenType::RIGHT_BRACE};

    Token previousToken = tokens.at(0);
    TokenType previousType = previousToken.getType();
    for (unsigned int i = 1; i < tokens.size(); i++) {
        Token currentToken = tokens.at(i);
        TokenType currentType = currentToken.getType();
        if (!isValid(tokenMap, currentType, previousType)) {
            std::cout << "Invalid type " << currentType << " after " << previousType
                << " at [" << previousToken.getPosition() << "]" << std::endl;
            return false;
        }
        previousToken = currentToken;
        previousType = currentType;
    }

    return true;
}


bool checkSigns(const std::vector<Token>& tokens) {
    if (tokens.empty()) return true;

    const auto firstType = tokens[0].getType();
    if (firstType == TokenType::MULTIPLICATION ||
            firstType == TokenType::DIVISION ||
            firstType == TokenType::MODULO) {
        std::cout << "Invalid first token" << std::endl;
        return false;
    }

    bool begin = true;
    int count = 0;
    for (unsigned int i = 0; i < tokens.size() - 1; i++) {
        const auto type = tokens[i].getType();
        if (type == TokenType::ADDITION ||
                type == TokenType::SUBTRACTION ||
                type == TokenType::MULTIPLICATION ||
                type == TokenType::DIVISION) {
            count++;
        } else count = 0;

        if ((begin && count > 1) || (count > 2)) {
            std::cout << "Too many signs in sequence!" << std::endl;
            return false;
        }
        if (i == 1) begin = false;
    }

    return true;
}


int main() {
    std::ifstream input("input.txt");
    std::stringstream ss;
    ss << input.rdbuf();

    std::string str = ss.str();
    std::cout << "Input: " << str << std::endl;
    std::optional<std::vector<Token>> tokensOpt = getTokens(str);

    if (!tokensOpt) return -1;

    const auto tokens = *tokensOpt;

    if (!checkBraces(tokens)) return -1;
    if (!checkOrder(tokens)) return -1;
    if (!checkSigns(tokens)) return -1;

    for (unsigned int t = 0; t < tokens.size(); t++) {
        std::cout << tokens.at(t) << std::endl;
    }

    return 0;
}
