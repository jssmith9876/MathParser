#include "../inc/Parser.h"

/*
 *  Helper Functions
 */
bool isOperatorChar(const char& c) {
    if (c == '+' ||
        c == '-' ||
        c == '*' ||
        c == '/' ||
        c == '^') return true;
    return false;
}

/*
 *  Constructors
 */
ExpressionHandler::ExpressionHandler() {}
ExpressionHandler::ExpressionHandler(const std::vector<char>& freeVars) {
    m_freeVars = freeVars;
}
ExpressionHandler::ExpressionHandler(const std::string& infixStr) {
    m_infixStr = infixStr;
    parseToPostfix();
}
ExpressionHandler::ExpressionHandler(const std::vector<char>& freeVars, const std::string& infixStr) {
    m_freeVars = freeVars;
    m_infixStr = infixStr;
    parseToPostfix();
}

/*
 *  Getters
 */
std::vector<char> ExpressionHandler::getFreeVars() const {
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
        else if (t.isIden()) 
            res += t.getIden();
        else
            res += std::to_string(t.getVal());
        res += " ";
    }

    return res;
}

/*
 *  Setters
 */
void ExpressionHandler::setFreeVars(const std::vector<char>& freeVars) {
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
    char prevChar = ' ', currentChar, nextChar;
    std::vector<char>::iterator freeVarIter;

    for (int i = 0; i < infixStrLen; i++) {
        // Evaluate the character
        currentChar = m_infixStr[i];
        
        // Get the next character (for parsing numbers)
        if (i + 1 < infixStrLen) {
            nextChar = m_infixStr[i + 1];
        } else {
            nextChar = ' ';
        }

        // Check if the current char is a free variable
        freeVarIter = std::find(m_freeVars.begin(), m_freeVars.end(), currentChar);


        if (freeVarIter != m_freeVars.end() && 
            !isalpha(prevChar) &&
            !isalpha(nextChar)) {
            
            m_postfixQueue.push_back(Token(*freeVarIter));  
        } else if (currentChar == ' ') {
            continue;
        } else if (isdigit(currentChar) || currentChar == '.') {
             temp += currentChar;

             // If the next character is NOT a digit or a decimal, add the current number onto the output
             if (!isdigit(nextChar) && nextChar != '.') {
                m_postfixQueue.push_back(Token(stod(temp)));
                temp = "";
             }
        // Operators
        } else if (isOperatorChar(currentChar)) {
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
                    if (operatorStack.empty()) {
                        break;
                    }
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

        // Set the previous char
        prevChar = currentChar;

    }

    // If we still have anything in our temp holder, then add it to the output deque
    if (temp.length()) {
        m_postfixQueue.push_back(Token(stod(temp)));
    }

    // Push back the rest of the operators left on the stack (unless it's a left paren, then we throw an error)
    while (!operatorStack.empty()) {
        assert(!operatorStack.top().isLParen());

        m_postfixQueue.push_back(operatorStack.top());
        operatorStack.pop();
    }
}

// Private evaluate function that the public evaluate functions will call
double ExpressionHandler::_evaluate(const std::map<char, double>& valueMap) const {
    assert(m_postfixQueue.size());  // Make sure we have a queue to evaluate
    assert(valueMap.size() == m_freeVars.size());   // Make sure we're provided with the same number of values as we have free vars
    
    std::deque<Token>::const_iterator iter = m_postfixQueue.begin();
    Token t;
    std::stack<double> valueStack;
    char currOp, currIden;
    double temp1, temp2;

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
        } else if (t.isIden()) {
            currIden = t.getIden();
            assert(valueMap.count(currIden));   // Make sure the user provided a value for the free var
            
            valueStack.push(valueMap.at(currIden));
        }
    }

    // Sanity check
    assert(valueStack.size() == 1);

    return valueStack.top();
}

double ExpressionHandler::evaluate() const {
    return _evaluate(std::map<char, double>());
}

double ExpressionHandler::evaluate(const std::map<char, double>& valueMap) const {
    return _evaluate(valueMap);
}
