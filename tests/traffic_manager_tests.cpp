#include <gtest/gtest.h>

#include "../traffic_manager.h"

class TrafficManagerTester {
 public:
  explicit TrafficManagerTester(TrafficManager traffic_manager) :
      traffic_manager_(std::move(traffic_manager)) {};
  Graph GetGraph() const {
    return traffic_manager_.graph_;
  }

  int GetVehicleCapacity() const {
    return traffic_manager_.vehicle_capacity_;
  }

 private:
  TrafficManager traffic_manager_;
};

bool operator==(const Graph& lhs, const Graph& rhs) {
  if (lhs.GetSize() == rhs.GetSize()) {
    for (int from = 0; from < lhs.GetSize(); ++from) {
      if (lhs.GetEdges(from) != rhs.GetEdges(from)) {
        return false;
      }
    }
    return true;
  }
  return false;
}

TEST(TrafficManager, Constructor) {
  {
    Graph graph;
    std::vector<int> buns_amounts;
    std::vector<int> vehicles;
    int vehicle_capacity = 0;
    TrafficManager traffic_manager(
        graph,
        buns_amounts,
        vehicles,
        vehicle_capacity);
    TrafficManagerTester tester(traffic_manager);
    EXPECT_EQ(tester.GetGraph(), graph);
    EXPECT_EQ(tester.GetVehicleCapacity(), vehicle_capacity);
    EXPECT_EQ(traffic_manager.GetBunsAmounts(), buns_amounts);
    EXPECT_EQ(traffic_manager.GetVehicles(), vehicles);
    EXPECT_EQ(traffic_manager.GetTotalBunsAmount(), 0);
    EXPECT_EQ(traffic_manager.GetTotalVehicles(), 0);
  }
  {
    Graph graph(4);
    std::vector<int> buns_amounts = {1, 2, 3, 42};
    std::vector<int> vehicles = {42, 1, 4, 8};
    int vehicle_capacity = 14;
    TrafficManager traffic_manager(
        graph,
        buns_amounts,
        vehicles,
        vehicle_capacity);
    TrafficManagerTester tester(traffic_manager);
    EXPECT_EQ(tester.GetGraph(), Graph({
                                           {Graph::Edge(1, 1),
                                            Graph::Edge(2, 1),
                                            Graph::Edge(3, 1)},
                                           {Graph::Edge(0, 1),
                                            Graph::Edge(2, 1),
                                            Graph::Edge(3, 1)},
                                           {Graph::Edge(0, 1),
                                            Graph::Edge(1, 1),
                                            Graph::Edge(3, 1)},
                                           {Graph::Edge(0, 1),
                                            Graph::Edge(1, 1),
                                            Graph::Edge(2, 1)},
                                       }));
    EXPECT_EQ(tester.GetVehicleCapacity(), vehicle_capacity);
    EXPECT_EQ(traffic_manager.GetBunsAmounts(), buns_amounts);
    EXPECT_EQ(traffic_manager.GetVehicles(), vehicles);
    EXPECT_EQ(traffic_manager.GetTotalBunsAmount(), 48);
    EXPECT_EQ(traffic_manager.GetTotalVehicles(), 55);
  }
}

