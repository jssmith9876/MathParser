#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <assert.h>

class Token {
private:
    char m_op;
    bool m_isOp = false;
    int m_val;
    bool m_isVal = false;
public:
    Token() {}
    Token(char op) { m_op = op; m_isOp = true; }
    Token(int val) { m_val = val; m_isVal = true; }

    bool isOp() const { return m_isOp; }

    bool isVal() const { return m_isVal; }

    char getOp() const {
        assert(m_isOp);
        return m_op;
    }

    int getVal() const {
        assert(m_isVal);
        return m_val;
    }
};


class Parser {
private:
    std::string m_infixStr;
    std::queue<Token> m_postfixQueue;

    // Private functions
    void parseToPostfix();
public:
    // Constructors
    Parser();
    Parser(std::string);

    // Getters
    std::string getInfixStr() const;
    std::string getPostfixStr() const;

    // Setters
    void setInfixStr(std::string);

    // Other
};

#endif
