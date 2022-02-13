#include "traffic_manager.h"

TrafficManager::TrafficManager(
    const Graph& graph,
    std::vector<int> buns_amounts,
    std::vector<int> vehicles,
    int vehicle_capacity) :
    graph_(graph),
    buns_amounts_(std::move(buns_amounts)),
    vehicles_(std::move(vehicles)),
    vehicle_capacity_(vehicle_capacity) {}
