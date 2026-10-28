#include <iostream>
#include "polynomial.h"

int main()
{
    double c1[] = {1.0, 2.0};      // 1 + 2x
    double c2[] = {0.0, 1.0, 1.0}; // x + x^2
    Polynomial a(c1, 1);
    a.print(std::cout);
    Polynomial b(c2, 2);
    b.print(std::cout);
    Polynomial s(a);
    s.add(b);
    s.print(std::cout);                        // 1 + 3x + x^2
    std::cout << s.evaluate(2.0) << std::endl; // 1 + 6 + 4 = 11
    Polynomial m(a);
    m.mul(b);
    m.print(std::cout);
    return 0;
}