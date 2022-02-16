#include "traffic_manager.h"

#include <cassert>
#include <cmath>
#include <numeric>
#include <set>
#include <unordered_map>

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
  assert(0 <= town && town < buns_amounts_.size());
  return buns_amounts_[town];
}

int TrafficManager::GetVehicle(int town) const {
  assert(0 <= town && town < vehicles_.size());
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
  assert(0 <= town && town < buns_amounts_.size());
  total_buns_amount_ -= buns_amounts_[town];
  total_buns_amount_ += buns_amount;
  buns_amounts_[town] = buns_amount;
}

void TrafficManager::SetVehicle(int town, int vehicle) {
  assert(0 <= town && town < vehicles_.size());
  total_vehicles_ -= vehicles_[town];
  total_vehicles_ += vehicle;
  vehicles_[town] = vehicle;
}

// maybe check if it's possible to move
int TrafficManager::MoveVehicles(int from, int to, int count) {
  assert(0 <= from && from < vehicles_.size());
  assert(0 <= to && to < vehicles_.size());
  vehicles_[from] -= count;
  vehicles_[to] += count;
  return GetLenForPath(graph_.GetShortestPath(from, to));
}

int TrafficManager::Transport(int from, int to, int buns_amount) {
  assert(0 <= from && from < vehicles_.size());
  assert(0 <= to && to < vehicles_.size());
  int vehicles_needed = ceil(1. * buns_amount / vehicle_capacity_);
  int result =
      MoveClosestVehicles(from, std::max(vehicles_needed - vehicles_[from], 0));
  result += MoveVehicles(from, to, vehicles_needed);
  return result;
}

struct PathToTownInfo {
  int town_index{0};
  int length{0};

  std::strong_ordering operator<=>(
      const PathToTownInfo& path_to_town_info) const = default;
};

int TrafficManager::MoveClosestVehicles(int to, int count) {
  int res = 0;
  std::set<PathToTownInfo> towns_queue;
  towns_queue.insert({0, to});
  std::unordered_map<int, int> distance;
  distance[to] = 0;

  while (count > 0 && !towns_queue.empty()) {
    auto begin_it = towns_queue.begin();
    int town_index = begin_it->town_index;
    towns_queue.erase(begin_it);

    int cur_move_count = std::min(vehicles_[town_index], count);
    res = MoveVehicles(town_index, to, cur_move_count);
    count -= cur_move_count;
    for (auto [next_vertex, len] : graph_.GetEdges(town_index)) {
      bool can_update_existing =
          distance[next_vertex] > len + distance[town_index];
      if (can_update_existing) {
        towns_queue.erase({distance[next_vertex], next_vertex});
      }
      if (!distance.contains(next_vertex) || can_update_existing) {
        distance[next_vertex] = distance[town_index] + len;
        towns_queue.insert({distance[next_vertex], next_vertex});
      }
    }
  }
  return res;
}

int TrafficManager::GetLenForPath(const std::vector<Edge>& path) {
  // maybe int64_t
  int total_len = 0;
  for (const auto& [_, len] : path) {
    total_len += len;
  }
  return total_len;
}
