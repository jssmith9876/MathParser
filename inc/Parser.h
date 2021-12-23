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
    bool m_isLParen = false;
public:
    Token() {}
    Token(bool isLParen) { m_isLParen = isLParen; }
    Token(char op) { m_op = op; m_isOp = true; }
    Token(int val) { m_val = val; m_isVal = true; }

    bool isOp() const { return m_isOp; }

    bool isVal() const { return m_isVal; }

    bool isLParen() const { return m_isLParen; }

    char getOp() const {
        assert(m_isOp);
        return m_op;
    }

    int getVal() const {
        assert(m_isVal);
        return m_val;
    }

    int getPrec() const {
        assert(m_isOp);
        if (m_op == '+' || m_op == '-') 
            return 2;
        else if (m_op == '*' || m_op == '/') 
            return 3;
        else if (m_op == '^') 
            return 4;
        else 
            return -1;
    }

    bool isLAssoc() const {
        assert(m_isOp);
        
        return (m_op != '^');
    }

};


class Parser {
private:
    std::string m_infixStr;
    std::queue<Token> m_postfixQueue;

    // Private functions
    void parseToPostfix();
    bool isOperator(char) const;
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
