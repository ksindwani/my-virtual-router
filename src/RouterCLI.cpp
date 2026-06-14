#include "RouterCLI.hpp"
#include "EventProcessor.hpp"
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <arpa/inet.h>
#include <nlohmann/json.hpp>

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
            std::string filename;
            ss >> filename; // Captures the filename argument
            
            if (filename.empty()) {
                std::cerr << "Error: Please provide a filename. Usage: replay-events <file>" << std::endl;
                continue;
            }

            std::ifstream file(filename);
            if (!file.is_open()) {
                std::cerr << "Error: Could not open " << filename << std::endl;
                continue;
            }
            
            nlohmann::json j;
            file >> j;
            EventProcessor processor(rt);
            
            // Iterate through the array directly
            for (const auto& event : j) {
                processor.processEvent(event);
            }
            std::cout << "Event replay complete." << std::endl;
        } else if (cmd == "explain-lookup") {
            std::string ip_str;
            ss >> ip_str;
            if (!isValidIP(ip_str)) {
                std::cerr << "Error: Invalid IP address format: " << ip_str << std::endl;
                continue; // Skip the lookup and return to shell prompt
            }
            uint32_t ip = IPPrefix::fromString(ip_str).addr;
            rt.explainLookup(ip);
        } else if (cmd == "show-routes") {
            printRouteTable(rt.getRoutes());
        } else if (cmd == "show-interfaces") {
            printInterfaceTable(rt.getInterfaces());
        } else if (cmd == "lookup") {
            std::string ip_str;
            ss >> ip_str;
            if (!isValidIP(ip_str)) {
                std::cerr << "Error: Invalid IP address format: " << ip_str << std::endl;
                continue; // Skip the lookup and return to shell prompt
            }
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

void printInterfaceTable(const std::vector<Interface>& intfList) {
    std::cout << std::left << std::setw(15) << "Name" 
              << std::setw(20) << "IP/Prefix" 
              << std::setw(10) << "Status" 
              << std::setw(10) << "RX" 
              << std::setw(10) << "TX" << "\n";
    std::cout << std::string(65, '-') << "\n";

    for (const auto& entry : intfList) {
        std::cout << std::left << std::setw(15) << entry.name 
                  << std::setw(20) << entry.ip_prefix 
                  << std::setw(10) << (entry.is_up ? "UP" : "DOWN") 
                  << std::setw(10) << entry.rx_packets 
                  << std::setw(10) << entry.tx_packets << "\n";
    }
}

void printRouteTable(const std::vector<UnifiedRoute>& routeList) {
    // Header
    std::cout << std::left << std::setw(20) << "Prefix" 
              << std::setw(15) << "Next Hop" 
              << std::setw(10) << "Type" << "\n";
    std::cout << std::string(45, '-') << "\n";

    for (const auto& route : routeList) {
        // Build the combined string here
        std::string full_prefix = binToIP(route.prefix_bin) + "/" + std::to_string(route.prefix_len);

        // Now print the single string with the width
        std::cout << std::left 
                  << std::setw(20) << full_prefix 
                  << std::setw(15) << route.next_hop 
                  << std::setw(10) << route.type << "\n";
    }
}

bool isValidIP(const std::string& ip_str) {
    struct sockaddr_in sa;
    // inet_pton returns 1 on success, 0 on invalid format
    return inet_pton(AF_INET, ip_str.c_str(), &(sa.sin_addr)) != 0;
}

std::string binToIP(uint32_t bin_ip) {
    struct in_addr addr;
    addr.s_addr = htonl(bin_ip); // Convert back to network byte order
    char str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &addr, str, INET_ADDRSTRLEN);
    return std::string(str);
}