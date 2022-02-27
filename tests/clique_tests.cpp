#include "../clique.h"
#include "gtest/gtest.h"

TEST(Clique, Constructors) {
  {
    Clique graph;

    ASSERT_EQ(graph.GetEdgesCount(), 0);
    ASSERT_EQ(graph.GetSize(), 0);
  }
  {
    Clique graph(4);

    ASSERT_EQ(graph.GetEdgesCount(), 6);
    ASSERT_EQ(graph.GetSize(), 4);
    for (int i = 0; i < graph.GetSize(); ++i) {
      for (int j = 0; j < graph.GetSize(); ++j) {
        if (i == j) {
          ASSERT_EQ(graph.GetEdgeLength(i, j), 0);
        } else {
          ASSERT_EQ(graph.GetEdgeLength(i, j), 1);
        }
      }
    }
  }
  {
    std::vector<std::vector<Clique::Edge>> connections = {
        {Clique::Edge(1, 1), Clique::Edge(2, 1), Clique::Edge(3, 1)},
        {Clique::Edge(0, 1), Clique::Edge(2, 1), Clique::Edge(3, 1)},
        {Clique::Edge(0, 1), Clique::Edge(1, 1), Clique::Edge(3, 1)},
        {Clique::Edge(0, 1), Clique::Edge(1, 1), Clique::Edge(2, 1)},
    };

    Clique graph1(connections);
    Clique graph2(4);
    ASSERT_EQ(graph1.GetSize(), 4);
    ASSERT_EQ(graph1.GetEdgesCount(), 6);
    for (int from = 0; from < 4; ++from) {
      ASSERT_EQ(graph1.GetEdges(from).size(), 3);
      for (int to = 0; to < 4; ++to) {
        ASSERT_EQ(graph1.GetEdgeLength(from, to),
                  graph2.GetEdgeLength(from, to));
      }
    }
  }
}

TEST(Clique, GetEdgeLength) {
  for (int size = 0; size < 10; ++size) {
    Clique graph(size);

    for (int i = 0; i < size; ++i) {
      for (int j = 0; j < size; ++j) {
        if (i == j) {
          ASSERT_EQ(graph.GetEdgeLength(i, j), 0);
        } else {
          ASSERT_EQ(graph.GetEdgeLength(i, j), 1);
        }
      }
    }
  }
}

TEST(Clique, GetEdges) {
  {
    Clique graph;

    ASSERT_DEATH(graph.GetEdges(0), "");
  }
  {
    Clique graph(4);
    std::vector<std::vector<Clique::Edge>> connections = {
        {Clique::Edge(1, 1), Clique::Edge(2, 1), Clique::Edge(3, 1)},
        {Clique::Edge(0, 1), Clique::Edge(2, 1), Clique::Edge(3, 1)},
        {Clique::Edge(0, 1), Clique::Edge(1, 1), Clique::Edge(3, 1)},
        {Clique::Edge(0, 1), Clique::Edge(1, 1), Clique::Edge(2, 1)},
    };

    for (int i = 0; i < graph.GetSize(); ++i) {
      ASSERT_EQ(graph.GetEdges(i), connections[i]);
    }
  }
}

// returns std::nullopt if path is invalid
std::optional<int> GetPathLength(
    const Clique& graph,
    const std::vector<Clique::Edge>& path,
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

TEST(Clique, GetAnyPath) {
  {
    Clique graph(6);

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
    std::vector<std::vector<Clique::Edge>> connections = {
        {Clique::Edge(1, 1), Clique::Edge(2, 2), Clique::Edge(3, 3)},
        {Clique::Edge(0, 1), Clique::Edge(2, 3), Clique::Edge(3, 4)},
        {Clique::Edge(0, 2), Clique::Edge(1, 3), Clique::Edge(3, 5)},
        {Clique::Edge(0, 3), Clique::Edge(1, 4), Clique::Edge(2, 5)},
    };
    Clique graph(connections);

    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        if (i != j) {
          ASSERT_EQ(GetPathLength(graph, graph.GetAnyPath(i, j), i), i + j);
        } else {
          ASSERT_TRUE(graph.GetAnyPath(i, j).empty());
        }
      }
    }
  }
}

TEST(Clique, GetShortestPath) {
  {
    Clique graph(6);

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
    std::vector<std::vector<Clique::Edge>> connections = {
        {Clique::Edge(1, 41), Clique::Edge(2, 21)},
        {Clique::Edge(0, 41), Clique::Edge(2, 32)},
        {Clique::Edge(0, 21), Clique::Edge(1, 32)},
    };

    Clique graph(connections);

    for (int from = 0; from < 3; ++from) {
      for (int to = 0; to < 3; ++to) {
        ASSERT_EQ(GetPathLength(graph,
                                graph.GetShortestPath(from, to),
                                from).value(),
                  graph.GetEdgeLength(from, to));
      }
    }
  }
}

TEST(Clique, GetShortestPaths) {
  {
    Clique graph(6);

    for (int from = 0; from < 6; ++from) {
      auto paths = graph.GetShortestPaths(from);
      std::vector<std::vector<Clique::Edge>> expected(6);
      for (int to = 0; to < 6; ++to) {
        expected[to] = graph.GetShortestPath(from, to);
      }
      ASSERT_EQ(paths, expected);
    }
  }
  {
    std::vector<std::vector<Clique::Edge>> connections = {
        {Clique::Edge(1, 41), Clique::Edge(2, 21)},
        {Clique::Edge(0, 41), Clique::Edge(2, 32)},
        {Clique::Edge(0, 21), Clique::Edge(1, 32)},
    };

    Clique graph(connections);

    for (int from = 0; from < 3; ++from) {
      auto paths = graph.GetShortestPaths(from);
      std::vector<std::vector<Clique::Edge>> expected(3);
      for (int to = 0; to < 3; ++to) {
        expected[to] = graph.GetShortestPath(from, to);
      }
      ASSERT_EQ(paths, expected);
    }
  }
}

TEST(Clique, GetEdgesCount) {
  {
    Clique graph;

    ASSERT_EQ(graph.GetEdgesCount(), 0);
  }
  {
    Clique graph(6);

    ASSERT_EQ(graph.GetEdgesCount(), 15);
  }
  {
    std::vector<std::vector<Clique::Edge>> connections = {
        {Clique::Edge(1, 41), Clique::Edge(2, 21)},
        {Clique::Edge(0, 41), Clique::Edge(2, 32)},
        {Clique::Edge(0, 21), Clique::Edge(1, 32)},
    };

    Clique graph(connections);

    ASSERT_EQ(graph.GetEdgesCount(), 3);
  }
}
