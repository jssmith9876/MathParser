#include <iostream>
#include "../inc/Parser.h"

int main(int argc, char** argv) {
    // Create an infix string to parse through
    std::string test = "sin( max(2, 3) / 3 * 3.14) ";

    ExpressionHandler myParser(test);

    std::cout << myParser.getPostfixStr() << std::endl;

    // std::cout << myParser.evaluate(valueMap) << std::endl;

    return 0;
}
