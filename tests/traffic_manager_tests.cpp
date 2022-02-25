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

}

TEST(TrafficManager, MoveVehicles) {

}

TEST(TrafficManager, Transport) {

}

TEST(TrafficManager, TransportWithReturns) {

}
