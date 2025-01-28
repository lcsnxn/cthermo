#ifndef GASPROPERTIES
#define GASPROPERTIES

#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

namespace GasConstants {

    /*
    Struct to store the physical constants of a gas.

    Fields:
    - `name`: Gas name;
    - `CASN`: Element CASN number;
    - `criticalTemperature`: Temperature at the critical point (in K);
    - `criticalPressure`: Pressure at the critical point (in Pa);
    - `criticalVolume`: Volume at the critical point (in m3/kmol);
    - `molecularWeight`: Molecular weight (in kg/kmol);
    - `acentricFactor`.
    - `idealGasHeatCapacityPolyCoeffs`: The ideal gas heat capacity coefficients
        of the polynomial equation C(T) = A + BT + CT^2 + DT^3
    */
    struct GasProperties {
        std::string name;
        std::string CASN;
        double criticalTemperature;
        double criticalPressure;
        double criticalVolume;
        double molecularWeight;
        double acentricFactor;
        std::map<std::string, double> idealGasHeatCapacityPolyCoeffs;
    };

    /* 
    Function to parse the JSON gas constants ChemSep database.

    Returns:
        A vector of `GasProperties` objects.
    */
    std::vector<GasProperties> parseGasProperties(const std::string& filePath);

    /*
    Function to load the physical constants of a gas. 
    The gas identifier can be either the gas name or its CASN number

    Arguments:
    - `gases`: A vector of `GasProperties` objects;
    - `identifier`: Either the gas CASN number or its name;

    Returns:
        A `GasProperties` object.
    */
    GasProperties getGasProperties(const std::vector<GasProperties>& gases, const std::string& identifier);

}

#endif