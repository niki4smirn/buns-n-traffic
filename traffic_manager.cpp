#include "traffic_manager.h"

#include <cassert>
#include <numeric>

TrafficManager::TrafficManager(
    const Graph& graph,
    std::vector<int> buns_amounts,
    std::vector<int> vehicles,
    int vehicle_capacity) :
    graph_(graph),
    buns_amounts_(std::move(buns_amounts)),
    vehicles_(std::move(vehicles)),
    vehicle_capacity_(vehicle_capacity) {
  total_buns_amount_ =
      std::accumulate(buns_amounts_.begin(), buns_amounts_.end(), 0);
  total_vehicles_ =
      std::accumulate(vehicles_.begin(), vehicles_.end(), 0);
}

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

int TrafficManager::GetTotalBunsAmount() const {
  return total_buns_amount_;
}

int TrafficManager::GetTotalVehicles() const {
  return total_vehicles_;
}
