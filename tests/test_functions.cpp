#include <gtest/gtest.h>

#include <sstream>
#include <string>
#include <utility>

#include "./src/polynomial.h"

namespace
{
    std::string to_string(const Polynomial &p)
    {
        std::ostringstream os;
        p.print(os);
        return os.str();
    }

    constexpr double kEps = 1e-9;
}
TEST(PolynomialBasic, ConstructorStoresDegreeAndCoefficients)
{
    double c[] = {1.0, 2.0, 3.0}; // 1 + 2x + 3x^2
    Polynomial p(c, 2);
    EXPECT_EQ(p.return_degree(), 2);
    EXPECT_NEAR(p.evaluate(2.0), 17.0, kEps); // 1 + 4 + 12
}

TEST(PolynomialBasic, EmptyObjectHasDegreeMinusOne)
{
    Polynomial p(nullptr, -1);
    EXPECT_EQ(p.return_degree(), -1);
}

TEST(PolynomialBasic, SingleElementConstant)
{
    double c[] = {5.0};
    Polynomial p(c, 0);
    EXPECT_EQ(p.return_degree(), 0);
    EXPECT_NEAR(p.evaluate(0.0), 5.0, kEps);
    EXPECT_NEAR(p.evaluate(100.0), 5.0, kEps);
    EXPECT_EQ(to_string(p), "5\n");
}

TEST(PolynomialBasic, EvaluateNegativeArgument)
{
    double c[] = {1.0, -1.0, 1.0}; // 1 - x + x^2
    Polynomial p(c, 2);
    EXPECT_NEAR(p.evaluate(-2.0), 7.0, kEps); // 1 + 2 + 4
}

TEST(PolynomialBasic, PrintFormat)
{
    double c[] = {1.0, 2.0, 3.0};
    Polynomial p(c, 2);
    EXPECT_EQ(to_string(p), "1 + 2x + 3x^2\n");
}

TEST(PolynomialBasic, AddSameDegree)
{
    double c1[] = {1.0, 2.0, 3.0};
    double c2[] = {4.0, 5.0, 6.0};
    Polynomial a(c1, 2);
    Polynomial b(c2, 2);
    a.add(b);
    EXPECT_EQ(a.return_degree(), 2);
    EXPECT_EQ(to_string(a), "5 + 7x + 9x^2\n");
    EXPECT_NEAR(a.evaluate(1.0), 21.0, kEps);
}

TEST(PolynomialBasic, MulDifferentDegrees)
{
    double c1[] = {1.0, 2.0};      // 1 + 2x
    double c2[] = {0.0, 1.0, 1.0}; // x + x^2
    Polynomial a(c1, 1);
    Polynomial b(c2, 2);
    a.mul(b); // x + 3x^2 + 2x^3
    EXPECT_EQ(a.return_degree(), 3);
    EXPECT_EQ(to_string(a), "0 + x + 3x^2 + 2x^3\n");
    EXPECT_NEAR(a.evaluate(2.0), 30.0, kEps); // 2 + 12 + 16
}

TEST(PolynomialBasic, MulByConstantScales)
{
    double c1[] = {1.0, 2.0, 3.0};
    double c2[] = {2.0};
    Polynomial a(c1, 2);
    Polynomial two(c2, 0);
    a.mul(two);
    EXPECT_EQ(a.return_degree(), 2);
    EXPECT_EQ(to_string(a), "2 + 4x + 6x^2\n");
}

TEST(PolynomialCopyMove, CopyConstructorIsDeepCopy)
{
    double c1[] = {1.0, 2.0};
    double c2[] = {10.0, 20.0};
    Polynomial original(c1, 1);
    Polynomial copy(original);
    copy.add(Polynomial(c2, 1));

    EXPECT_EQ(to_string(copy), "11 + 22x\n");
    EXPECT_EQ(to_string(original), "1 + 2x\n"); // оригинал не изменился
}

TEST(PolynomialCopyMove, CopyOfEmpty)
{
    Polynomial empty(nullptr, -1);
    Polynomial copy(empty);
    EXPECT_EQ(copy.return_degree(), -1);
    EXPECT_EQ(empty.return_degree(), -1);
}

