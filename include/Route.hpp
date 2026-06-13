#pragma once
#include <string>

struct Route {
    std::string prefix;    // Destination network
    std::string next_hop;  // Gateway IP or local interface
    int preference;        // Administrative distance
    std::string type;      // "Static", "Direct", etc.
};