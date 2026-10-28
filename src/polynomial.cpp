#include <iostream>
#include <utility>
#include "polynomial.h"

Polynomial::Polynomial(const double *coeffs, int degree) : coeffs_(degree == -1 ? nullptr : new double[degree + 1]), degree_(degree)
{
    if (coeffs && coeffs_)
    {
        for (int i = 0; i <= degree; ++i)
        {
            coeffs_[i] = coeffs[i];
        }
    }
}

Polynomial::Polynomial(const Polynomial &other) : coeffs_(other.degree_ == -1 ? nullptr : new double[other.degree_ + 1])
{
    if (other.coeffs_)
    {
        for (int i = 0; i <= other.degree_; ++i)
        {
            coeffs_[i] = other.coeffs_[i];
        }
    }
}

Polynomial &Polynomial::operator=(Polynomial other)
{
    std::swap(coeffs_, other.coeffs_);
    std::swap(degree_, other.degree_);
    return *this;
}

Polynomial::Polynomial(Polynomial &&other)
{
    coeffs_ = other.coeffs_;
    degree_ = other.degree_;
    other.coeffs_ = nullptr;
    other.degree_ = -1;
}

Polynomial::~Polynomial()
{
    delete[] coeffs_;
}