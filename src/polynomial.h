#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <iostream>

class Polynomial
{
private:
    double *coeffs_;
    int degree_;

public:
    Polynomial(const double *coeffs, int degree);
    Polynomial(const Polynomial &other);
    void add(const Polynomial &other);
    void mul(const Polynomial &other);
    void print(std::ostream &out) const;
    ~Polynomial();
};

#endif