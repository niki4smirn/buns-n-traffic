#include <gtest/gtest.h>

#include "../traffic_manager.h"

class TrafficManagerTester {
 public:
  explicit TrafficManagerTester(TrafficManager traffic_manager) :
      traffic_manager_(std::move(traffic_manager)) {}
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
  {
    Graph graph(4);
    std::vector<int> buns_amounts = {1, 2, 3, 42};
    std::vector<int> vehicles = {42, 1, 4, 8};
    int total_vehicles_count = 55;
    int vehicle_capacity = 14;
    TrafficManager traffic_manager(
        graph,
        buns_amounts,
        vehicles,
        vehicle_capacity);

    traffic_manager.MoveVehicles(0, 0, 42);
    EXPECT_EQ(traffic_manager.GetVehicles(), std::vector<int>({42, 1, 4, 8}));
    EXPECT_EQ(traffic_manager.GetTotalVehicles(), total_vehicles_count);

    traffic_manager.MoveVehicles(0, 1, 40);
    EXPECT_EQ(traffic_manager.GetVehicles(), std::vector<int>({2, 41, 4, 8}));
    EXPECT_EQ(traffic_manager.GetTotalVehicles(), total_vehicles_count);

    traffic_manager.MoveVehicles(3, 2, 8);
    EXPECT_EQ(traffic_manager.GetVehicles(), std::vector<int>({2, 41, 12, 0}));
    EXPECT_EQ(traffic_manager.GetTotalVehicles(), total_vehicles_count);

    traffic_manager.MoveVehicles(1, 3, 41);
    EXPECT_EQ(traffic_manager.GetVehicles(), std::vector<int>({2, 0, 12, 41}));
    EXPECT_EQ(traffic_manager.GetTotalVehicles(), total_vehicles_count);

    traffic_manager.MoveVehicles(2, 0, 11);
    EXPECT_EQ(traffic_manager.GetVehicles(), std::vector<int>({13, 0, 1, 41}));
    EXPECT_EQ(traffic_manager.GetTotalVehicles(), total_vehicles_count);

    traffic_manager.MoveVehicles(2, 3, 1);
    EXPECT_EQ(traffic_manager.GetVehicles(), std::vector<int>({13, 0, 0, 42}));
    EXPECT_EQ(traffic_manager.GetTotalVehicles(), total_vehicles_count);
  }
  {
    Graph graph(1);
    std::vector<int> buns_amounts = {1};
    std::vector<int> vehicles = {14};
    int vehicle_capacity = 14;
    TrafficManager traffic_manager(
        graph,
        buns_amounts,
        vehicles,
        vehicle_capacity);

    EXPECT_DEATH(traffic_manager.MoveVehicles(0, 1, 0), "");
    EXPECT_DEATH(traffic_manager.MoveVehicles(1, 0, 0), "");
    EXPECT_DEATH(traffic_manager.MoveVehicles(0, -1, 0), "");
    EXPECT_DEATH(traffic_manager.MoveVehicles(-1, 0, 0), "");
    EXPECT_DEATH(traffic_manager.MoveVehicles(3, 2, 0), "");
    EXPECT_DEATH(traffic_manager.MoveVehicles(-4, -6, 0), "");
    EXPECT_DEATH(traffic_manager.MoveVehicles(5, -7, 0), "");
    EXPECT_DEATH(traffic_manager.MoveVehicles(-8, 16, 0), "");
  }
  {
    std::vector<std::vector<Graph::Edge>> connections = {
        {Graph::Edge(4, 6)},
        {Graph::Edge(2, 3), Graph::Edge(4, 1)},
        {Graph::Edge(1, 3), Graph::Edge(3, 1)},
        {Graph::Edge(2, 1), Graph::Edge(4, 3)},
        {Graph::Edge(0, 6), Graph::Edge(1, 1), Graph::Edge(3, 3)}};

    Graph graph(connections);

    std::vector<int> buns_amounts = {1, 1, 1, 1, 1};
    std::vector<int> vehicles = {14, 14, 14, 28, 14};
    int vehicle_capacity = 14;
    TrafficManager traffic_manager(
        graph,
        buns_amounts,
        vehicles,
        vehicle_capacity);

    for (int from = 0; from < 5; ++from) {
      for (int to = 0; to < 5; ++to) {
        int path_len = 0;
        for (const auto&[_, len] : graph.GetShortestPath(from, to)) {
          path_len += len;
        }
        EXPECT_EQ(traffic_manager.MoveVehicles(from, to, 1),
                  path_len);
      }
    }
  }
}

Graph GenerateTransportTestGraph() {
  std::vector<std::vector<Graph::Edge>> connections = {
      {Graph::Edge(1, 10)},
      {Graph::Edge(0, 10), Graph::Edge(2, 1),
       Graph::Edge(3, 2), Graph::Edge(4, 3)},
      {Graph::Edge(1, 1)},
      {Graph::Edge(1, 2)},
      {Graph::Edge(1, 3)}};

  return Graph{connections};
}

