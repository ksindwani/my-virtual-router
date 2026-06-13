#pragma once
#include <string>

struct Interface {
    std::string name;
    std::string ip_prefix;
    bool is_up;
    int rx_packets;
    int tx_packets;

    // Optional: Constructor for easier creation
    Interface(std::string n, std::string ip, bool up, int rx, int tx)
        : name(n), ip_prefix(ip), is_up(up), rx_packets(rx), tx_packets(tx) {}
    
    // Default constructor for vector operations
    Interface() : is_up(false), rx_packets(0), tx_packets(0) {}
};