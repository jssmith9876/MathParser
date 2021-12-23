#include "../inc/Parser.h"

/*
 *  Constructors
 */
Parser::Parser() {}
Parser::Parser(std::string infixStr) {
    m_infixStr = infixStr;
    parseToPostfix();
}

/*
 *  Getters
 */
std::string Parser::getInfixStr() const {
    return m_infixStr;
}

std::string Parser::getPostfixStr() const {
    std::string res = "";
    std::queue<Token> temp_q = m_postfixQueue;
    Token t;

    while (!temp_q.empty()) {
        t = temp_q.front();
        if (t.isOp())
            res += t.getOp();
        else 
            res += std::to_string(t.getVal());
        res += " ";

        temp_q.pop();
    }

    return res;
}

/*
 *  Setters
 */
void Parser::setInfixStr(std::string infixStr) {
    m_infixStr = infixStr;
    parseToPostfix();
}

/*
 *  Other
 */
void Parser::parseToPostfix() {
    std::stack<Token> operatorStack;
    std::string temp = "";
    char currentChar;

    for (size_t i = 0; i < m_infixStr.length(); i++) {
        // Evaluate the character
        currentChar = m_infixStr[i];
        if (currentChar == ' ') {
            // If we have a temp number in our string, push it as an int
            if (temp.length()) {
                m_postfixQueue.push(Token(stoi(temp)));
                temp = "";
            }
            continue;
        } else if (isdigit(currentChar)) {
             temp += currentChar;
        // Operators
        } else {
            operatorStack.push(Token(currentChar));
        } 
    }

    // If we still have anything in our temp holder, then add it to the output queue
    if (temp.length()) {
        m_postfixQueue.push(Token(stoi(temp)));
    }

    while (!operatorStack.empty()) {
        m_postfixQueue.push(operatorStack.top());
        operatorStack.pop();
    }
}
