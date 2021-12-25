#include "../inc/Parser.h"

/*
 *  Constructors
 */
ExpressionHandler::ExpressionHandler() {}
ExpressionHandler::ExpressionHandler(const std::vector<std::string>& freeVars) {
    m_freeVars = freeVars;
}
ExpressionHandler::ExpressionHandler(const std::string& infixStr) {
    m_infixStr = infixStr;
    parseToPostfix();
}
ExpressionHandler::ExpressionHandler(const std::vector<std::string>& freeVars, const std::string& infixStr) {
    m_freeVars = freeVars;
    m_infixStr = infixStr;
    parseToPostfix();
}

/*
 *  Getters
 */
std::vector<std::string> ExpressionHandler::getFreeVars() const {
    return m_freeVars;
}

std::string ExpressionHandler::getInfixStr() const {
    return m_infixStr;
}

std::string ExpressionHandler::getPostfixStr() const {
    std::string res = "";
    std::deque<Token>::const_iterator iter = m_postfixQueue.begin();
    Token t;

    while (iter != m_postfixQueue.end()) {
        t = *iter++;
        if (t.isOp())
            res += t.getOp();
        else
            res += std::to_string(t.getVal());
        res += " ";
    }

    return res;
}

/*
 *  Setters
 */
void ExpressionHandler::setFreeVars(const std::vector<std::string>& freeVars) {
    m_freeVars = freeVars;
}

void ExpressionHandler::setInfixStr(const std::string& infixStr) {
    m_infixStr = infixStr;
    parseToPostfix();
}

/*
 *  Other
 */

/*
 *  This method utilizes the Shunting-yard algorithm. 
 *      More information can be found here: https://en.wikipedia.org/wiki/Shunting-yard_algorithm
 */
void ExpressionHandler::parseToPostfix() {
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
                m_postfixQueue.push_back(Token(stoi(temp)));
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
                    m_postfixQueue.push_back(stackTopToken);
                    operatorStack.pop();
                    stackTopToken = operatorStack.top();
                }
            }
            
            operatorStack.push(Token(currentChar));

        } else if (currentChar == '(') {
            // Add a left paren to the op stack
            operatorStack.push(Token(true));

        } else if (currentChar == ')') {

            // While the top of the stack is not a left paren, pop it off and store it into the deque
            Token stackTopToken = operatorStack.top();

            while (!stackTopToken.isLParen()) {
                assert(!operatorStack.empty());

                m_postfixQueue.push_back(stackTopToken);
                operatorStack.pop();
                stackTopToken = operatorStack.top();
            }

            assert(operatorStack.top().isLParen());
            operatorStack.pop();
        }

    }

    // If we still have anything in our temp holder, then add it to the output deque
    if (temp.length()) {
        m_postfixQueue.push_back(Token(stoi(temp)));
    }

    // Push back the rest of the operators left on the stack (unless it's a left paren, then we throw an error)
    while (!operatorStack.empty()) {
        assert(!operatorStack.top().isLParen());

        m_postfixQueue.push_back(operatorStack.top());
        operatorStack.pop();
    }
}

bool ExpressionHandler::isOperator(const char& c) const {
    // Return true ONLY IF we are given an operator
    if (c == '+' ||
        c == '-' ||
        c == '*' ||
        c == '/' ||
        c == '^') return true;

    return false;
}

int ExpressionHandler::evaluate() const {
    assert(m_postfixQueue.size());  // Make sure we have a queue to evaluate
    
    std::deque<Token>::const_iterator iter = m_postfixQueue.begin();
    Token t;
    std::stack<int> valueStack;
    char currOp;
    int temp1, temp2;

    while (iter != m_postfixQueue.end()) {
        t = *iter++;
        if (t.isVal()) {
            valueStack.push(t.getVal());
        } else if (t.isOp()) {
            currOp = t.getOp();
            
            // Get the top two values of the stack
            temp1 = valueStack.top(); valueStack.pop();
            temp2 = valueStack.top(); valueStack.pop();

            if (currOp == '+') {
                valueStack.push(temp2 + temp1);
            } else if (currOp == '-') {
                valueStack.push(temp2 - temp1);
            } else if (currOp == '*') {
                valueStack.push(temp2 * temp1);
            } else if (currOp == '/') {
                valueStack.push(temp2 / temp1);
            } else if (currOp == '^') {
                valueStack.push(pow(temp2, temp1));
            }
        }
    }

    // Sanity check
    assert(valueStack.size() == 1);

    return valueStack.top();

}
