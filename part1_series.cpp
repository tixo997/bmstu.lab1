// Лабораторная работа №1, часть 1, вариант 1.
// Ряд Лейбница: pi = 4 * (1 - 1/3 + 1/5 - 1/7 + ...).
// Точное значение pi не используется при суммировании и остановке циклов.
#include <cmath>
#include <cstddef>
#include <iomanip>
#include <iostream>

struct Result
{
    double sum;
    std::size_t terms;
};

// После n слагаемых модуль ошибки не превосходит первого отброшенного
// слагаемого: |pi - S_n| <= 4 / (2*n + 1).
double errorBound(std::size_t n)
{
    return 4.0 / (2.0 * n + 1.0);
}

Result sumWhile(double epsilon)
{
    Result result{0.0, 0};
    int sign = 1;
    while (errorBound(result.terms) > epsilon)
    {
        result.sum += sign * errorBound(result.terms);
        ++result.terms;
        sign = -sign;
    }
    return result;
}

// Во всех заданных случаях epsilon < 4, поэтому нужно хотя бы одно слагаемое.
Result sumDoWhile(double epsilon)
{
    Result result{0.0, 0};
    int sign = 1;
    do
    {
        result.sum += sign * errorBound(result.terms);
        ++result.terms;
        sign = -sign;
    } while (errorBound(result.terms) > epsilon);
    return result;
}

Result sumFor(double epsilon)
{
    Result result{0.0, 0};
    for (int sign = 1; errorBound(result.terms) > epsilon;
         ++result.terms, sign = -sign)
    {
        result.sum += sign * errorBound(result.terms);
    }
    return result;
}

void printResult(const char* loop, double epsilon, const Result& result)
{
    std::cout << std::left << std::setw(12) << loop
              << std::scientific << std::setprecision(0)
              << std::setw(12) << epsilon
              << std::setw(12) << result.terms
              << std::fixed << std::setprecision(12)
              << std::setw(20) << result.sum
              << std::scientific << std::setprecision(8)
              << errorBound(result.terms) << '\n';
}

int main()
{
    const double epsilons[] = {1e-2, 1e-4, 1e-6};
    std::cout << "Leibniz series: pi = 4 * (1 - 1/3 + 1/5 - ... )\n"
              << "Stop when 4 / (2*N + 1) <= epsilon.\n\n"
              << std::left << std::setw(12) << "Loop"
              << std::setw(12) << "Epsilon"
              << std::setw(12) << "Terms"
              << std::setw(20) << "Approximation"
              << "Error bound\n";

    for (double epsilon : epsilons)
    {
        printResult("while", epsilon, sumWhile(epsilon));
        printResult("do-while", epsilon, sumDoWhile(epsilon));
        printResult("for", epsilon, sumFor(epsilon));
        std::cout << '\n';
    }

    // Справочное значение вычисляется и печатается только ПОСЛЕ результатов.
    // Это машинное приближение к точному значению, а не критерий остановки.
    std::cout << std::fixed << std::setprecision(15)
              << "Reference pi = " << std::acos(-1.0) << '\n';
    return 0;
}
