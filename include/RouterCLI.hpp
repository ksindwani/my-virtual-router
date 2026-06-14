// RouterCLI.hpp
#pragma once
#include <vector>
#include "RoutingTable.hpp"
#include "Route.hpp"

void runShell(RoutingTable& rt);


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
bool isValidIP(const std::string& ip_str);