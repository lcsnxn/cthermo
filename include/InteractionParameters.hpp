#ifndef INTERACTIONPARAMETERS
#define INTERACTIONPARAMETERS

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace BinaryIPs {

    /*
    Struct to store the interaction parameter of a couple of gases.

    Fields:
    - `CASN_1`: CASN number of the first gas;
    - `CASN_2`: CASN number of the second gas;
    - `Name_1`: Name of the first gas;
    - `Name_2`: Name of the second gas;
    - `k12`: Interaction parameter.
    */ 
    struct InteractionParameter {
        std::string CASN_1;
        std::string CASN_2;
        std::string Name_1;
        std::string Name_2;
        double k12;
    };

    /*
    Function to parse the JSON interaction parameters database.

    Returns:
        A vector of `InteractionParameter` objects.
    */
    std::vector<InteractionParameter> parseInteractionParameters(const std::string& filePath);

    /* 
    Function to load the interaction parameters of a couple of gases.
    The id's of the gases can be either the gas name or its CASN number.

    Arguments:
    - `gasesIPs`: A vector of `InteractionParameter` objects;
    - `id_1`: ID of the first gas;
    - `id_2`: ID of the second gas;

    Returns:
        A `InteractionParameter` object.
    */
    InteractionParameter getInteractionParameters(
        const std::vector<InteractionParameter>& gasesIPs, 
        const std::string& id_1, 
        const std::string& id_2);

}

#endif