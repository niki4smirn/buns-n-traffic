#include "traffic_manager.h"

#include <cassert>

TrafficManager::TrafficManager(
    const Graph& graph,
    std::vector<int> buns_amounts,
    std::vector<int> vehicles,
    int vehicle_capacity) :
    graph_(graph),
    buns_amounts_(std::move(buns_amounts)),
    vehicles_(std::move(vehicles)),
    vehicle_capacity_(vehicle_capacity) {}

const std::vector<int>& TrafficManager::GetBunsAmounts() const {
  return buns_amounts_;
}

const std::vector<int>& TrafficManager::GetVehicles() const {
  return vehicles_;
}

int TrafficManager::GetBunsAmount(int town) const {
  assert(town < buns_amounts_.size());
  return buns_amounts_[town];
}

int TrafficManager::GetVehicle(int town) const {
  assert(town < vehicles_.size());
  return vehicles_[town];
}
