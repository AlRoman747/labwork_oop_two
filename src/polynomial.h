#include <iostream>

class Polynomial
{
private:
    double *coeffs_;
    int degree_;

public:
    Polynomial(const double *coeffs, int degree);
    Polynomial(const Polynomial &other);
    Polynomial(Polynomial &&other) noexcept;
    Polynomial &operator=(Polynomial other);
    void add(const Polynomial &other);
    void mul(const Polynomial &other);
    double evaluate(double x) const;
    double definite_integral(double a, double b) const;
    void print(std::ostream &out) const;
    ~Polynomial();
    int return_degree();
};