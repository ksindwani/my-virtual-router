#include "RoutingTable.hpp"
#include "CLIUtils.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    RoutingTable rt;
    rt.loadFromJSON("input/interfaces.json", "input/static_routes.json");

    std::string cmd = argv[1];
    if (cmd == "show-routes") {
        printRouteTable(rt.getRoutes());
    } else if (cmd == "lookup") {
        uint32_t ip = IPPrefix::fromString(argv[2]).addr;
        rt.explainLookup(ip);
    }
    return 0;
}