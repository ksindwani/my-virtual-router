#pragma once
#include <nlohmann/json.hpp>
#include "RoutingTable.hpp"

class EventProcessor {
    RoutingTable& table;
public:
    EventProcessor(RoutingTable& rt) : table(rt) {}
    void processEvent(const nlohmann::json& event);
};