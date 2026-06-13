#include "CLIUtils.hpp"
#include <iostream>
#include <iomanip>

// Use explicit std:: prefixes to avoid collisions with any potential macros
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
    std::cout << std::left << std::setw(15) << "Prefix Bin" 
              << std::setw(10) << "Len" 
              << std::setw(15) << "Next Hop" 
              << std::setw(10) << "Type" << "\n";
    std::cout << std::string(50, '-') << "\n";

    for (const auto& route : routeList) {
        std::cout << std::left << std::setw(15) << route.prefix_bin 
                  << std::setw(10) << route.prefix_len 
                  << std::setw(15) << route.next_hop 
                  << std::setw(10) << route.type << "\n";
    }
}