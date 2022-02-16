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

void TrafficManager::SetBunsAmounts(std::vector<int> buns_amounts) {
  buns_amounts_ = std::move(buns_amounts);
  total_buns_amount_ =
      std::accumulate(buns_amounts_.begin(), buns_amounts_.end(), 0);
}

void TrafficManager::SetVehicles(std::vector<int> vehicles) {
  vehicles_ = std::move(vehicles);
  total_vehicles_ =
      std::accumulate(vehicles_.begin(), vehicles_.end(), 0);
}

void TrafficManager::SetBunsAmount(int town, int buns_amount) {
  total_buns_amount_ -= buns_amounts_[town];
  total_buns_amount_ += buns_amount;
  buns_amounts_[town] = buns_amount;
}

void TrafficManager::SetVehicle(int town, int vehicle) {
  total_vehicles_ -= vehicles_[town];
  total_vehicles_ += vehicle;
  vehicles_[town] = vehicle;
}

// maybe check if it's possible to move
int TrafficManager::MoveVehicles(int from, int to, int count) {
  vehicles_[from] -= count;
  vehicles_[to] += count;
  auto path = graph_.GetShortestPath(from, to);
  // maybe int64_t
  int total_len = 0;
  for (const auto& [_, len] : path) {
    total_len += len;
  }
  return total_len;
}