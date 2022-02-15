#include "../graph.h"
#include "gtest/gtest.h"

TEST(Graph, Constructors) {
  {
    Graph graph;

    ASSERT_EQ(graph.GetSize(), 0);
    ASSERT_DEATH(graph.GetEdges(0), "");
  }
  {
    Graph graph(4);
    std::vector<std::vector<Graph::Edge>> connections = {
        {Graph::Edge(1, 1), Graph::Edge(2, 1), Graph::Edge(3, 1)},
        {Graph::Edge(0, 1), Graph::Edge(2, 1), Graph::Edge(3, 1)},
        {Graph::Edge(0, 1), Graph::Edge(1, 1), Graph::Edge(3, 1)},
        {Graph::Edge(0, 1), Graph::Edge(1, 1), Graph::Edge(2, 1)},
    };

    ASSERT_EQ(graph.GetSize(), 4);
    for (int i = 0; i < graph.GetSize(); ++i) {
      ASSERT_EQ(graph.GetEdges(i), connections[i]);
    }
  }
  {
    std::vector<std::vector<Graph::Edge>> connections = {
        {Graph::Edge(4, 6)},
        {Graph::Edge(2, 3), Graph::Edge(4, 1)},
        {Graph::Edge(1, 3), Graph::Edge(3, 2)},
        {Graph::Edge(2, 2), Graph::Edge(4, 7)},
        {Graph::Edge(0, 6), Graph::Edge(1, 1), Graph::Edge(3, 7)}};

    Graph graph(connections);

    ASSERT_EQ(graph.GetSize(), 5);
    for (int i = 0; i < graph.GetSize(); ++i) {
      ASSERT_EQ(graph.GetEdges(i), connections[i]);
    }
  }
}

TEST(Graph, GetSize) {
  {
    Graph graph;

    ASSERT_EQ(graph.GetSize(), 0);
  }
  {
    Graph graph(6);

    ASSERT_EQ(graph.GetSize(), 6);
  }
  {
    std::vector<std::vector<Graph::Edge>> connections = {
        {Graph::Edge(4, 6)},
        {Graph::Edge(2, 3), Graph::Edge(4, 1)},
        {Graph::Edge(1, 3), Graph::Edge(3, 2)},
        {Graph::Edge(2, 2), Graph::Edge(4, 7)},
        {Graph::Edge(0, 6), Graph::Edge(1, 1), Graph::Edge(3, 7)}};

    Graph graph(connections);

    ASSERT_EQ(graph.GetSize(), 5);
  }
}
