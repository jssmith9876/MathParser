#include "../inc/Parser.h"

/*
 *  START HELPER FUNCTIONS
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
 *  END HELPER FUNCTIONS
 */

/*
 *  START CONSTRUCTORS 
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
 *  END CONSTRUCTORS
 */

/*
 *  START GETTERS
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
        else if (t.isFunc())
            res += t.getFunc();
        else
            res += std::to_string(t.getVal());
        res += " ";
    }

    return res;
}
/*
 *  END GETTERS
 */

/*
 *  START SETTERS
 */
void ExpressionHandler::setFreeVars(const std::vector<char>& freeVars) {
    m_freeVars = freeVars;
}

void ExpressionHandler::setInfixStr(const std::string& infixStr) {
    m_infixStr = infixStr;
    parseToPostfix();
}
/*
 *  END SETTERS
 */


/*
 *  START PARSER FUNCTIONS
 */
// This algorithm uses the Shunting-Yard algorithm
//  For more information, reference the Wiki page: https://en.wikipedia.org/wiki/Shunting-yard_algorithm 
void ExpressionHandler::parseToPostfix() {
    std::stack<Token> operatorStack;
    std::string temp = "";
    size_t infixStrLen = m_infixStr.length();
    char prevChar = ' ', currentChar, nextChar;

    std::vector<char>::iterator freeVarIter;

    // For finding the math functions in the str
    std::map<std::string, std::vector<size_t>> functionPosInStr;
    std::map<std::string, std::vector<size_t>>::iterator functionIterator;
    std::vector<size_t>::iterator funPosIterator;
    bool hasFoundFunction;

    // Get the all of the function positions in the string
    getFunctionPos(functionPosInStr);

    for (size_t i = 0; i < infixStrLen; i++) {
        hasFoundFunction = false;

        // First, check if we're at a function position
        for (functionIterator = functionPosInStr.begin(); functionIterator != functionPosInStr.end(); functionIterator++) {
            funPosIterator = std::find(functionIterator->second.begin(), functionIterator->second.end(), i);

            // We found the function
            if (funPosIterator != functionIterator->second.end()) {
                operatorStack.push(Token(functionIterator->first));

                // Increase the size of i to skip over the rest of the function
                i += functionIterator->first.size() - 1;
                
                hasFoundFunction = true;
                break;  // We found a function, so no need to look for any more
            }
        }

        // If we've found a function, we don't need to look any further
        if (hasFoundFunction) 
            continue;

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

        // Handle free variables
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

            if (!operatorStack.empty() && operatorStack.top().isFunc()) {
                m_postfixQueue.push_back(operatorStack.top());
                operatorStack.pop();
            }
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
    
// Function to find all of the positions of the predefined functions in the string
void ExpressionHandler::getFunctionPos(std::map<std::string, std::vector<size_t>>& functionPosInStr) const {
    std::vector<size_t> currFunctionPos;
    size_t currPos;
    size_t infixStrSize = m_infixStr.size();

    // Something that could probably be parallelized but I'm too lazy :-)
    for (auto& func : m_functions) {
        currPos = m_infixStr.find(func);

        while (currPos != std::string::npos) {
            currFunctionPos.push_back(currPos);

            currPos = m_infixStr.find(func, currPos + infixStrSize);
        }

        functionPosInStr[func] = currFunctionPos;

        // Reset the variables
        currFunctionPos.clear();
    }
} 
/*
 *  END PARSER FUNCTIONS
 */


/*
 *  START EVALUATOR FUNCTIONS
 */

// Private evaluate function that the public evaluate functions will call
double ExpressionHandler::_evaluate(const std::map<char, double>& valueMap) const {
    assert(m_postfixQueue.size());  // Make sure we have a queue to evaluate
    assert(valueMap.size() == m_freeVars.size());   // Make sure we're provided with the same number of values as we have free vars
    
    std::deque<Token>::const_iterator iter = m_postfixQueue.begin();
    Token t;
    std::stack<double> valueStack;
    char currOp, currIden;
    double temp1, temp2;
    std::string currFunc;

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
        } else if (t.isFunc()) {
            currFunc = t.getFunc();
            temp1 = valueStack.top(); valueStack.pop();

            // START TRIG FUNCTIONS
            if (currFunc == "sin") {
                valueStack.push(sin(temp1));
            } else if (currFunc == "cos") {
                valueStack.push(cos(temp1));
            } else if (currFunc == "tan") {
                valueStack.push(tan(temp1));
            } else if (currFunc == "csc") {
                valueStack.push(1.0 / sin(temp1));
            } else if (currFunc == "sec") {
                valueStack.push(1.0 / cos(temp1));
            } else if (currFunc == "cot") {
                valueStack.push(1.0 / tan(temp1));
            } 
            // END TRIG FUNCTIONS
            // START MISC FUNCTIONS
            else if (currFunc == "max") {
                temp2 = valueStack.top(); valueStack.pop();

                valueStack.push( (temp1 > temp2) ? temp1 : temp2 );
            } else if (currFunc == "min") {
                temp2 = valueStack.top(); valueStack.pop();

                valueStack.push( (temp1 < temp2) ? temp1 : temp2 );
            }
            // END MISC FUNCTIONS
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

/*
 *  END EVALUATOR FUNCTIONS
 */