TEST(TrafficManager, Setters) {
  {
    Graph graph(4);
    std::vector<int> buns_amounts = {1, 2, 3, 42};
    std::vector<int> vehicles = {42, 1, 4, 8};
    int vehicle_capacity = 14;
    TrafficManager traffic_manager(
        graph,
        buns_amounts,
        vehicles,
        vehicle_capacity);
    std::vector<int> new_buns_amounts = {2, 14, 7, 5};
    std::vector<int> new_vehicles = {6, 5, 0, 12};
    traffic_manager.SetBunsAmounts(new_buns_amounts);
    traffic_manager.SetVehicles(new_vehicles);
    EXPECT_EQ(traffic_manager.GetBunsAmounts(), new_buns_amounts);
    EXPECT_EQ(traffic_manager.GetVehicles(), new_vehicles);
    EXPECT_EQ(traffic_manager.GetTotalBunsAmount(), 28);
    EXPECT_EQ(traffic_manager.GetTotalVehicles(), 23);
  }
  {
    Graph graph(4);
    std::vector<int> buns_amounts = {1, 2, 3, 42};
    std::vector<int> vehicles = {42, 1, 4, 8};
    int vehicle_capacity = 14;
    TrafficManager traffic_manager(
        graph,
        buns_amounts,
        vehicles,
        vehicle_capacity);
    std::vector<int> new_buns_amounts = {1, 2, 1, 42};
    std::vector<int> new_vehicles = {40, 1, 4, 8};
    traffic_manager.SetBunsAmount(2, 1);
    traffic_manager.SetVehicle(0, 40);
    EXPECT_EQ(traffic_manager.GetBunsAmounts(), new_buns_amounts);
    EXPECT_EQ(traffic_manager.GetVehicles(), new_vehicles);
    EXPECT_EQ(traffic_manager.GetTotalBunsAmount(), 46);
    EXPECT_EQ(traffic_manager.GetTotalVehicles(), 53);
  }
  {
    Graph graph(2);
    std::vector<int> buns_amounts = {0, 0};
    std::vector<int> vehicles = {0, 0};
    int vehicle_capacity = 14;
    TrafficManager traffic_manager(
        graph,
        buns_amounts,
        vehicles,
        vehicle_capacity);
    std::vector<int> new_buns_amounts = {4, 11};
    std::vector<int> new_vehicles = {8, 6};
    traffic_manager.SetBunsAmount(0, 4);
    traffic_manager.SetBunsAmount(1, 11);
    traffic_manager.SetVehicle(0, 8);
    traffic_manager.SetVehicle(1, 6);
    EXPECT_EQ(traffic_manager.GetBunsAmounts(), new_buns_amounts);
    EXPECT_EQ(traffic_manager.GetVehicles(), new_vehicles);
    EXPECT_EQ(traffic_manager.GetTotalBunsAmount(), 15);
    EXPECT_EQ(traffic_manager.GetTotalVehicles(), 14);
  }
}

TEST(TrafficManager, Getters) {
  {
    Graph graph(4);
    std::vector<int> buns_amounts = {1, 2, 3, 42};
    std::vector<int> vehicles = {42, 1, 4, 8};
    int vehicle_capacity = 14;
    TrafficManager traffic_manager(
        graph,
        buns_amounts,
        vehicles,
        vehicle_capacity);
    auto got_buns_amounts = traffic_manager.GetBunsAmounts();
    auto got_vehicles = traffic_manager.GetVehicles();
    for (int i = 0; i < 4; ++i) {
      EXPECT_EQ(got_buns_amounts[i], traffic_manager.GetBunsAmount(i));
      EXPECT_EQ(got_vehicles[i], traffic_manager.GetVehicle(i));
    }
  }
  {
    Graph graph(4);
    std::vector<int> buns_amounts = {1, 2, 3, 42};
    std::vector<int> vehicles = {42, 1, 4, 8};
    int vehicle_capacity = 14;
    TrafficManager traffic_manager(
        graph,
        buns_amounts,
        vehicles,
        vehicle_capacity);
    EXPECT_DEATH(traffic_manager.GetBunsAmount(-1), "");
    EXPECT_DEATH(traffic_manager.GetBunsAmount(-2), "");
    EXPECT_DEATH(traffic_manager.GetBunsAmount(4), "");
    EXPECT_DEATH(traffic_manager.GetBunsAmount(5), "");
    EXPECT_DEATH(traffic_manager.GetBunsAmount(100), "");

    EXPECT_DEATH(traffic_manager.GetVehicle(-1), "");
    EXPECT_DEATH(traffic_manager.GetVehicle(-2), "");
    EXPECT_DEATH(traffic_manager.GetVehicle(4), "");
    EXPECT_DEATH(traffic_manager.GetVehicle(5), "");
    EXPECT_DEATH(traffic_manager.GetVehicle(100), "");
  }
}

TEST(TrafficManager, MoveVehicles) {

}

TEST(TrafficManager, Transport) {

}

TEST(TrafficManager, TransportWithReturns) {

}
