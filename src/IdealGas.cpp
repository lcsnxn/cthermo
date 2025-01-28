#ifndef IDEALGASEOS
#define IDEALGASEOS

#include "../include/EquationOfState.hpp"
#include "../include/GasProperties.hpp"
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>

class IdealGasEOS : public EquationOfState {
    private:
        double R = 8.3145;
        std::vector<GasConstants::GasProperties> gasesProperties;

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
        IdealGasEOS(const std::vector<std::string>& gasNames) {
            loadGasProperties(gasNames);
        }

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
            return 1.0;
        }

        double volume(double pressure, double temperature, const std::vector<double>& moleFractions, UnitBase unit = UnitBase::MASS) const override {
            double Vm = R * temperature / pressure;

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
            int nComponents = moleFractions.size();
            float MW = averageMolarWeight(moleFractions);
            std::vector<double> A(nComponents), B(nComponents), C(nComponents), D(nComponents), Hid(nComponents);
            double T0 = 298.15, Hm = 0.0;

            for (int i = 0; i < nComponents; i++) {
                A[i] = gasesProperties[i].idealGasHeatCapacityPolyCoeffs.at("A");
                B[i] = gasesProperties[i].idealGasHeatCapacityPolyCoeffs.at("B");
                C[i] = gasesProperties[i].idealGasHeatCapacityPolyCoeffs.at("C");
                D[i] = gasesProperties[i].idealGasHeatCapacityPolyCoeffs.at("D");

                Hid[i] = A[i] * temperature + B[i] * temperature * temperature / 2.0 + C[i] * temperature * temperature * temperature / 3.0 + D[i] * temperature * temperature * temperature * temperature / 4.0;
                Hid[i] -= A[i] * T0 + B[i] * T0 * T0 / 2.0 + C[i] * T0 * T0 * T0 / 3.0 + D[i] * T0 * T0 * T0 * T0 / 4.0;
                Hid[i] /= MW;

                Hm += Hid[i] * (moleFractions[i] * gasesProperties[i].molecularWeight / MW);
            }
            return Hm;
            // if (unit == UnitBase::MOLAR) {
            //     return Hm;
            // } else if (unit == UnitBase::MASS) {
            //     return 1e3 * Hm / averageMolarWeight(moleFractions);
            // } else {
            //     throw std::invalid_argument("Unit of measurement not supported.");
            // }

        }


};

#endif