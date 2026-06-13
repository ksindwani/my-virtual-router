// src/EventProcessor.cpp
#include <iostream>
#include "EventProcessor.hpp"
#include <nlohmann/json.hpp> // Ensure this is included

// ADD THIS LINE
using json = nlohmann::json;

void EventProcessor::processEvent(const json& event) {
    std::string type = event["type"];
    std::cout << "[DEBUG] Processing event type: " << type << std::endl;

    if (type == "interface_state") {
        std::string name = event["interface"];
        bool state = event["state"]; // true for UP, false for DOWN
        // 1. Update interface status
        table.updateInterfaceState(name, state);
        // 2. Cascading removal if DOWN
        if (!state) {
            table.removeRoutesByInterface(name);
        }
    } 
    else if (type == "route_remove") {
        std::string prefix = event["prefix"];
        std::cout << "[Event] Removing route: " << prefix << std::endl;
        table.removeRoute(event["prefix"]);
    }
}