TEST(TrafficManager, Transport) {
  {
    Graph graph = GenerateTransportTestGraph();

    std::vector<int> buns_amounts = {1, 14, 1, 1, 1};
    std::vector<int> vehicles = {14, 14, 14, 28, 14};
    int vehicle_capacity = 14;
    TrafficManager traffic_manager(
        graph,
        buns_amounts,
        vehicles,
        vehicle_capacity);

    EXPECT_EQ(traffic_manager.Transport(1, 0, 12), 10);
    EXPECT_EQ(traffic_manager.GetBunsAmounts(),
              std::vector<int>({13, 2, 1, 1, 1}));
    EXPECT_EQ(traffic_manager.GetVehicles(),
              std::vector<int>({15, 13, 14, 28, 14}));
    EXPECT_EQ(traffic_manager.GetTotalBunsAmount(), 18);
    EXPECT_EQ(traffic_manager.GetTotalVehicles(), 84);
  }
  {
    Graph graph = GenerateTransportTestGraph();

    std::vector<int> buns_amounts = {1, 15, 1, 1, 1};
    std::vector<int> vehicles = {14, 14, 14, 28, 14};
    int vehicle_capacity = 14;
    TrafficManager traffic_manager(
        graph,
        buns_amounts,
        vehicles,
        vehicle_capacity);

    EXPECT_EQ(traffic_manager.Transport(1, 0, 15), 10);
    EXPECT_EQ(traffic_manager.GetBunsAmounts(),
              std::vector<int>({16, 0, 1, 1, 1}));
    EXPECT_EQ(traffic_manager.GetVehicles(),
              std::vector<int>({16, 12, 14, 28, 14}));
    EXPECT_EQ(traffic_manager.GetTotalBunsAmount(), 19);
    EXPECT_EQ(traffic_manager.GetTotalVehicles(), 84);
  }
  {
    Graph graph = GenerateTransportTestGraph();

    std::vector<int> buns_amounts = {1, 15, 1, 1, 1};
    std::vector<int> vehicles = {0, 0, 1, 1, 0};
    int vehicle_capacity = 13;
    TrafficManager traffic_manager(
        graph,
        buns_amounts,
        vehicles,
        vehicle_capacity);

    EXPECT_EQ(traffic_manager.Transport(1, 0, 15), 12);
    EXPECT_EQ(traffic_manager.GetBunsAmounts(),
              std::vector<int>({16, 0, 1, 1, 1}));
    EXPECT_EQ(traffic_manager.GetVehicles(),
              std::vector<int>({2, 0, 0, 0, 0}));
    EXPECT_EQ(traffic_manager.GetTotalBunsAmount(), 19);
    EXPECT_EQ(traffic_manager.GetTotalVehicles(), 2);
  }
  {
    Graph graph = GenerateTransportTestGraph();

    std::vector<int> buns_amounts = {0, 16, 0, 0, 0};
    std::vector<int> vehicles = {1, 0, 1, 1, 1};
    int vehicle_capacity = 5;
    TrafficManager traffic_manager(
        graph,
        buns_amounts,
        vehicles,
        vehicle_capacity);

    EXPECT_EQ(traffic_manager.Transport(1, 0, 15), 13);
    EXPECT_EQ(traffic_manager.GetBunsAmounts(), std::vector<int>({15, 1, 0, 0, 0}));
    EXPECT_EQ(traffic_manager.GetVehicles(), std::vector<int>({4, 0, 0, 0, 0}));
    EXPECT_EQ(traffic_manager.GetTotalBunsAmount(), 16);
    EXPECT_EQ(traffic_manager.GetTotalVehicles(), 4);
  }
  {
    Graph graph = GenerateTransportTestGraph();

    std::vector<int> buns_amounts = {0, 16, 0, 0, 0};
    std::vector<int> vehicles = {1, 0, 1, 1, 1};
    int vehicle_capacity = 5;
    TrafficManager traffic_manager(
        graph,
        buns_amounts,
        vehicles,
        vehicle_capacity);

    EXPECT_EQ(traffic_manager.Transport(1, 0, 16), 20);
    EXPECT_EQ(traffic_manager.GetBunsAmounts(),
              std::vector<int>({16, 0, 0, 0, 0}));
    EXPECT_EQ(traffic_manager.GetVehicles(),
              std::vector<int>({4, 0, 0, 0, 0}));
    EXPECT_EQ(traffic_manager.GetTotalBunsAmount(), 16);
    EXPECT_EQ(traffic_manager.GetTotalVehicles(), 4);
  }
}

