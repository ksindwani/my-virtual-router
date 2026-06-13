#include "EventProcessor.hpp"

void EventProcessor::processEvent(const nlohmann::json& event) {
    std::string type = event["type"];

    if (type == "interface_state") {
        std::string name = event["interface"];
        bool state = event["state"]; // true=up, false=down
        table.updateInterfaceState(name, state); // You need to implement this
    } 
    else if (type == "route_remove") {
        std::string prefix = event["prefix"];
        table.removeRoute(prefix); // You need to implement this
    }
}