#include "RoutingTable.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <iomanip>

using json = nlohmann::json;

void RoutingTable::loadFromJSON(const std::string& intfFile, const std::string& routeFile) {
    // 1. Parse Interfaces (Connected Routes)
    std::ifstream iFile(intfFile);
    if (iFile.is_open()) {
        json j;
        iFile >> j;
        for (const auto& item : j["interfaces"]) {
            IPPrefix p = IPPrefix::fromString(item["ip_prefix"]);
            addRoute({p.addr, p.prefix_len, item["name"], "Connected"});
        }
    }

    // 2. Parse Static Routes
    std::ifstream rFile(routeFile);
    if (rFile.is_open()) {
        json j;
        rFile >> j;
        for (const auto& item : j["static_routes"]) {
            IPPrefix p = IPPrefix::fromString(item["prefix"]);
            addRoute({p.addr, p.prefix_len, item["next_hop"], "Static"});
        }
    }
}

UnifiedRoute* RoutingTable::lookup(uint32_t dest_ip) {
    UnifiedRoute* best_match = nullptr;

    for (auto& r : routes) {
        uint32_t mask = IPPrefix::getMask(r.prefix_len);
        // Bitwise match check
        if ((dest_ip & mask) == (r.prefix_bin & mask)) {
            // Longest Prefix Match logic: prefer higher prefix_len
            if (!best_match || r.prefix_len > best_match->prefix_len) {
                best_match = &r;
            }
        }
    }
    return best_match;
}

void RoutingTable::explainLookup(uint32_t dest_ip) {
    std::cout << "--- Routing Lookup Explanation ---" << std::endl;
    std::cout << "Destination IP (Binary): " << dest_ip << std::endl;
    
    UnifiedRoute* match = lookup(dest_ip);
    
    if (match) {
        std::cout << "Result: Match Found!" << std::endl;
        std::cout << "Selected Prefix: " << match->prefix_bin << "/" << match->prefix_len << std::endl;
        std::cout << "Route Type:      " << match->type << std::endl;
        std::cout << "Resolved To:     " << match->next_hop << std::endl;
        std::cout << "Criteria:        Best match (longest prefix length)." << std::endl;
    } else {
        std::cout << "Result: No route found (Dropped)." << std::endl;
    }
    std::cout << "----------------------------------" << std::endl;
}