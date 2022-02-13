#pragma once

#include <vector>

class Graph {

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
  Graph graph_;
  std::vector<int> buns_amounts_;
  std::vector<int> vehicles_;
  int vehicle_capacity_{0};
};


