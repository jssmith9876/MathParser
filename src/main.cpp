#include <iostream>
#include "../inc/Parser.h"

int main() {
    // Create an infix string to parse through
    std::string infixTest = "sin( max(2, 3) / 3 * 3.14)";

    ExpressionHandler myParser(infixTest);

    std::cout << myParser.getPostfixStr() << std::endl;

    std::cout << myParser.evaluate() << std::endl;

    return 0;
}
