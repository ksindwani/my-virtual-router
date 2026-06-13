#include "CLIUtils.hpp"
#include <iostream>
#include <iomanip>
#include <arpa/inet.h>

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

std::string binToIP(uint32_t bin_ip) {
    struct in_addr addr;
    addr.s_addr = htonl(bin_ip); // Convert back to network byte order
    char str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &addr, str, INET_ADDRSTRLEN);
    return std::string(str);
}