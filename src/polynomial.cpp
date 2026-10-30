#include <iostream>
#include <utility>
#include "polynomial.h"
#include <cmath>
#include <algorithm>

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

Polynomial::Polynomial(const Polynomial &other) : degree_(other.degree_), coeffs_(other.degree_ == -1 ? nullptr : new double[other.degree_ + 1])
{
    if (other.coeffs_)
    {
        for (int i = 0; i <= other.degree_; ++i)
        {
            coeffs_[i] = other.coeffs_[i];
        }
    }
}

Polynomial::Polynomial(Polynomial &&other) noexcept
{
    coeffs_ = other.coeffs_;
    degree_ = other.degree_;
    other.coeffs_ = nullptr;
    other.degree_ = -1;
}

Polynomial &Polynomial::operator=(Polynomial other)
{
    std::swap(coeffs_, other.coeffs_);
    std::swap(degree_, other.degree_);
    return *this;
}

void Polynomial::add(const Polynomial &other)
{
    if (!other.coeffs_ || other.degree_ == -1)
        return;
    int new_degree = std::max(degree_, other.degree_);
    double *temp = new double[new_degree + 1]();
    if (coeffs_)
    {
        for (int i = 0; i <= degree_; ++i)
        {
            temp[i] += coeffs_[i];
        }
    }

    for (int i = 0; i <= other.degree_; ++i)
    {
        temp[i] += other.coeffs_[i];
    }
    delete[] coeffs_;
    coeffs_ = temp;
    degree_ = new_degree;
}

void Polynomial::mul(const Polynomial &other)
{
    if (!coeffs_ || degree_ == -1)
        return;
    if (!other.coeffs_ || other.degree_ == -1)
    {
        degree_ = -1;
        delete[] coeffs_;
        coeffs_ = nullptr;
        return;
    }
    int old_degree = degree_;
    degree_ = degree_ + other.degree_;
    double *new_coeffs = new double[degree_ + 1]();
    for (size_t i = 0; i <= old_degree; ++i)
    {
        for (size_t j = 0; j <= other.degree_; ++j)
        {
            double coef = coeffs_[i] * other.coeffs_[j];
            new_coeffs[i + j] += coef;
        }
    }
    delete[] coeffs_;
    coeffs_ = new_coeffs;
}

double Polynomial::evaluate(double x) const
{
    if (!coeffs_ || degree_ == -1)
        return 0;
    {
        double res = coeffs_[0];
        for (size_t i = 1; i <= degree_; ++i)
        {
            res += coeffs_[i] * pow(x, i);
        }
        return res;
    }
    return 0;
}

double Polynomial::definite_integral(double a, double b) const
{
    if (!coeffs_ || degree_ == -1)
        return 0;
    double res = 0;
    for (size_t i = 0; i <= degree_; ++i)
    {
        double coeff = coeffs_[i] * 1 / (double)(i + 1);
        res += coeff * pow(b, i + 1) - coeff * pow(a, i + 1);
    }
    return res;
}
void Polynomial::print(std::ostream &out) const
{
    if (!coeffs_ || degree_ == -1)
        return;
    for (size_t i = 0; i <= degree_; ++i)
    {
        if (i > 0 && coeffs_[i] >= 0)
        {
            out << " + ";
        }
        else if (coeffs_[i] < 0)
        {
            out << " - ";
        }
        if (i != 0 && std::abs(coeffs_[i]))
            std::cout << "";
        if (i == 0)
        {
            out << std::abs(coeffs_[i]);
            continue;
        }
        if (i != 0 && coeffs_[i] != 1)
            out << std::abs(coeffs_[i]);
        if (i == 1)
        {
            out << "x";
            continue;
        }
        if (i == 0)
            continue;
        out << "x^" << i;
    }
    out << "\n";
}

Polynomial::~Polynomial()
{
    delete[] coeffs_;
}