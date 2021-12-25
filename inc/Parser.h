#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <stack>
#include <deque>
#include <vector>
#include <math.h>
#include <assert.h>

/*
 *  Token class to handle generic elements in the expression
 */
class Token {
private:
    char m_op;
    bool m_isOp = false;
    int m_val;
    bool m_isVal = false;
    bool m_isLParen = false;
public:
    /*
     *  Constructors
     */
    Token() {}
    Token(const bool& isLParen) { m_isLParen = isLParen; }
    Token(const char& op) { m_op = op; m_isOp = true; }
    Token(const int& val) { m_val = val; m_isVal = true; }

    /*
     *  Methods to get the type of the token
     */
    bool isOp() const { return m_isOp; }
    bool isVal() const { return m_isVal; }
    bool isLParen() const { return m_isLParen; }

    /*
     *  Getters
     */
    char getOp() const {
        assert(m_isOp);
        return m_op;
    }
    int getVal() const {
        assert(m_isVal);
        return m_val;
    }


    // Function to get operator precedence
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

    // Function to get whether the operator is left associative
    bool isLAssoc() const {
        assert(m_isOp);
        return (m_op != '^');
    }

};

/*
 *  Expression handler class 
 */
class ExpressionHandler {
private:
    std::string m_infixStr;
    std::deque<Token> m_postfixQueue;
    std::vector<std::string> m_freeVars;

    // Private functions
    void parseToPostfix();
    bool isOperator(const char&) const;
public:
    // Constructors
    ExpressionHandler();
    ExpressionHandler(const std::vector<std::string>&);
    ExpressionHandler(const std::string&);
    ExpressionHandler(const std::vector<std::string>&, const std::string&);

    // Getters
    std::vector<std::string> getFreeVars() const;
    std::string getInfixStr() const;
    std::string getPostfixStr() const;

    // Setters
    void setFreeVars(const std::vector<std::string>&);
    void setInfixStr(const std::string&);

    // Other
    int evaluate() const; 
};

#endif
