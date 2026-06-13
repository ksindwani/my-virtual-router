#include "RoutingTable.hpp"
#include "CLIUtils.hpp"
#include "EventProcessor.hpp"
#include <iostream>
#include <fstream>            // Required for std::ifstream
#include <nlohmann/json.hpp>  // Required for nlohmann::json

int main(int argc, char* argv[]) {
    if (argc < 2) return 1;
    RoutingTable rt;
    rt.loadFromJSON("input/interfaces.json", "input/static_routes.json");

    std::string cmd = argv[1];
    if (cmd == "show-routes") {
        printRouteTable(rt.getRoutes());
    } else if (cmd == "replay-events" && argc == 3) {
        std::ifstream eFile(argv[2]);
        if (!eFile.is_open()) {
            std::cerr << "Error: Could not open events file!" << std::endl;
            return 1;
        }
        EventProcessor processor(rt);
        nlohmann::json j;
        eFile >> j;
        for (const auto& event : j["events"]) {
            std::cout<<"event change for evt: "<<event<<std::endl;
            processor.processEvent(event);
        }
        std::cout << "Event replay complete." << std::endl;
    } else if (cmd == "lookup") {
        uint32_t ip = IPPrefix::fromString(argv[2]).addr;
        rt.explainLookup(ip);
    } else if (cmd == "explain-lookup" && argc == 3) {
        uint32_t ip = IPPrefix::fromString(argv[2]).addr; // Ensure this exists
        rt.explainLookup(ip);
    } else if (cmd == "show-interfaces") {
        printInterfaceTable(rt.getInterfaces()); // Make sure this is implemented
    } else if (cmd == "lookup" && argc == 3) {
        uint32_t ip = IPPrefix::fromString(argv[2]).addr;
        UnifiedRoute* match = rt.lookup(ip);
        if (match) std::cout << "Match found: " << match->next_hop << std::endl;
        else std::cout << "No match." << std::endl;
    } else {
        std::cout<<"Unknown command: "<< cmd << std::endl;
    }
    return 0;
}