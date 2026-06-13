#pragma once
#include <vector>
#include <string>
#include "Utils.hpp"

struct UnifiedRoute {
    uint32_t prefix_bin;
    int prefix_len;
    std::string next_hop;
    std::string type;
};

class RoutingTable {
    std::vector<UnifiedRoute> routes;
public:
    void addRoute(const UnifiedRoute& r) { routes.push_back(r); }
    void loadFromJSON(const std::string& intfFile, const std::string& routeFile);
    UnifiedRoute* lookup(uint32_t dest_ip);
    void explainLookup(uint32_t dest_ip);
    const std::vector<UnifiedRoute>& getRoutes() const { return routes; }
};