#pragma once

#include <vector>
#include <queue>

struct Edge {
  int to{0};
  int len{0};
};

class Graph {
 public:
  std::vector<Edge> GetShortestPath(int from, int to) const {
    return {};
  }
  std::vector<std::vector<Edge>> GetShortestPaths(int from) const {
    return {};
  }
  std::vector<Edge> GetEdges(int from) const {
    return {};
  }
};

class TrafficManager {
 public:
  TrafficManager(
      const Graph& graph,
      std::vector<int> buns_amounts,
      std::vector<int> vehicles,
      int vehicle_capacity);

  const std::vector<int>& GetBunsAmounts() const;
  const std::vector<int>& GetVehicles() const;
  int GetBunsAmount(int town) const;
  int GetVehicle(int town) const;

  void SetBunsAmounts(std::vector<int> buns_amounts);
  void SetVehicles(std::vector<int> vehicles);
  void SetBunsAmount(int town, int buns_amount);
  void SetVehicle(int town, int vehicle);

  // maybe int64_t as return type
  int GetTotalBunsAmount() const;
  int GetTotalVehicles() const;

  int MoveVehicles(int from, int to, int count);

  int Transport(int from, int to, int buns_amount);
  int TransportWithReturns(int from, int to, int buns_amount);

 private:
  void MoveBuns(int from, int to, int count);
  int MoveClosestVehicles(int to, int count);
  static int GetLenForPath(const std::vector<Edge>& path);

  struct ArrivalAction {
    int timestamp{0};
    int vehicles_amount{0};
    int from_town_index{0};
    int to_town_index{0};

    bool operator>(ArrivalAction arrival_action) const;
  };

  using ActionsQueue = std::priority_queue<ArrivalAction,
                                           std::vector<ArrivalAction>,
                                           std::greater<>>;
  ActionsQueue InitActionsQueue(int from) const;

 private:
  Graph graph_;
  std::vector<int> buns_amounts_;
  std::vector<int> vehicles_;
  int vehicle_capacity_{0};

  int total_buns_amount_{0};
  int total_vehicles_{0};
};


