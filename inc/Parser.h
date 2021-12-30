#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <stack>
#include <deque>
#include <vector>
#include <map>
#include <math.h>
#include <assert.h>
#include <algorithm>

// Helper Functions
bool isOperatorChar(const char& c);

/*
 *  Token class to handle generic elements in the expression
 */
class Token {
private:
    char m_op;
    bool m_isOp = false;

    double m_val;
    bool m_isVal = false;

    char m_iden;
    bool m_isIden = false;

    std::string m_func;
    bool m_isFunc = false;

    bool m_isLParen = false;
public:
    /*
     *  Constructors
     */
    Token() {}
    Token(const bool& isLParen) { m_isLParen = isLParen; }
    Token(const char& c) { 
        if (isOperatorChar(c)) { 
            m_op = c; m_isOp = true; 
        } else {
            m_iden = c; m_isIden = true;
        }
    }
    Token(const double& val) { m_val = val; m_isVal = true; }
    Token(const std::string& func) { m_func = func; m_isFunc = true; }

    /*
     *  Methods to get the type of the token
     */
    bool isOp() const { return m_isOp; }
    bool isVal() const { return m_isVal; }
    bool isIden() const { return m_isIden; }
    bool isFunc() const { return m_isFunc; }
    bool isLParen() const { return m_isLParen; }

    /*
     *  Getters
     */
    char getOp() const {
        assert(m_isOp);
        return m_op;
    }
    double getVal() const {
        assert(m_isVal);
        return m_val;
    }
    char getIden() const {
        assert(m_isIden);
        return m_iden;
    }
    std::string getFunc() const {
        assert(m_isFunc);
        return m_func;
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
    std::vector<char> m_freeVars;
    const std::vector<std::string> m_functions { "sin", "cos", "max" };

    // Private functions
    void parseToPostfix();
    double _evaluate(const std::map<char, double>&) const;
    void getFunctionPos(std::map<std::string, std::vector<size_t>>&) const;
public:
    // Constructors
    ExpressionHandler();
    ExpressionHandler(const std::vector<char>&);
    ExpressionHandler(const std::string&);
    ExpressionHandler(const std::vector<char>&, const std::string&);

    // Getters
    std::vector<char> getFreeVars() const;
    std::string getInfixStr() const;
    std::string getPostfixStr() const;

    // Setters
    void setFreeVars(const std::vector<char>&);
    void setInfixStr(const std::string&);

    // Other
    double evaluate() const; 
    double evaluate(const std::map<char, double>&) const;
};

#endif