TEST(PolynomialCopyMove, MoveConstructorTransfersAndEmptiesSource)
{
    double c[] = {1.0, 2.0, 3.0};
    Polynomial src(c, 2);
    Polynomial dst(std::move(src));

    EXPECT_EQ(dst.return_degree(), 2);
    EXPECT_EQ(to_string(dst), "1 + 2x + 3x^2\n");
    EXPECT_EQ(src.return_degree(), -1); // NOLINT: проверяем состояние после move
}

TEST(PolynomialCopyMove, MoveOfEmpty)
{
    Polynomial empty(nullptr, -1);
    Polynomial moved(std::move(empty));
    EXPECT_EQ(moved.return_degree(), -1);
}

TEST(PolynomialCopyMove, CopyAssignmentReplacesContent)
{
    double c1[] = {1.0, 2.0};
    double c2[] = {7.0, 8.0, 9.0};
    Polynomial a(c1, 1);
    Polynomial b(c2, 2);
    a = b;

    EXPECT_EQ(a.return_degree(), 2);
    EXPECT_EQ(to_string(a), "7 + 8x + 9x^2\n");
    EXPECT_EQ(to_string(b), "7 + 8x + 9x^2\n"); // источник не тронут
}

TEST(PolynomialCopyMove, MoveAssignment)
{
    double c1[] = {1.0};
    double c2[] = {3.0, 4.0};
    Polynomial a(c1, 0);
    Polynomial b(c2, 1);
    a = std::move(b);

    EXPECT_EQ(a.return_degree(), 1);
    EXPECT_EQ(to_string(a), "3 + 4x\n");
}

TEST(PolynomialCopyMove, SelfAssignmentKeepsValue)
{
    double c[] = {1.0, 2.0, 3.0};
    Polynomial p(c, 2);
    Polynomial &ref = p;
    p = ref;

    EXPECT_EQ(p.return_degree(), 2);
    EXPECT_EQ(to_string(p), "1 + 2x + 3x^2\n");
}

TEST(PolynomialCopyMove, SelfAssignmentOfEmpty)
{
    Polynomial p(nullptr, -1);
    Polynomial &ref = p;
    p = ref;
    EXPECT_EQ(p.return_degree(), -1);
}

TEST(PolynomialCopyMove, AssignEmptyToNonEmptyAndBack)
{
    double c[] = {1.0, 2.0};
    Polynomial p(c, 1);
    Polynomial empty(nullptr, -1);

    p = empty;
    EXPECT_EQ(p.return_degree(), -1);

    Polynomial q(c, 1);
    empty = q;
    EXPECT_EQ(empty.return_degree(), 1);
    EXPECT_EQ(to_string(empty), "1 + 2x\n");
}

TEST(PolynomialIntegral, Constant)
{
    double c[] = {3.0};
    Polynomial p(c, 0);
    EXPECT_NEAR(p.definite_integral(0.0, 2.0), 6.0, kEps);
}

TEST(PolynomialIntegral, Linear)
{
    double c[] = {1.0, 2.0}; // 1 + 2x, int 0..1 = 1 + 1
    Polynomial p(c, 1);
    EXPECT_NEAR(p.definite_integral(0.0, 1.0), 2.0, kEps);
}

TEST(PolynomialIntegral, Quadratic)
{
    double c[] = {0.0, 0.0, 1.0}; // x^2, int 0..3 = 9
    Polynomial p(c, 2);
    EXPECT_NEAR(p.definite_integral(0.0, 3.0), 9.0, kEps);
}

TEST(PolynomialIntegral, ZeroLengthInterval)
{
    double c[] = {1.0, 2.0, 3.0};
    Polynomial p(c, 2);
    EXPECT_NEAR(p.definite_integral(1.5, 1.5), 0.0, kEps);
}

TEST(PolynomialIntegral, ReversedBoundsChangeSign)
{
    double c[] = {1.0, 2.0, 3.0};
    Polynomial p(c, 2);
    EXPECT_NEAR(p.definite_integral(0.0, 2.0), -p.definite_integral(2.0, 0.0), kEps);
}

TEST(PolynomialIntegral, OddFunctionOnSymmetricInterval)
{
    double c[] = {0.0, 1.0}; // x, int -1..1 = 0
    Polynomial p(c, 1);
    EXPECT_NEAR(p.definite_integral(-1.0, 1.0), 0.0, kEps);
}

