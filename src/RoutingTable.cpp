#include "RoutingTable.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <arpa/inet.h>

using json = nlohmann::json;

void RoutingTable::loadInterfaces(const std::string& file) {
    std::lock_guard<std::mutex> lock(mtx);
    interfaces.clear();
    std::ifstream f(file);
    if (!f.is_open()) return;

    nlohmann::json j;
    f >> j;

    for (const auto& item : j) {
        Interface intf;
        intf.name = item["name"];
        
        // Gatekeeper logic: Interface is UP only if both are "up"
        bool admin = (item.value("admin_state", "down") == "up");
        bool oper  = (item.value("oper_state", "down") == "up");
        intf.is_up = (admin && oper);
        
        intf.rx_packets = item.value("rx_packets", 0);
        intf.tx_packets = item.value("tx_packets", 0);
        
        interfaces.push_back(intf);
        
        std::cout << "[DEBUG] Loaded " << intf.name 
                  << " (Admin: " << item["admin_state"] 
                  << ", Oper: " << item["oper_state"] 
                  << ") -> is_up: " << (intf.is_up ? "TRUE" : "FALSE") << std::endl;
    }
}

void RoutingTable::loadRoutes(const std::string& file) {
    std::lock_guard<std::mutex> lock(mtx);
    routes.clear();
    std::ifstream f(file);
    if (!f.is_open()) return;

    nlohmann::json j;
    f >> j;

    for (const auto& item : j) {
        UnifiedRoute r;
        
        // Parse "172.16.0.0/16"
        std::string cidr = item["prefix"];
        size_t pos = cidr.find('/');
        std::string ip_str = cidr.substr(0, pos);
        r.prefix_len = std::stoi(cidr.substr(pos + 1));
        
        struct in_addr addr;
        inet_pton(AF_INET, ip_str.c_str(), &addr);
        r.prefix_bin = ntohl(addr.s_addr);

        // Flexible next-hop assignment
        if (item.contains("egress_interface")) {
            r.next_hop = item["egress_interface"];
        } else if (item.contains("next_hop_ip")) {
            // Note: In a real router, you would map this IP to an interface
            r.next_hop = "eth0"; 
        } else {
            r.next_hop = "unknown";
        }
        
        r.type = "Static";
        std::cout<<"Adding route";
        routes.push_back(r);
    }
}

UnifiedRoute* RoutingTable::lookup(uint32_t dest_ip) {
    std::lock_guard<std::mutex> lock(mtx);
    UnifiedRoute* best_match = nullptr;

    for (auto& r : routes) {
        if (!isInterfaceUp(r.next_hop)) continue;
        uint32_t mask = IPPrefix::getMask(r.prefix_len);
        // Bitwise match check
        if ((dest_ip & mask) == (r.prefix_bin & mask)) {
            // Longest Prefix Match logic: prefer higher prefix_len
            if (!best_match || r.prefix_len > best_match->prefix_len) {
                best_match = &r;
            }
        }
    }
    return best_match;
}

void RoutingTable::explainLookup(uint32_t dest_ip) {
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "--- Routing Lookup Explanation ---" << std::endl;
    std::cout << "Destination IP (Binary): " << dest_ip << std::endl;

    UnifiedRoute* best_match = nullptr;

    for (auto& r : routes) {
        // 1. Gatekeeper Check
        if (!isInterfaceUp(r.next_hop)) {
            std::cout << "Skipping route " << r.prefix_bin << "/" << r.prefix_len 
                      << " (Interface " << r.next_hop << " is DOWN)" << std::endl;
            continue;
        }

        // 2. Matching Check
        if (isMatch(dest_ip, r)) {
            std::cout << "Match Found: " << r.prefix_bin << "/" << r.prefix_len << std::endl;

            // 3. Best Match Selection Logic
            if (best_match == nullptr || r.prefix_len > best_match->prefix_len) {
                best_match = &r;
                std::cout << "-> New Best Match!" << std::endl;
            }
        }
    }

    // 4. Output Result
    if (best_match) {
        std::cout << "Result: Match Found!" << std::endl;
        std::cout << "Selected Prefix: " << best_match->prefix_bin << "/" << best_match->prefix_len << std::endl;
        std::cout << "Route Type:      " << best_match->type << std::endl;
        std::cout << "Resolved To:     " << best_match->next_hop << std::endl;
        std::cout << "Criteria:        Best match (longest prefix length)." << std::endl;
    } else {
        std::cout << "Result: No valid route found." << std::endl;
    }
}

bool RoutingTable::isInterfaceUp(const std::string& name) {
    for (const auto& intf : interfaces) {
        if (intf.name == name) {
            return intf.is_up;
        }
    }
    // Safety: If the interface isn't found, treat it as DOWN.
    return false;
}

bool RoutingTable::isMatch(uint32_t dest_ip, const UnifiedRoute& r) {
    // 1. A prefix length of 0 is the default route, which matches everything.
    if (r.prefix_len == 0) {
        return true;
    }

    // 2. Create a mask based on prefix_len.
    // Example: If len is 24, we need 24 leading ones.
    // We shift 0xFFFFFFFF (32 ones) to the right by (32 - len).
    uint32_t mask = 0xFFFFFFFF << (32 - r.prefix_len);

    // 3. Compare the network portion of both IPs.
    return (dest_ip & mask) == (r.prefix_bin & mask);
}

void RoutingTable::updateInterfaceState(const std::string& name, bool is_up) {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto& intf : interfaces) {
        if (intf.name == name) {
            intf.is_up = is_up;
            std::cout << "[EVENT] Interface " << name << " state set to " 
                      << (is_up ? "UP" : "DOWN") << std::endl;
        }
    }
}

void RoutingTable::removeRoute(const std::string& prefix) {
    // Use std::remove_if to find the specific route by string match
    auto it = std::remove_if(routes.begin(), routes.end(), 
        [&](const UnifiedRoute& r) {
            // Assuming your UnifiedRoute stores the original string prefix
            // You might need to compare r.prefix_bin/len or store the string
            return (formatPrefix(r) == prefix); 
        });

    if (it != routes.end()) {
        routes.erase(it, routes.end());
        std::cout << "[SUCCESS] Removed route: " << prefix << std::endl;
    } else {
        std::cout << "[WARNING] Route not found: " << prefix << std::endl;
    }
}

std::string RoutingTable::formatPrefix(const UnifiedRoute& r) {
    // 1. Convert binary IP back to string
    struct in_addr addr;
    addr.s_addr = htonl(r.prefix_bin); // Convert host-order back to network-order
    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &addr, ip_str, INET_ADDRSTRLEN);

    // 2. Append the prefix length
    std::stringstream ss;
    ss << ip_str << "/" << r.prefix_len;
    return ss.str();
}