Graph GenerateTransportWithReturnsTestGraph() {
  std::vector<std::vector<Graph::Edge>> connections = {
      {Graph::Edge(1, 2)},
      {Graph::Edge(0, 2), Graph::Edge(2, 1),
       Graph::Edge(3, 6), Graph::Edge(4, 7)},
      {Graph::Edge(1, 1)},
      {Graph::Edge(1, 6)},
      {Graph::Edge(1, 7)}};

  return Graph{connections};
}

TEST(TrafficManager, TransportWithReturns) {
  {
    Graph graph = GenerateTransportTestGraph();

    std::vector<int> buns_amounts = {1, 14, 1, 1, 1};
    std::vector<int> vehicles = {14, 14, 14, 28, 14};
    int vehicle_capacity = 14;
    TrafficManager traffic_manager(
        graph,
        buns_amounts,
        vehicles,
        vehicle_capacity);

    EXPECT_EQ(traffic_manager.TransportWithReturns(1, 0, 12), 10);
    EXPECT_EQ(traffic_manager.GetBunsAmounts(),
              std::vector<int>({13, 2, 1, 1, 1}));
    EXPECT_EQ(traffic_manager.GetVehicles(),
              std::vector<int>({15, 13, 14, 28, 14}));
    EXPECT_EQ(traffic_manager.GetTotalBunsAmount(), 18);
    EXPECT_EQ(traffic_manager.GetTotalVehicles(), 84);
  }
  {
    Graph graph = GenerateTransportTestGraph();

    std::vector<int> buns_amounts = {1, 15, 1, 1, 1};
    std::vector<int> vehicles = {14, 14, 14, 28, 14};
    int vehicle_capacity = 14;
    TrafficManager traffic_manager(
        graph,
        buns_amounts,
        vehicles,
        vehicle_capacity);

    EXPECT_EQ(traffic_manager.TransportWithReturns(1, 0, 15), 10);
    EXPECT_EQ(traffic_manager.GetBunsAmounts(),
              std::vector<int>({16, 0, 1, 1, 1}));
    EXPECT_EQ(traffic_manager.GetVehicles(),
              std::vector<int>({16, 12, 14, 28, 14}));
    EXPECT_EQ(traffic_manager.GetTotalBunsAmount(), 19);
    EXPECT_EQ(traffic_manager.GetTotalVehicles(), 84);
  }
  {
    Graph graph = GenerateTransportTestGraph();

    std::vector<int> buns_amounts = {1, 15, 1, 1, 1};
    std::vector<int> vehicles = {0, 0, 1, 1, 0};
    int vehicle_capacity = 13;
    TrafficManager traffic_manager(
        graph,
        buns_amounts,
        vehicles,
        vehicle_capacity);

    EXPECT_EQ(traffic_manager.TransportWithReturns(1, 0, 15), 12);
    EXPECT_EQ(traffic_manager.GetBunsAmounts(),
              std::vector<int>({16, 0, 1, 1, 1}));
    EXPECT_EQ(traffic_manager.GetVehicles(),
              std::vector<int>({2, 0, 0, 0, 0}));
    EXPECT_EQ(traffic_manager.GetTotalBunsAmount(), 19);
    EXPECT_EQ(traffic_manager.GetTotalVehicles(), 2);
  }
  {
    Graph graph = GenerateTransportTestGraph();

    std::vector<int> buns_amounts = {0, 16, 0, 0, 0};
    std::vector<int> vehicles = {1, 0, 1, 1, 1};
    int vehicle_capacity = 5;
    TrafficManager traffic_manager(
        graph,
        buns_amounts,
        vehicles,
        vehicle_capacity);

    EXPECT_EQ(traffic_manager.TransportWithReturns(1, 0, 15), 13);
    EXPECT_EQ(traffic_manager.GetBunsAmounts(),
              std::vector<int>({15, 1, 0, 0, 0}));
    EXPECT_EQ(traffic_manager.GetVehicles(),
              std::vector<int>({4, 0, 0, 0, 0}));
    EXPECT_EQ(traffic_manager.GetTotalBunsAmount(), 16);
    EXPECT_EQ(traffic_manager.GetTotalVehicles(), 4);
  }
  {
    Graph graph = GenerateTransportTestGraph();

    std::vector<int> buns_amounts = {0, 16, 0, 0, 0};
    std::vector<int> vehicles = {1, 0, 1, 1, 1};
    int vehicle_capacity = 5;
    TrafficManager traffic_manager(
        graph,
        buns_amounts,
        vehicles,
        vehicle_capacity);

    EXPECT_EQ(traffic_manager.TransportWithReturns(1, 0, 16), 20);
    EXPECT_EQ(traffic_manager.GetBunsAmounts(),
              std::vector<int>({16, 0, 0, 0, 0}));
    EXPECT_EQ(traffic_manager.GetVehicles(),
              std::vector<int>({4, 0, 0, 0, 0}));
    EXPECT_EQ(traffic_manager.GetTotalBunsAmount(), 16);
    EXPECT_EQ(traffic_manager.GetTotalVehicles(), 4);
  }

  {
    Graph graph = GenerateTransportWithReturnsTestGraph();

    std::vector<int> buns_amounts = {1, 14, 1, 1, 1};
    std::vector<int> vehicles = {14, 14, 14, 28, 14};
    int vehicle_capacity = 14;
    TrafficManager traffic_manager(
        graph,
        buns_amounts,
        vehicles,
        vehicle_capacity);

    EXPECT_EQ(traffic_manager.TransportWithReturns(1, 0, 12), 2);
    EXPECT_EQ(traffic_manager.GetBunsAmounts(),
              std::vector<int>({13, 2, 1, 1, 1}));
    EXPECT_EQ(traffic_manager.GetVehicles(),
              std::vector<int>({15, 13, 14, 28, 14}));
    EXPECT_EQ(traffic_manager.GetTotalBunsAmount(), 18);
    EXPECT_EQ(traffic_manager.GetTotalVehicles(), 84);
  }
  {
    Graph graph = GenerateTransportWithReturnsTestGraph();

    std::vector<int> buns_amounts = {1, 15, 1, 1, 1};
    std::vector<int> vehicles = {14, 14, 14, 28, 14};
    int vehicle_capacity = 14;
    TrafficManager traffic_manager(
        graph,
        buns_amounts,
        vehicles,
        vehicle_capacity);

    EXPECT_EQ(traffic_manager.TransportWithReturns(1, 0, 15), 2);
    EXPECT_EQ(traffic_manager.GetBunsAmounts(),
              std::vector<int>({16, 0, 1, 1, 1}));
    EXPECT_EQ(traffic_manager.GetVehicles(),
              std::vector<int>({16, 12, 14, 28, 14}));
    EXPECT_EQ(traffic_manager.GetTotalBunsAmount(), 19);
    EXPECT_EQ(traffic_manager.GetTotalVehicles(), 84);
  }
  {
    Graph graph = GenerateTransportWithReturnsTestGraph();

    std::vector<int> buns_amounts = {1, 15, 1, 1, 1};
    std::vector<int> vehicles = {0, 0, 1, 1, 0};
    int vehicle_capacity = 13;
    TrafficManager traffic_manager(
        graph,
        buns_amounts,
        vehicles,
        vehicle_capacity);

    EXPECT_EQ(traffic_manager.TransportWithReturns(1, 0, 15), 7);
    EXPECT_EQ(traffic_manager.GetBunsAmounts(),
              std::vector<int>({16, 0, 1, 1, 1}));
    EXPECT_EQ(traffic_manager.GetVehicles(),
              std::vector<int>({1, 0, 0, 1, 0}));
    EXPECT_EQ(traffic_manager.GetTotalBunsAmount(), 19);
    EXPECT_EQ(traffic_manager.GetTotalVehicles(), 2);
  }
  {
    Graph graph = GenerateTransportWithReturnsTestGraph();

    std::vector<int> buns_amounts = {0, 16, 0, 0, 0};
    std::vector<int> vehicles = {1, 0, 1, 1, 1};
    int vehicle_capacity = 5;
    TrafficManager traffic_manager(
        graph,
        buns_amounts,
        vehicles,
        vehicle_capacity);

    EXPECT_EQ(traffic_manager.TransportWithReturns(1, 0, 15), 7);
    EXPECT_EQ(traffic_manager.GetBunsAmounts(),
              std::vector<int>({15, 1, 0, 0, 0}));
    EXPECT_EQ(traffic_manager.GetVehicles(),
              std::vector<int>({1, 1, 0, 1, 1}));
    EXPECT_EQ(traffic_manager.GetTotalBunsAmount(), 16);
    EXPECT_EQ(traffic_manager.GetTotalVehicles(), 4);
  }
  {
    Graph graph = GenerateTransportWithReturnsTestGraph();

    std::vector<int> buns_amounts = {0, 16, 0, 0, 0};
    std::vector<int> vehicles = {1, 0, 1, 1, 1};
    int vehicle_capacity = 5;
    TrafficManager traffic_manager(
        graph,
        buns_amounts,
        vehicles,
        vehicle_capacity);

    EXPECT_EQ(traffic_manager.TransportWithReturns(1, 0, 16), 8);
    EXPECT_EQ(traffic_manager.GetBunsAmounts(),
              std::vector<int>({16, 0, 0, 0, 0}));
    EXPECT_EQ(traffic_manager.GetVehicles(),
              std::vector<int>({2, 1, 0, 0, 1}));
    EXPECT_EQ(traffic_manager.GetTotalBunsAmount(), 16);
    EXPECT_EQ(traffic_manager.GetTotalVehicles(), 4);
  }
}
