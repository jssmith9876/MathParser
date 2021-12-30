# MathParser
A parser that evaluates math expressions provided as an infix string (ex. 3 + 4 * 6) and returns the result of the expression. 

The purpose of this code is to provide people create mathematical functions to be able to accept variable functions and expressions from the user to evaluate rather than a hard-coded function.

Given some infix string, we first parse it into a postfix-equivalent queue. This queue holds token representations of the elements in the expression. We translate the infix string to the postfix queue using the Shunting-Yard algorithm (https://en.wikipedia.org/wiki/Shunting-yard_algorithm). Once we have parsed our infix string to the postfix queue, we then evaluate the expressing using stack evaluation. 

## Current Features
This expression parser not only can handle simple mathematical expressions (ex. 3 + 4 * 6), but can also evaluate mathematical functions such as trig functions such as sin, cos, tan, etc. As well, this parser handles free variables in the function, provided that they are single-character and are provieded by the user.

## Things to add
- I do not believe that the parser currently handles negative numbers in the provided infix string. Something important to add
- Other mathematical functions
- Being able to use strings for free variables rather than only single characters
- Work on better error handling and passing more useful error messages to the user

## Code examples

#### Evaluation of a simple expression
```
std::string simpleExample = "3 + 4 * 2 / (1 - 5) ^ 2 ^ 3";

ExpressionHandler myParser(simpleExample);

// To show the postfix equivalent of the expression
std::cout << "Postfix equivalent: " << myParser.getPostfixStr() << std::endl;

// To output the result of the expression
std::cout << "Result: " << myParser.evaluate() << std::endl;
```

The above code will output
```
Postfix equivalent: 3.000000 4.000000 2.000000 * 1.000000 5.000000 - 2.000000 3.000000 ^ ^ / +
Result: 3.00012
```

#### Evaluation of expresions involving mathematical functions
```
std::string functionExample = "sin(max(2, 3) / 3 * 3.1415)";

ExpressionHandler myParser(functionExample);

// To show the postfix equivalent of the expression
std::cout << "Postfix equivalent: " << myParser.getPostfixStr() << std::endl;

// To output the result of the expression
std::cout << "Result: " << myParser.evaluate() << std::endl;
```
The above code will output 
```
Postfix equivalent: 2.000000 3.000000 max 3.000000 / 3.141500 * sin
Result: 9.26536e-05
```

#### Evaluation of expressions involving free variables
```
std::vector<char> freeVars { 'x', 'y', 'z' };
std::string variableExample = "cos(x) + 3*y - 2 ^ z";

ExpressionHandler myParser(freeVars, variableExample);

// To assign values to the free variables
std::map<char, double> valueMap;
valueMap['x'] = 3.1415;
valueMap['y'] = 1.6;
valueMap['z'] = 3.9;

// To show the postfix equivalent of the expression
std::cout << "Postfix equivalent: " << myParser.getPostfixStr() << std::endl;

// To output the result of the expression
std::cout << "Result: " << myParser.evaluate(valueMap) << std::endl;
```

The above code will output
```
Postfix equivalent: x cos 3.000000 y * + 2.000000 z ^ -
Result: -11.1285
```


