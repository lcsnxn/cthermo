#include "../src/GasProperties.cpp"
#include "../src/InteractionParameters.cpp"
#include <iostream>
#include <string>

int main() {
    auto gases = GasConstants::parseGasProperties("../utils/databases/chemsepdb.json");
    auto gasesIPs = BinaryIPs::parseInteractionParameters("../utils/databases/pripdb.json");
    std::string identifier;
    std::string id_1, id_2;

    std::cout << "Enter ID: ";
    std::getline(std::cin, identifier);

    auto gas = getGasProperties(gases, identifier);

    std::cout << gas.name << "\n";
    std::cout << gas.CASN << "\n";
    std::cout << gas.criticalPressure << "\n";
    std::cout << gas.criticalTemperature << "\n";
    std::cout << gas.molecularWeight << "\n";
    std::cout << gas.idealGasHeatCapacityPolyCoeffs.at("A") << "\n";
    std::cout << gas.idealGasHeatCapacityPolyCoeffs.at("B") << "\n";
    std::cout << gas.idealGasHeatCapacityPolyCoeffs.at("C") << "\n";
    std::cout << gas.idealGasHeatCapacityPolyCoeffs.at("D") << "\n";

    std::cout << "Enter ID1: ";
    std::getline(std::cin, id_1);
    std::cout << "Enter ID2: ";
    std::getline(std::cin, id_2);

    auto IP = getInteractionParameters(gasesIPs, id_1, id_2);

    std::cout << IP.CASN_1 << "\n";
    std::cout << IP.CASN_2 << "\n";
    std::cout << IP.Name_1 << "\n";
    std::cout << IP.Name_2 << "\n";
    std::cout << IP.k12 << "\n";
    
    return 0;
}