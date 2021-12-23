#include <iostream>
#include "../inc/Parser.h"

int main() {
    std::string test = "31 + 4 * 2 / (1 - 5) ^ 2 ^ 3";

    Parser myParser(test);

    std::cout << myParser.getPostfixStr() << std::endl;


    return 0;
}
