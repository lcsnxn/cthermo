#ifndef PENGROBINSONEOS
#define PENGROBINSONEOS

#include "../include/EquationOfState.hpp"
#include "../include/GasProperties.hpp"
#include "../include/InteractionParameters.hpp"
#include "../include/RootFinding.hpp"
#include <cmath>
#include <complex>
#include <iostream>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <vector>
#include <map>

class PengRobinsonEOS : public EquationOfState {
private:
    double R = 8.3145;
    bool volumeTranslation;
    std::map<std::string, double> volumeTranslationCoeffs = {
        {"Nitrogen", -0.1927},
        {"Carbon dioxide", -0.0817},
        {"Methane", -0.1595},
        {"Ethane", -0.1134}, 
        {"Propane", -0.0863},
        {"Isobutane", -0.0844},
        {"N-butane", -0.0675},
        {"Isopentane", -0.0608},
        {"N-pentane", -0.039},
        {"N-hexane", -0.008},
        {"N-heptane", 0.0033}
    };
    std::vector<GasConstants::GasProperties> gasesProperties;
    std::vector<std::vector<double>> kij;

    void loadInteractionParameters(const std::vector<std::string>& gasNames){
        std::filesystem::path filePath = "utils/databases/pripdb.json";
        auto gasesIPs = BinaryIPs::parseInteractionParameters(filePath.string());
        double k12;
        std::vector<double> aux;
        
        for (const std::string& gasName1 : gasNames) {
            for (const std::string& gasName2 : gasNames) {
                if (gasName1 == gasName2) {
                    k12 = 0.0;
                } else {
                    try {
                        auto dataIP = BinaryIPs::getInteractionParameters(gasesIPs, gasName1, gasName2);
                        k12 = dataIP.k12;
                    }
                    catch (const std::invalid_argument& e) {
                        std::cerr << "WARNING: " << e.what() << " Returning 0.0 instead." << std::endl;
                        k12 = 0.0;
                    }
                }
                aux.push_back(k12);
            }
            kij.push_back(aux);
            aux.clear();
        }
    }

    void loadGasProperties(const std::vector<std::string>& gasNames) {
        std::filesystem::path filePath = "utils/databases/chemsepdb.json";
        auto gases = GasConstants::parseGasProperties(filePath.string());
        GasConstants::GasProperties gas;
        
        for (const std::string& gasName : gasNames) {
            gas = GasConstants::getGasProperties(gases, gasName);
            gasesProperties.push_back(gas);
        }
    }

public:
    PengRobinsonEOS(const std::vector<std::string>& gasNames, const bool withVolumeTranslation = true) : volumeTranslation(withVolumeTranslation) {
        loadGasProperties(gasNames);
        loadInteractionParameters(gasNames);
    }

    std::vector<std::vector<double>> getKIJ() { return kij; }

    std::vector<GasConstants::GasProperties> getGasesProperties() { return gasesProperties; }

    double averageMolarWeight(const std::vector<double>& moleFractions) const override {
        double mW = 0.0;
        int nComponents = moleFractions.size();

        for (int i = 0; i < nComponents; i++) {
            mW += moleFractions[i] * gasesProperties[i].molecularWeight;
        }

        return mW;
    }

    double compressibilityFactor(double pressure, double temperature, const std::vector<double>& moleFractions) const override {
        int nComponents = moleFractions.size();
        double omega, Pc, Tc, A, B, a_mix = 0.0, b_mix = 0.0;
        std::vector<double> alpha(nComponents, 0.0), a(nComponents, 0.0), b(nComponents, 0.0);
        std::vector<std::complex<double>> coeffs(4, 0.0);
        std::vector<std::vector<double>> aux_mix(nComponents, std::vector<double>(nComponents, 0.0));

        for (int i = 0; i < nComponents; i++) {
            omega = gasesProperties[i].acentricFactor;
            Pc = gasesProperties[i].criticalPressure;
            Tc = gasesProperties[i].criticalTemperature;

            alpha[i] = (0.37464 + 1.54226 * omega - 0.26992 * omega * omega);
            alpha[i] *= (1.0 - sqrt(temperature / Tc));
            alpha[i] += 1.0;
            alpha[i] *= alpha[i];

            a[i] = 0.45724 * alpha[i] * R * R * Tc * Tc / Pc;
            b[i] = 0.0778 * R * Tc / Pc;
        }

        for (int i = 0; i < nComponents; i++) {
            for (int j = 0; j < nComponents; j++) {
                aux_mix[i][j] = sqrt(a[i] * a[j]) * (1.0 - kij[i][j]);
            }
        }

        for (int i = 0; i < nComponents; i++) {
            for (int j = 0; j < nComponents; j++) {
                a_mix += moleFractions[i] * moleFractions[j] * aux_mix[i][j];
            }
        }

        for (int i = 0; i < nComponents; i++) {
            b_mix += moleFractions[i] * b[i];
        }

        A = a_mix * pressure / (R * R * temperature * temperature);
        B = b_mix * pressure / (R * temperature);

        coeffs[0] = -A * B + B * B + B * B * B;
        coeffs[1] = A - 3.0 * B * B - 2.0 * B;
        coeffs[2] = B - 1.0;
        coeffs[3] = 1.0;

        std::vector<std::complex<double>> z_roots = RootFind::roots(coeffs);

        double Z = 0.0;
        for (const auto& z_root : z_roots) {
            if (z_root.real() > Z) {
                Z = z_root.real();
            }
        }

        if (volumeTranslation) {
            double vtc, aux = 0.0;
            for (int i = 0; i < nComponents; i++) {
                try {
                    vtc = volumeTranslationCoeffs.at(gasesProperties[i].name);
                } catch (const std::invalid_argument& e) {
                    std::cerr << "WARNING: " << e.what() << " Returning 0.0 instead." << std::endl;
                    vtc = 0.0;
                }
                Pc = gasesProperties[i].criticalPressure;
                Tc = gasesProperties[i].criticalTemperature;
                aux += moleFractions[i] * vtc * 0.0778 * 8.314 * Tc / Pc; 
            }

            Z -= aux / R / temperature * pressure;
        }
        
        return Z;
    }

    double volume(double pressure, double temperature, const std::vector<double>& moleFractions, UnitBase unit = UnitBase::MASS) const override {
        double Z = compressibilityFactor(pressure, temperature, moleFractions);
        double Vm = Z * R * temperature / pressure;

        if (unit == UnitBase::MOLAR) {
            return Vm;
        } else if (unit == UnitBase::MASS) {
            return 1e3 * Vm / averageMolarWeight(moleFractions);
        } else {
            throw std::invalid_argument("Unit of measurement not supported.");
        }
    }

    double density(double pressure, double temperature, const std::vector<double>& moleFractions, UnitBase unit = UnitBase::MASS) const override {
        double V = volume(pressure, temperature, moleFractions, unit);

        return 1.0 / V;

    }

    double enthalpy(double pressure, double temperature, const std::vector<double>& moleFractions, UnitBase unit) const override {
        
    }

};

#endif