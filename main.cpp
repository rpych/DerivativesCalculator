#include <iostream>
#include "Parser.h"

int main() {
    // Examples:
    //     -2x
    //     (-x)^4
    //     (-16x)^(-3)
    //     (((2x^4 -3x)/(x-14))^3) * (3x-5)
    //     (2x^3 -3x)*((5/3)x^2 -1)
    //     ((2x^3-1)*(11x^2-5x))^(-1/2)
    //     (5)*(13x^5 - 21x^3 + 10x)
    

    Parser parser;
    parser.takeExpression();


    return 0;
}