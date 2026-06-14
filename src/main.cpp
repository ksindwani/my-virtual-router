#include <iostream>
#include <thread>
#include "RoutingTable.hpp"
#include "RouterCLI.hpp"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <interfaces_file> <static_routes_file>" << std::endl;
        return 1;
    }
    RoutingTable rt;
    rt.loadInterfaces(argv[1]);
    rt.loadRoutes(argv[2]);

    runShell(rt);

    return 0;
}