#include "../src/PengRobinson.cpp"
#include "../src/IdealGas.cpp"
#include <vector>
#include <string>
#include <iomanip>
#include <iostream>

int main(int argc, char* argv[]) {
    std::vector<std::string> gasNames = {
        "Carbon dioxide", 
        "Nitrogen", 
        "Methane", 
        "Ethane", 
        "Propane", 
        "N-butane", 
        "Isobutane", 
        "Isopentane", 
        "N-pentane", 
        "N-hexane", 
        "N-heptane"
    };
    std::vector<double> zs = {
        0.39903778076171875, 
        0.004628387093544006, 
        0.4591173553466797, 
        0.05989595890045166, 
        0.044221301078796384, 
        0.015023279190063476, 
        0.007778112888336182, 
        0.004155109822750092, 
        0.0050702130794525145, 
        0.001051282286643982, 
        2.121955156326294e-05
    };
    
    IdealGasEOS eos_id = IdealGasEOS(gasNames);
    PengRobinsonEOS eos = PengRobinsonEOS(gasNames);
    
    double P = 167e5, T = 313.15;
    
    double Z = eos_id.compressibilityFactor(P, T, zs);

    std::cout << Z << "\n";

    double mW = eos_id.averageMolarWeight(zs);

    std::cout << mW << "\n";

    double v = eos_id.volume(P, T, zs, UnitBase::MASS);
    double d = eos_id.density(P, T, zs, UnitBase::MASS);

    std::cout << v << "\n";
    std::cout << d << "\n";

    double h = eos_id.enthalpy(P, T, zs, UnitBase::MOLAR);

    std::cout << h << "\n";

    // std::vector<std::vector<double>> k12 = eos.getKIJ();

    // const int width = 10;
    // for (const auto& row : k12) {
    //     for (const auto& value: row) {
    //         std::cout << std::setw(width) << std::fixed << std::setprecision(4) << value;
    //     } 
    //     std::cout << std::endl;
    // }

    return 0;
}