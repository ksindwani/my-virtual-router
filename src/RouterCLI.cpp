#include "RouterCLI.hpp"
#include "CLIUtils.hpp"
#include "EventProcessor.hpp"
#include <sys/inotify.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <nlohmann/json.hpp>

void watchAndReload(RoutingTable& rt) {
    int fd = inotify_init();
    int wd = inotify_add_watch(fd, "input", IN_MODIFY);
    char buffer[4096];

    while (true) {
        read(fd, buffer, sizeof(buffer));
        usleep(200000); // Debounce: wait 200ms for file write to complete

        std::cout << "\n[WATCHER] Configuration change detected. Reloading..." << std::endl;
        rt.loadInterfaces("input/interfaces.json");
        rt.loadRoutes("input/static_routes.json");
    }
}

void runShell(RoutingTable& rt) {
    std::string line;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, line);
        if (line == "exit") break;
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string cmd;
        ss >> cmd;

        if (cmd == "replay-events") {
            // std::string dir;
            // ss >> dir; // e.g., "./sample"
            
            std::ifstream file("input/events.json");
            if (!file.is_open()) {
                std::cerr << "Error: Could not open events.json" << std::endl;
                continue;
            }
            
            nlohmann::json j;
            file >> j;
            EventProcessor processor(rt);
            for (const auto& event : j["events"]) {
                processor.processEvent(event);
            }
            std::cout << "Event replay complete." << std::endl;
        } else if (cmd == "explain-lookup") {
            std::string ip_str; ss >> ip_str;
            uint32_t ip = IPPrefix::fromString(ip_str).addr;
            rt.explainLookup(ip);
        } else if (cmd == "show-routes") {
            printRouteTable(rt.getRoutes());
        } else if (cmd == "show-interfaces") {
            printInterfaceTable(rt.getInterfaces());
        } else if (cmd == "lookup") {
            std::string ip_str; ss >> ip_str;
            uint32_t ip = IPPrefix::fromString(ip_str).addr;
            UnifiedRoute* match = rt.lookup(ip);
            if (match) {
                std::cout << "Match found: " << match->next_hop << std::endl;
            }
            else {
                std::cout << "No match." << std::endl;
            }
        }
        else {
            std::cout << "Unknown command: " << cmd << std::endl;
        }
    }
}