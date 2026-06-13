#include "EventProcessor.hpp"
#include <iostream>

void EventProcessor::processEvent(const nlohmann::json& event) {
    std::string type = event["type"];

    if (type == "interface_state") {
        std::string name = event["target"];
        // Convert "down" string to boolean false
        bool is_up = (event["new_state"] == "up"); 
        table.updateInterfaceState(name, is_up);
        std::cout << "[EVENT] Interface " << name << " state updated to " << event["new_state"] << std::endl;
    } 
    else if (type == "route_remove") {
        std::string prefix = event["target"];
        table.removeRoute(prefix);
        std::cout << "[EVENT] Route " << prefix << " removed." << std::endl;
    }
}