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
    std::queue<Token> temp_q = m_postfixQueue;  // Temporary queue to output the string
    Token t;

    // Pull each token from the temp queue and add it to the string
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
    int infixStrLen = m_infixStr.length();
    char currentChar;
    char nextChar;

    for (int i = 0; i < infixStrLen; i++) {
        // Evaluate the character
        currentChar = m_infixStr[i];
        if (i + 1 < infixStrLen)
            nextChar = m_infixStr[i + 1];
        else 
            nextChar = ' ';

        if (currentChar == ' ') {
            continue;
        } else if (isdigit(currentChar)) {
             temp += currentChar;

             // If the next character is NOT a digit, add the current number onto the output
             if (!isdigit(nextChar)) {
                m_postfixQueue.push(Token(stoi(temp)));
                temp = "";
             }
        // Operators
        } else if (isOperator(currentChar)) {
            Token currentToken(currentChar);
            
            if (!operatorStack.empty()) {
                Token stackTopToken = operatorStack.top();

                // This while loop handles the precedence of the operators

                // While the top of the stack is not a left paren AND 
                //  the top of the stack has a higher prec than the current op OR 
                //  the top of the stack and the current op have the same prec AND the current op is left associative:
                while (!stackTopToken.isLParen() && (currentToken.getPrec() < stackTopToken.getPrec() ||
                                                   (currentToken.getPrec() == stackTopToken.getPrec() && currentToken.isLAssoc()))) {
                    m_postfixQueue.push(stackTopToken);
                    operatorStack.pop();
                    stackTopToken = operatorStack.top();
                }
            }
            
            operatorStack.push(Token(currentChar));

        } else if (currentChar == '(') {
            // Add a left paren to the op stack
            operatorStack.push(Token(true));

        } else if (currentChar == ')') {

            // While the top of the stack is not a left paren, pop it off and store it into the queue
            Token stackTopToken = operatorStack.top();

            while (!stackTopToken.isLParen()) {
                assert(!operatorStack.empty());

                m_postfixQueue.push(stackTopToken);
                operatorStack.pop();
                stackTopToken = operatorStack.top();
            }

            assert(operatorStack.top().isLParen());
            operatorStack.pop();
        }

    }

    // If we still have anything in our temp holder, then add it to the output queue
    if (temp.length()) {
        m_postfixQueue.push(Token(stoi(temp)));
    }

    while (!operatorStack.empty()) {
        assert(!operatorStack.top().isLParen());

        m_postfixQueue.push(operatorStack.top());
        operatorStack.pop();
    }
}

bool Parser::isOperator(char c) const {
    if (c == '+' ||
        c == '-' ||
        c == '*' ||
        c == '/' ||
        c == '^') return true;

    return false;
}