TEST(PolynomialIntegral, NegativeCoefficients)
{
    double c[] = {-1.0, 0.0, -3.0}; // -1 - 3x^2, int 0..1 = -1 - 1
    Polynomial p(c, 2);
    EXPECT_NEAR(p.definite_integral(0.0, 1.0), -2.0, kEps);
}

TEST(PolynomialIntegral, AdditiveOverInterval)
{
    double c[] = {2.0, -1.0, 0.5, 4.0};
    Polynomial p(c, 3);
    double whole = p.definite_integral(-1.0, 3.0);
    double parts = p.definite_integral(-1.0, 1.0) + p.definite_integral(1.0, 3.0);
    EXPECT_NEAR(whole, parts, 1e-9);
}

TEST(PolynomialIntegral, IntegralOfSumEqualsSumOfIntegrals)
{
    double c1[] = {1.0, 2.0, 3.0};
    double c2[] = {4.0, 5.0, 6.0};
    Polynomial a(c1, 2);
    Polynomial b(c2, 2);
    double expected = a.definite_integral(0.0, 2.0) + b.definite_integral(0.0, 2.0);
    a.add(b);
    EXPECT_NEAR(a.definite_integral(0.0, 2.0), expected, 1e-9);
}

TEST(PolynomialEmpty, EvaluateEmptyReturnsZero)
{
    Polynomial empty(nullptr, -1);
    EXPECT_NEAR(empty.evaluate(5.0), 0.0, kEps);
}

TEST(PolynomialEmpty, IntegralOfEmptyIsZero)
{
    Polynomial empty(nullptr, -1);
    EXPECT_NEAR(empty.definite_integral(0.0, 1.0), 0.0, kEps);
}

TEST(PolynomialEmpty, PrintEmptyDoesNotCrash)
{
    Polynomial empty(nullptr, -1);
    EXPECT_NO_FATAL_FAILURE(to_string(empty));
}

TEST(PolynomialEmpty, AddNonEmptyToEmpty)
{
    double c[] = {1.0, 2.0};
    Polynomial empty(nullptr, -1);
    Polynomial p(c, 1);
    empty.add(p);
    EXPECT_EQ(empty.return_degree(), 1);
    EXPECT_EQ(to_string(empty), "1 + 2x\n");
}

TEST(PolynomialEmpty, AddEmptyToNonEmpty)
{
    double c[] = {1.0, 2.0};
    Polynomial p(c, 1);
    Polynomial empty(nullptr, -1);
    p.add(empty);
    EXPECT_EQ(p.return_degree(), 1);
    EXPECT_EQ(to_string(p), "1 + 2x\n");
}

TEST(PolynomialEmpty, AddHigherDegreeToLowerDegree)
{
    double c1[] = {1.0, 2.0};
    double c2[] = {0.0, 1.0, 1.0};
    Polynomial a(c1, 1);
    Polynomial b(c2, 2);
    a.add(b);
    EXPECT_EQ(a.return_degree(), 2);
    EXPECT_EQ(to_string(a), "1 + 3x + x^2\n");
}

TEST(PolynomialEmpty, AddLowerDegreeToHigherDegree)
{
    double c1[] = {0.0, 1.0, 1.0};
    double c2[] = {1.0, 2.0};
    Polynomial a(c1, 2);
    Polynomial b(c2, 1);
    a.add(b);
    EXPECT_EQ(a.return_degree(), 2);
    EXPECT_EQ(to_string(a), "1 + 3x + x^2\n");
}

TEST(PolynomialEmpty, MulByEmptyGivesEmpty)
{
    double c[] = {1.0, 2.0};
    Polynomial p(c, 1);
    Polynomial empty(nullptr, -1);
    p.mul(empty);
    EXPECT_EQ(p.return_degree(), -1);
}

TEST(PolynomialEmpty, EmptyMulNonEmptyGivesEmpty)
{
    double c[] = {1.0, 2.0};
    Polynomial empty(nullptr, -1);
    Polynomial p(c, 1);
    empty.mul(p);
    EXPECT_EQ(empty.return_degree(), -1);
}