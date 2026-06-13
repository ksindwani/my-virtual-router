#include <iostream>
#include <thread>
#include "RoutingTable.hpp"
#include "RouterCLI.hpp"

int main() {
    RoutingTable rt;
    std::string fIntf = "input/interfaces.json";
    std::string fStRt = "input/static_routes.json";

    rt.loadInterfaces(fIntf);
    rt.loadRoutes(fStRt);
    runShell(rt);

    return 0;
}