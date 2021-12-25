#include <iostream>
#include "../inc/Parser.h"

int main(int argc, char** argv) {
    std::string test = "2 ^ 3";

    ExpressionHandler myParser(test);

    std::cout << myParser.getPostfixStr() << std::endl;

    std::cout << myParser.evaluate() << std::endl;

    return 0;
}
