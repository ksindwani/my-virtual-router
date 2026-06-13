#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include "Utils.hpp"
#include "Interface.hpp"

struct UnifiedRoute {
    uint32_t prefix_bin;
    int prefix_len;
    std::string next_hop;
    std::string type;
};

class RoutingTable {
    std::vector<UnifiedRoute> routes;
    std::vector<Interface> interfaces; // Track interfaces state
public:
    void addRoute(const UnifiedRoute& r) { routes.push_back(r); }
    void loadInterfaces(const std::string& filename);
    void loadRoutes(const std::string& filename);
    void saveState(const std::string& intfFile, const std::string& routeFile);
    
    // State management for events
    void updateInterfaceState(const std::string& name, bool is_up);
    void removeRoutesByInterface(const std::string& intfName);
    void removeRoute(const std::string& prefix);
    
    UnifiedRoute* lookup(uint32_t dest_ip);
    void explainLookup(uint32_t dest_ip);
    const std::vector<UnifiedRoute>& getRoutes() const { return routes; }
    const std::vector<Interface>& getInterfaces() const { return interfaces; }
    bool isInterfaceUp(const std::string& name);
    bool isMatch(uint32_t dest_ip, const UnifiedRoute& r);
};