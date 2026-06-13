#pragma once
#include <string>
#include <cstdint>

struct Interface {
    std::string name;
    std::string ip_prefix; // e.g., "192.168.1.1/24"
    bool is_up;
    uint64_t rx_packets;
    uint64_t tx_packets;
};