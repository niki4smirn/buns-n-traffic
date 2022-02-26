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
  if (count == 0) {
    return 0;
  }
  vehicles_[from] -= count;
  vehicles_[to] += count;
  return GetLenForPath(graph_.GetShortestPath(from, to));
}

int TrafficManager::Transport(int from, int to, int buns_amount) {
  assert(0 <= from && from < vehicles_.size());
  assert(0 <= to && to < vehicles_.size());
  assert(buns_amounts_[from] >= buns_amount);
  int vehicles_needed = ceil(1. * buns_amount / vehicle_capacity_);
  int result =
      MoveClosestVehicles(from, std::max(vehicles_needed - vehicles_[from], 0));
  result += MoveVehicles(from, to, vehicles_needed);
  MoveBuns(from, to, buns_amount);
  return result;
}

struct PathToTownInfo {
  int length{0};
  int town_index{0};

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
    for (auto[next_node, len] : graph_.GetEdges(town_index)) {
      bool can_update_existing = false;
      if (distance.contains(next_node) &&
          distance[next_node] > len + distance[town_index]) {
        can_update_existing = true;
      }
      if (can_update_existing) {
        towns_queue.erase({distance[next_node], next_node});
      }
      if (!distance.contains(next_node) || can_update_existing) {
        distance[next_node] = distance[town_index] + len;
        towns_queue.insert({distance[next_node], next_node});
      }
    }
  }
  return res;
}

int TrafficManager::GetLenForPath(const std::vector<Graph::Edge>& path) {
  // maybe int64_t
  int total_len = 0;
  for (const auto&[_, len] : path) {
    total_len += len;
  }
  return total_len;
}

void TrafficManager::MoveBuns(int from, int to, int count) {
  SetBunsAmount(from, buns_amounts_[from] - count);
  SetBunsAmount(to, buns_amounts_[to] + count);
}

bool TrafficManager::ArrivalAction::operator>(
    TrafficManager::ArrivalAction arrival_action) const {
  return timestamp > arrival_action.timestamp;
}

TrafficManager::ActionsQueue TrafficManager::InitActionsQueue(
    int start_town,
    int finish_town,
    int main_path_len) const {
  ActionsQueue actions_queue;
  auto paths = graph_.GetShortestPaths(start_town);
  int cur_town_index = 0;
  actions_queue.push({0,
                      vehicles_[start_town],
                      start_town,
                      start_town});
  for (const auto& path : paths) {
    if (cur_town_index != start_town) {
      actions_queue.push({GetLenForPath(path) + main_path_len,
                          vehicles_[cur_town_index],
                          cur_town_index,
                          finish_town});
    }
    ++cur_town_index;
  }
  return actions_queue;
}

int TrafficManager::TransportWithReturns(int from, int to, int buns_amount) {
  assert(0 <= from && from < vehicles_.size());
  assert(0 <= to && to < vehicles_.size());
  assert(buns_amounts_[from] >= buns_amount);
  int result = 0;
  int main_path_len = GetLenForPath(graph_.GetShortestPath(from, to));
  auto actions_queue = InitActionsQueue(from, to, main_path_len);
  int vehicles_needed = ceil(1. * buns_amount / vehicle_capacity_);
  while (!actions_queue.empty()) {
    auto[timestamp, vehicles_count, cur_from, cur_to] = actions_queue.top();
    result = timestamp;
    actions_queue.pop();
    MoveVehicles(cur_from, cur_to, vehicles_count);
    if (cur_to == from) {
      actions_queue.push({timestamp + main_path_len,
                          vehicles_count,
                          from,
                          to});
    } else {
      vehicles_needed -= vehicles_count;
      if (vehicles_needed <= 0) {
        int to_return = -vehicles_needed;
        MoveVehicles(cur_to, cur_from, to_return);
        break;
      }
      actions_queue.push({timestamp + main_path_len,
                          vehicles_count,
                          to,
                          from});
    }
  }
  MoveBuns(from, to, buns_amount);
  return result;
}
