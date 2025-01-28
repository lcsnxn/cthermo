#ifndef EQUATIONOFSTATE
#define EQUATIONOFSTATE

#include <string>
#include <vector>
#include <complex>

enum class UnitBase {
    MOLAR, // Molar base
    MASS   // Mass base, kg
};

// Base class for EoS implementations
class EquationOfState {
public:
    virtual ~EquationOfState() = default;

    // Compute the average molar weight MW
    virtual double averageMolarWeight(const std::vector<double>& moleFractions) const = 0;

    // Compute the compressibility factor Z
    virtual double compressibilityFactor(double pressure, double temperature, const std::vector<double>& moleFractions) const = 0;
    
    // Compute the volume V, either in m3/mol or in m3/kg
    virtual double volume(double pressure, double temperature, const std::vector<double>& moleFractions, UnitBase unit) const = 0;
    
    // Compute the density, either in mol/mo3 or in kg/m3
    virtual double density(double pressure, double temperature, const std::vector<double>& moleFractions, UnitBase unit) const = 0;

    // Compute the enthalpy, either in kJ/kg or in kJ/mol
    virtual double enthalpy(double pressure, double temperature, const std::vector<double>& moleFractions, UnitBase unit) const = 0;
};

#endif