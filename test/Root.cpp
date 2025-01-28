#include "../include/RootFinding.hpp"
#include <iostream>
#include <cmath>
#include <vector>
#include <complex>

int main(int argc, char* argv[])
{
    std::vector<std::complex<double>> a1 = {1.0, 2.0, 1.0};
    std::vector<std::complex<double>> a2 = {2.0, -3.0, 1.0};
    std::vector<std::complex<double>> a3 = {1.0, 0.0, 1.0};
    std::vector<std::complex<double>> a4 = {-6.0, 11.0, -6.0, 1};
    
    std::vector<std::complex<double>> roots1 = RootFind::roots(a1);
    std::vector<std::complex<double>> roots2 = RootFind::roots(a2);
    std::vector<std::complex<double>> roots3 = RootFind::roots(a3);
    std::vector<std::complex<double>> roots4 = RootFind::roots(a4);

    std::cout << "The roots of x^2 + 2x + 1 are: ";
    for (const auto& r : roots1) {
        std::cout << r << ", ";
    }
    std::cout << "\n";
    std::cout << "The roots of x^2 + -3x + 2 are: ";
    for (const auto& r : roots2) {
        std::cout << r << ", ";
    }
    std::cout << "\n";
    std::cout << "The roots of x^2 + 1 are: ";
    for (const auto& r : roots3) {
        std::cout << r << ", ";
    }
    std::cout << "\n";
    std::cout << "The roots of x^3 - 6x^2 + 11x - 6 are: ";
    for (const auto& r : roots4) {
        std::cout << r << ", ";
    }
    std::cout << "\n";
}