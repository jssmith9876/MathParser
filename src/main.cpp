#include <iostream>
#include "../inc/Parser.h"

int main(int argc, char** argv) {
    // Create an infix string to parse through
    std::string test = "x + 3*y + 2";

    std::vector<char> freeVars {'x', 'y'};

    ExpressionHandler myParser(freeVars, test);

    std::map<char, double> valueMap;
    valueMap['x'] = 1.0;
    valueMap['y'] = 6.0;

    std::cout << myParser.evaluate(valueMap) << std::endl;

    return 0;
}
