#include "../graph.h"
#include "gtest/gtest.h"

TEST(Graph, Constructors) {
  {
    Graph graph;

    ASSERT_EQ(graph.GetSize(), 0);
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

TEST(Graph, GetEdgeLength) {
  {
    Graph graph(6);

    for (int i = 0; i < 6; ++i) {
      for (int j = 0; j < 6; ++j) {
        if (i == j) {
          ASSERT_EQ(graph.GetEdgeLength(i, j), 0);
        } else {
          ASSERT_EQ(graph.GetEdgeLength(i, j), 1);
        }
      }
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

    ASSERT_EQ(graph.GetEdgeLength(0, 4), 6);
    ASSERT_EQ(graph.GetEdgeLength(4, 1), 1);
    ASSERT_EQ(graph.GetEdgeLength(4, 3), 7);
    ASSERT_EQ(graph.GetEdgeLength(1, 2), 3);
    ASSERT_EQ(graph.GetEdgeLength(2, 3), 2);
    ASSERT_EQ(graph.GetEdgeLength(0, 2), 0);

    ASSERT_EQ(graph.GetEdgeLength(4, 0), 6);
    ASSERT_EQ(graph.GetEdgeLength(1, 4), 1);
    ASSERT_EQ(graph.GetEdgeLength(3, 4), 7);
    ASSERT_EQ(graph.GetEdgeLength(2, 1), 3);
    ASSERT_EQ(graph.GetEdgeLength(3, 2), 2);
    ASSERT_EQ(graph.GetEdgeLength(2, 0), 0);
  }
}

TEST(Graph, GetEdges) {
  {
    Graph graph;

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

    for (int i = 0; i < graph.GetSize(); ++i) {
      ASSERT_EQ(graph.GetEdges(i), connections[i]);
    }
  }
}

// returns std::nullopt if path is invalid
std::optional<int> GetPathLength(
    const Graph& graph,
    const std::vector<Graph::Edge>& path,
    int from) {
  int distance = 0;

  for (const auto& edge : path) {
    if (graph.GetEdgeLength(from, edge.to) == edge.length) {
      distance += edge.length;
      from = edge.to;
    } else {
      return std::nullopt;
    }
  }

  return distance;
}

TEST(Graph, GetAnyPath) {
  {
    Graph graph(6);

    for (int i = 0; i < 6; ++i) {
      for (int j = 0; j < 6; ++j) {
        if (i != j) {
          ASSERT_NE(GetPathLength(graph, graph.GetAnyPath(i, j), i),
                    std::nullopt);
        } else {
          ASSERT_TRUE(graph.GetAnyPath(i, j).empty());
        }
      }
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

    for (int i = 0; i < 5; ++i) {
      for (int j = 0; j < 5; ++j) {
        if (i != j) {
          ASSERT_NE(GetPathLength(graph, graph.GetAnyPath(i, j), i),
                    std::nullopt);
        } else {
          ASSERT_TRUE(graph.GetAnyPath(i, j).empty());
        }
      }
    }
  }
}

TEST(Graph, GetShortestPath) {
  {
    Graph graph(6);

    for (int i = 0; i < 6; ++i) {
      for (int j = 0; j < 6; ++j) {
        if (i != j) {
          ASSERT_EQ(GetPathLength(graph, graph.GetShortestPath(i, j), i), 1);
        } else {
          ASSERT_TRUE(graph.GetShortestPath(i, j).empty());
        }
      }
    }
  }
  {
    std::vector<std::vector<Graph::Edge>> connections = {
        {Graph::Edge(4, 6)},
        {Graph::Edge(2, 3), Graph::Edge(4, 1)},
        {Graph::Edge(1, 3), Graph::Edge(3, 1)},
        {Graph::Edge(2, 1), Graph::Edge(4, 3)},
        {Graph::Edge(0, 6), Graph::Edge(1, 1), Graph::Edge(3, 3)}};

    Graph graph(connections);

    ASSERT_EQ(GetPathLength(graph, graph.GetShortestPath(0, 1), 0), 7);
    ASSERT_EQ(GetPathLength(graph, graph.GetShortestPath(1, 0), 1), 7);

    ASSERT_EQ(GetPathLength(graph, graph.GetShortestPath(0, 2), 0), 10);
    ASSERT_EQ(GetPathLength(graph, graph.GetShortestPath(2, 0), 2), 10);

    ASSERT_EQ(GetPathLength(graph, graph.GetShortestPath(0, 3), 0), 9);
    ASSERT_EQ(GetPathLength(graph, graph.GetShortestPath(3, 0), 3), 9);

    ASSERT_EQ(GetPathLength(graph, graph.GetShortestPath(0, 4), 0), 6);
    ASSERT_EQ(GetPathLength(graph, graph.GetShortestPath(4, 0), 4), 6);

    ASSERT_EQ(GetPathLength(graph, graph.GetShortestPath(1, 2), 1), 3);
    ASSERT_EQ(GetPathLength(graph, graph.GetShortestPath(2, 1), 2), 3);

    ASSERT_EQ(GetPathLength(graph, graph.GetShortestPath(1, 3), 1), 4);
    ASSERT_EQ(GetPathLength(graph, graph.GetShortestPath(3, 1), 3), 4);

    ASSERT_EQ(GetPathLength(graph, graph.GetShortestPath(1, 4), 1), 1);
    ASSERT_EQ(GetPathLength(graph, graph.GetShortestPath(4, 1), 4), 1);

    ASSERT_EQ(GetPathLength(graph, graph.GetShortestPath(2, 3), 2), 1);
    ASSERT_EQ(GetPathLength(graph, graph.GetShortestPath(3, 2), 3), 1);

    ASSERT_EQ(GetPathLength(graph, graph.GetShortestPath(2, 4), 2), 4);
    ASSERT_EQ(GetPathLength(graph, graph.GetShortestPath(4, 2), 4), 4);

    ASSERT_EQ(GetPathLength(graph, graph.GetShortestPath(3, 4), 3), 3);
    ASSERT_EQ(GetPathLength(graph, graph.GetShortestPath(4, 3), 4), 3);
  }
}

TEST(Graph, GetShortestPaths) {
  {
    Graph graph(6);

    for (int i = 0; i < 6; ++i) {
      auto paths = graph.GetShortestPaths(i);
      for (int j = 0; j < 6; ++j) {
        if (i != j) {
          ASSERT_EQ(GetPathLength(graph, paths[j], i), 1);
        } else {
          ASSERT_TRUE(paths[j].empty());
        }
      }
    }
  }
  {
    std::vector<std::vector<Graph::Edge>> connections = {
        {Graph::Edge(4, 6)},
        {Graph::Edge(2, 3), Graph::Edge(4, 1)},
        {Graph::Edge(1, 3), Graph::Edge(3, 1)},
        {Graph::Edge(2, 1), Graph::Edge(4, 3)},
        {Graph::Edge(0, 6), Graph::Edge(1, 1), Graph::Edge(3, 3)}};

    Graph graph(connections);

    auto paths = graph.GetShortestPaths(0);
    ASSERT_TRUE(paths[0].empty());
    ASSERT_EQ(GetPathLength(graph, paths[1], 0), 7);
    ASSERT_EQ(GetPathLength(graph, paths[2], 0), 10);
    ASSERT_EQ(GetPathLength(graph, paths[3], 0), 9);
    ASSERT_EQ(GetPathLength(graph, paths[4], 0), 6);

    paths = graph.GetShortestPaths(1);
    ASSERT_EQ(GetPathLength(graph, paths[0], 1), 7);
    ASSERT_TRUE(paths[1].empty());
    ASSERT_EQ(GetPathLength(graph, paths[2], 1), 3);
    ASSERT_EQ(GetPathLength(graph, paths[3], 1), 4);
    ASSERT_EQ(GetPathLength(graph, paths[4], 1), 1);

    paths = graph.GetShortestPaths(2);
    ASSERT_EQ(GetPathLength(graph, paths[0], 2), 10);
    ASSERT_EQ(GetPathLength(graph, paths[1], 2), 3);
    ASSERT_TRUE(paths[2].empty());
    ASSERT_EQ(GetPathLength(graph, paths[3], 2), 1);
    ASSERT_EQ(GetPathLength(graph, paths[4], 2), 4);

    paths = graph.GetShortestPaths(3);
    ASSERT_EQ(GetPathLength(graph, paths[0], 3), 9);
    ASSERT_EQ(GetPathLength(graph, paths[1], 3), 4);
    ASSERT_EQ(GetPathLength(graph, paths[2], 3), 1);
    ASSERT_TRUE(paths[3].empty());
    ASSERT_EQ(GetPathLength(graph, paths[4], 3), 3);

    paths = graph.GetShortestPaths(4);
    ASSERT_EQ(GetPathLength(graph, paths[0], 4), 6);
    ASSERT_EQ(GetPathLength(graph, paths[1], 4), 1);
    ASSERT_EQ(GetPathLength(graph, paths[2], 4), 4);
    ASSERT_EQ(GetPathLength(graph, paths[3], 4), 3);
    ASSERT_TRUE(paths[4].empty());
  }
}

TEST(Graph, GetEdgesCount) {
  {
    Graph graph(6);

    ASSERT_EQ(graph.GetEdgesCount(), 15);
  }
  {
    std::vector<std::vector<Graph::Edge>> connections = {
        {Graph::Edge(4, 6)},
        {Graph::Edge(2, 3), Graph::Edge(4, 1)},
        {Graph::Edge(1, 3), Graph::Edge(3, 2)},
        {Graph::Edge(2, 2), Graph::Edge(4, 7)},
        {Graph::Edge(0, 6), Graph::Edge(1, 1), Graph::Edge(3, 7)}};

    Graph graph(connections);

    ASSERT_EQ(graph.GetEdgesCount(), 5);
  }
}
