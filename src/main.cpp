#include <iostream>
#include "../inc/Parser.h"

int main(int argc, char** argv) {
    std::string test = "(3.5 + 2.5) * 1.23";

    ExpressionHandler myParser(test);

    std::cout << myParser.getPostfixStr() << std::endl;

    std::cout << myParser.evaluate() << std::endl;

    return 0;
}
