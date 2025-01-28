#include "../include/GasProperties.hpp"
#include "../include/json.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using json = nlohmann::json;

namespace GasConstants {

    std::vector<GasProperties> parseGasProperties(const std::string& filePath) {
        std::ifstream file(filePath);
        double A, B, C, D;

        if (!file.is_open()) {
            throw std::runtime_error("Failed to load the chemsepdb file.");
        }

        json data = json::parse(file);
        std::vector<GasProperties> gases;

        for (const auto& item : data) {
            GasProperties gas;
            
            gas.name = item.value("Name", "");
            gas.CASN = item.value("CASN", "");
            gas.molecularWeight = std::stod(item.value("Molecular weight (kg/kmol)", "0.0"));
            gas.criticalPressure = std::stod(item.value("Critical pressure (Pa)", "0.0"));
            gas.criticalTemperature = std::stod(item.value("Critical temperature (K)", "0.0"));
            gas.criticalVolume = std::stod(item.value("Critical volume (m3/kmol)", "0.0"));
            gas.acentricFactor = std::stod(item.value("Acentric factor (_)", "0.0"));

            try {
                A = std::stod(item.value("A_coeff", "0.0"));
            } catch (const std::invalid_argument& e) {
                A = 0.0;
            }

            try {
                B = std::stod(item.value("B_coeff", "0.0"));
            } catch (const std::invalid_argument& e) {
                B = 0.0;
            }

            try {
                C = std::stod(item.value("C_coeff", "0.0"));
            } catch (const std::invalid_argument& e) {
                C = 0.0;
            }

            try {
                D = std::stod(item.value("D_coeff", "0.0"));
            } catch (const std::invalid_argument& e) {
                D = 0.0;
            }

            gas.idealGasHeatCapacityPolyCoeffs = {
                {"A", A},
                {"B", B},
                {"C", C},
                {"D", D}
            };

            gases.push_back(gas);
        }

        file.close();

        return gases;
    }

    GasProperties getGasProperties(const std::vector<GasProperties>& gases, const std::string& identifier) {
        for (const auto& item : gases) {
            if (item.name == identifier || item.CASN == identifier) {
                return item;
            }
        }
        
        throw std::invalid_argument("Could not find a gas with the identifier " + identifier);
    }

}