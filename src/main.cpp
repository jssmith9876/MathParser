#include <iostream>
#include "../inc/Parser.h"

int main() {
    std::string test = "3 + 4";

    Parser myParser(test);

    std::cout << myParser.getPostfixStr() << std::endl;


    return 0;
}
