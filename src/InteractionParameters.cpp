#include "../include/InteractionParameters.hpp"
#include "../include/json.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using json = nlohmann::json;

namespace BinaryIPs {

    std::vector<InteractionParameter> parseInteractionParameters(const std::string& filePath) {
        std::ifstream file(filePath);

        if (!file.is_open()) {
            throw std::runtime_error("Failed to load the pripdb file.");
        }

        json data = json::parse(file);
        std::vector<InteractionParameter> gasesIPs;

        for (const auto& item : data) {
            InteractionParameter auxIP;

            auxIP.CASN_1 = item.value("CASN_1", "");
            auxIP.CASN_2 = item.value("CASN_2", "");
            auxIP.Name_1 = item.value("Name_1", "");
            auxIP.Name_2 = item.value("Name_2", "");
            auxIP.k12 = std::stod(item.value("k12", "0.0"));

            gasesIPs.push_back(auxIP);
        }

        file.close();

        return gasesIPs;
    }

    InteractionParameter getInteractionParameters(const std::vector<InteractionParameter>& gasesIPs, const std::string& id_1, const std::string& id_2) {
        for (const auto& item : gasesIPs) {
            if (item.CASN_1 == id_1 || item.CASN_2 == id_1) {
                if (item.CASN_1 == id_2 || item.CASN_2 == id_2) {
                    return item;
                }
            } else if (item.Name_1 == id_1 || item.Name_2 == id_1) {
                if (item.Name_1 == id_2 || item.Name_2 == id_2) {
                    return item;
                }
            } else continue;
        }

        throw std::invalid_argument("Could not find the BIPs for the pair " + id_1 + ", " + id_2 + ".");
    }

}