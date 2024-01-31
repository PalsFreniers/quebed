// quebed : a queue based programing language developped in c++
// give me a file extension : .qbd
#define QUEBED_FILE_EXTENSION ".qbd"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <exception>
#include <queue>

class InvalidNumberException : public std::exception {
        virtual const char* what() const throw() {
                return "Invalid number";
        }
};

enum TokenType {
        NUMBER, STRING, CHAR,
        WORD,
};

enum Intrinsics {
        PLUS, MINUS, MULT, DIV, MOD,
};

enum Keyword {
        DUMP,
};

struct ParseData {
        bool isKeyword;
        Intrinsics intrinsic;
        Keyword keyword;
};

struct Token {
        TokenType type;
        std::string svalue;
        unsigned long nvalue;
        std::string file;
        int line;
        int column;
        ParseData data;

        Token(TokenType type, std::string svalue, std::string file, int line, int column) : type(type), svalue(svalue), file(file), line(line), column(column) {}
        Token(TokenType type, unsigned long nvalue, std::string file, int line, int column) : type(type), nvalue(nvalue), file(file), line(line), column(column) {}

        Token(const Token& token) : type(token.type), file(token.file), line(token.line), column(token.column) {
                if (token.type == NUMBER) {
                        nvalue = token.nvalue;
                } else {
                        svalue = token.svalue;
                }
        }

        Token &operator=(const Token& token) {
                if (this != &token) {
                        type = token.type;
                        file = token.file;
                        line = token.line;
                        column = token.column;
                        if (token.type == NUMBER) {
                                nvalue = token.nvalue;
                        } else {
                                svalue = token.svalue;
                        }
                }
                return *this;
        }
};

std::ostream& operator<<(std::ostream& os, const Token& token) {
        os << token.file << ":" << token.line << ":" << token.column << " -> ";
        if (token.type == NUMBER) os << token.nvalue;
        else os << token.svalue;
        return os;
}

std::string slurp(std::string fileName) {
        std::ifstream file(fileName);
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        return content;
}

unsigned long atolThrow(std::string num) throw(InvalidNumberException) {
        unsigned long result = 0;
        for (size_t i = 0; i < num.size(); i++) {
                if (num[i] < '0' || num[i] > '9') {
                        throw InvalidNumberException();
                }
                result = result * 10 + (num[i] - '0');
        }
        return result;
}

std::vector<Token> splitTokens(std::string name, std::string content) throw(std::runtime_error) {
        std::vector<Token> tokens;
        std::string token;
        std::string buff;
        int line = 1;
        int column = 1;
        for (size_t i = 0; i < content.size(); i++) {
                if (content[i] == '\n') {
                        line++;
                        column = 0;
                } else if(content[i] == '\"') {
                        while (content[++i] != '\"') {
                                if (content[i] == '\\') {
                                        buff += content[i];
                                        i++;
                                } else if(content[i] == '\n') {
                                        line++;
                                        column = 0;
                                }
                                buff += content[i++];
                        }
                        tokens.push_back(Token(STRING, buff, name, line, column));
                        buff = "";
                } else if(content[i] == '\'') {
                        buff += content[i];
                        if(content[i + 1] == '\\') {
                                buff += content[++i];
                        }
                        buff += content[++i];
                        if(content[i + 1] != '\'') {
                                throw std::runtime_error("Invalid character");
                        }
                        buff += content[++i];
                        tokens.push_back(Token(STRING, buff, name, line, column));
                        buff = "";
                } else if(content[i] == ' ') {
                        try {
                                tokens.push_back({NUMBER, atolThrow(buff), name, line, column});
                        } catch (InvalidNumberException e) {
                                tokens.push_back({WORD, buff, name, line, column});
                        }
                        buff = "";
                        while(content[i + 1] == ' ') {
                                i++;
                                column++;
                        }
                } else {
                        buff += content[i];
                } 
                column++;
        }
        if(!buff.empty()) {
                try {
                        tokens.push_back({NUMBER, atolThrow(buff), name, line, column});
                        std::cout << "Number : " << buff << std::endl;
                } catch (InvalidNumberException e) {
                        tokens.push_back({WORD, buff, name, line, column});
                }
        }
        return tokens;
}

std::vector<Token> parse(std::vector<Token> tokens) throw(std::runtime_error) {
        for (size_t i = 0; i < tokens.size(); i++) {
                if (tokens[i].type == WORD) {
                        if(tokens[i].svalue == "dump") {
                                tokens[i].data.isKeyword = true;
                                tokens[i].data.keyword = DUMP;
                        } else if(tokens[i].svalue == "+") {
                                tokens[i].data.isKeyword = false;
                                tokens[i].data.intrinsic = PLUS;
                        } else if(tokens[i].svalue == "-") {
                                tokens[i].data.isKeyword = false;
                                tokens[i].data.intrinsic = MINUS;
                        } else if(tokens[i].svalue == "*") {
                                tokens[i].data.isKeyword = false;
                                tokens[i].data.intrinsic = MULT;
                        } else if(tokens[i].svalue == "/") {
                                tokens[i].data.isKeyword = false;
                                tokens[i].data.intrinsic = DIV;
                        } else if(tokens[i].svalue == "%") {
                                tokens[i].data.isKeyword = false;
                                tokens[i].data.intrinsic = MOD;
                        } else {
                                throw std::runtime_error("Unknown word");
                        }
                }
        }
        return tokens;
}

void simulate(std::vector<Token> tokens) {
        std::vector<std::string> strings;
        std::queue<unsigned long> queue;
        for (size_t i = 0; i < tokens.size(); i++) {
                switch(tokens[i].type) {
                        case NUMBER:
                                queue.push(tokens[i].nvalue);
                                break;
                        case STRING:
                }
        }
}

int main(int argc, char **argv) {
        if (argc < 2) {
                std::cout << "Usage: " << argv[0] << " <file>" << std::endl;
                return 1;
        }
        std::string fileName = argv[1];
        std::string fileContent = slurp(fileName);
        std::cout << fileContent << std::endl;
        try {
        std::vector<Token> tokens = splitTokens(argv[1], fileContent);
        for (size_t i = 0; i < tokens.size(); i++) {
                std::cout << tokens[i] << std::endl;
        }
        tokens = parse(tokens);
        } catch (std::exception e) {
                std::cout << e.what() << std::endl;
                return 1;
        }
        simulate(tokens);
        return 0;
}
