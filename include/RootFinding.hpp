// RootFinding.hpp
// Algorithms to find roots of a complex polynomial
#ifndef ROOTFINDING
#define ROOTFINDING

#include <stdexcept>
#include <vector>
#include <complex>

namespace RootFind {
    
    // Function for finding the root of a polynomial using Laguerre's method
    std::complex<double> Laguerre(const std::vector<std::complex<double>>& a, std::complex<double>& x);

    // Function for finding all complex roots of a polynomial
    std::vector<std::complex<double>> roots(const std::vector<std::complex<double>>& a);

}

#endif
