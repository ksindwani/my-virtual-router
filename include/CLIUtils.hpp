#pragma once

#include <vector>
#include "Interface.hpp"
#include "RoutingTable.hpp"

// We use the full std:: namespace prefix to prevent naming conflicts
// with your own variables or other libraries.

/**
 * @brief Prints the interface status table
 * @param intfList The vector of interfaces to display
 */
void printInterfaceTable(const std::vector<Interface>& intfList);

/**
 * @brief Prints the routing table
 * @param routeList The vector of UnifiedRoute objects to display
 */
void printRouteTable(const std::vector<UnifiedRoute>& routeList);

std::string binToIP(uint32_t bin_ip);