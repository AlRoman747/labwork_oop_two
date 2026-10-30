#include <iostream>
#include "polynomial.h"

int main()
{
    double c[] = {1.0, 2.0};
    Polynomial p(c, 1);
    Polynomial empty(nullptr, -1);
    p.mul(empty);
    std::cout << p.return_degree();
    return 0;
}