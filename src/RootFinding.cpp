// RootFinding.cpp
// Implementation of root finding algorithms
#include "../include/RootFinding.hpp"
#include <algorithm>
#include <cmath>
#include <complex>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <vector>

namespace RootFind {

    std::complex<double> Laguerre(const std::vector<std::complex<double>>& a, std::complex<double>& x) {
        const int MR = 8, MT = 10, MAXIT = MT * MR;
        const double EPS = std::numeric_limits<double>::epsilon();
        static const double frac[MR + 1] = {0.0, 0.5, 0.25, 0.75, 0.13, 0.38, 0.62, 0.88, 1.0};
        std::complex<double> dx, x1, b, d, f, g, h, sq, gp, gm, g2;
        int m = a.size() - 1;

        for (int i = 1; i < MAXIT; i++) {
            b = a[m];
            double err = abs(b);
            d = f = 0.0;
            double abx = abs(x);

            for (int j = m - 1; j >= 0; j--) {
                f = x * f + d;
                d = x * d + b;
                b = x * b + a[j];
                err = abs(b) + abx * err;
            }
            
            err *= EPS;

            if (abs(b) <= err) return x;
            
            g = d / b;
            g2 = g * g;
            h = g2 - 2.0 * f / b;
            sq = sqrt((double)(m - 1) * ((double) m * h - g2));
            gp = g + sq;
            gm = g - sq;
            
            double abp = abs(gp);
            double abm = abs(gm);
            if (abp < abm) gp = gm;
            dx = std::max(abp, abm) > 0.0 ? (double) m / gp : std::polar(1 + abx, (double) i);
            
            x1 = x - dx;
            if (x == x1) return x;
            if (i % MT != 0) x = x1;
            else x -= frac[i / MT] * dx;
        }
        
        throw std::runtime_error("Method did not converge within the maximum number of iterations.");
    }

    std::vector<std::complex<double>> roots(const std::vector<std::complex<double>>& a) {
        const double EPS = 1.0e-14;
        int i;
        std::complex<double> x, b, c;
        int m = a.size() - 1;
        std::vector<std::complex<double>> ad(m + 1), results(m);

        for (int j = 0; j <= m; j++) ad[j] = a[j];

        for (int j = m - 1; j >= 0; j--) {
            x = 0.0;
            std::vector<std::complex<double>> ad_v(j + 2);

            for (int k = 0; k < j + 2; k++) ad_v[k] = ad[k];

            x = Laguerre(ad_v, x);

            if (abs(std::imag(x)) <= 2.0 * EPS * abs(std::real(x))) {
                x = std::complex<double>(std::real(x), 0.0);
            }

            results[j] = x;
            b = ad[j + 1];

            for (int k = j; k >= 0; k--) {
                c = ad[k];
                ad[k] = b;
                b = x * b + c;
            }
        }

        for (int j = 0; j < m; j++) results[j] = Laguerre(a, results[j]);

        for (int j = 1; j < m; j++) {
            x = results[j];
            for (i = j - 1; i >= 0; i--) {
                if (std::real(results[i]) <= std::real(x)) break;
                results[i + 1] = results[i];
            }
            results[i + 1] = x;
        }

        return results;
    }
}

