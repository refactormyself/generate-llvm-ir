#include <iostream>
#include "include/header.h"

int main() {
    std::cout << "squares of 34 : " << square(34) << std::endl;
    std::cout << "sum of squares of 3 and 4 : " << sumsquares(3, 4) << std::endl;
    std::cout << "factorial of five is :" << fivefactorial() << std::endl;
    std::cout << "factorial of five is :" << fivefactorialnofold() << std::endl;
}