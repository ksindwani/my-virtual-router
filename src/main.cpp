#include <iostream>
#include <thread>
#include "RoutingTable.hpp"
#include "RouterCLI.hpp"

int main() {
    // 1. Initialize the Routing Table
    RoutingTable rt;

    // 2. Perform initial load
    std::cout << "[SYSTEM] Performing initial data load..." << std::endl;
    rt.loadInterfaces("input/interfaces.json");
    rt.loadRoutes("input/static_routes.json");

    // 3. Launch background watcher thread for dynamic JSON updates
    // This thread runs independently and reloads data when files change
    std::thread watcherThread(watchAndReload, std::ref(rt));
    watcherThread.detach();

    // 4. Run the foreground CLI shell
    // This loop blocks execution until the user types 'exit'
    std::cout << "[SYSTEM] Router ready." << std::endl;
    runShell(rt);

    std::cout << "[SYSTEM] Exiting router..." << std::endl;
    return 0;
}