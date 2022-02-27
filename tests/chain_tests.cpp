#include "../chain.h"
#include "gtest/gtest.h"

TEST(Chain, Constructors) {
  {
    Chain graph;

    ASSERT_EQ(graph.GetEdgesCount(), 0);
    ASSERT_EQ(graph.GetSize(), 0);
  }
  {
    std::vector<std::vector<Chain::Edge>> connections = {};

    Chain graph(connections);

    ASSERT_EQ(graph.GetEdgesCount(), 0);
    ASSERT_EQ(graph.GetSize(), 0);
  }
  {
    std::vector<int> edges_len_list = {};

    Chain graph(edges_len_list);

    ASSERT_EQ(graph.GetEdgesCount(), 0);
    ASSERT_EQ(graph.GetSize(), 1);
  }
  {
    Chain graph(0);

    ASSERT_EQ(graph.GetEdgesCount(), 0);
    ASSERT_EQ(graph.GetSize(), 0);
  }
  {
    Chain graph(4);

    ASSERT_EQ(graph.GetEdgesCount(), 3);
    ASSERT_EQ(graph.GetSize(), 4);
    for (int from = 0; from < graph.GetSize(); ++from) {
      if (0 < from && from + 1 < graph.GetSize()) {
        ASSERT_EQ(graph.GetEdges(from).size(), 2);
      } else {
        ASSERT_EQ(graph.GetEdges(from).size(), 1);
      }
      if (from > 0) {
        ASSERT_EQ(graph.GetEdgeLength(from, from - 1), 1);
      }
      if (from + 1 < graph.GetSize()) {
        ASSERT_EQ(graph.GetEdgeLength(from, from + 1), 1);
      }
    }
  }
  {
    std::vector<std::vector<Chain::Edge>> connections = {
        {Chain::Edge(1, 1), Chain::Edge(2, 2)},
        {Chain::Edge(0, 1)},
        {Chain::Edge(0, 2), Chain::Edge(3, 5)},
        {Chain::Edge(2, 5)},
    };

    Chain graph(connections);

    ASSERT_EQ(graph.GetEdgesCount(), 3);
    ASSERT_EQ(graph.GetSize(), 4);

    ASSERT_EQ(graph.GetEdges(1).size(), 1);
    ASSERT_EQ(graph.GetEdgeLength(1, 0), 1);

    ASSERT_EQ(graph.GetEdges(0).size(), 2);
    ASSERT_EQ(graph.GetEdgeLength(0, 1), 1);
    ASSERT_EQ(graph.GetEdgeLength(0, 2), 2);

    ASSERT_EQ(graph.GetEdges(2).size(), 2);
    ASSERT_EQ(graph.GetEdgeLength(2, 0), 2);
    ASSERT_EQ(graph.GetEdgeLength(2, 3), 5);

    ASSERT_EQ(graph.GetEdges(3).size(), 1);
    ASSERT_EQ(graph.GetEdgeLength(3, 2), 5);
  }
}

Chain GenerateTestChain() {
  std::vector<std::vector<Chain::Edge>> connections = {
      {Chain::Edge(1, 1), Chain::Edge(2, 2)},
      {Chain::Edge(0, 1)},
      {Chain::Edge(0, 2), Chain::Edge(3, 5)},
      {Chain::Edge(2, 5)},
  };
  return Chain{connections};
}

TEST(Chain, GetEdgeLength) {
  Chain graph = GenerateTestChain();

  ASSERT_EQ(graph.GetEdgeLength(1, 0), 1);
  ASSERT_EQ(graph.GetEdgeLength(1, 1), 0);
  ASSERT_EQ(graph.GetEdgeLength(1, 2), 0);
  ASSERT_EQ(graph.GetEdgeLength(1, 3), 0);

  ASSERT_EQ(graph.GetEdgeLength(0, 1), 1);
  ASSERT_EQ(graph.GetEdgeLength(0, 2), 2);
  ASSERT_EQ(graph.GetEdgeLength(0, 0), 0);
  ASSERT_EQ(graph.GetEdgeLength(0, 3), 0);

  ASSERT_EQ(graph.GetEdgeLength(2, 0), 2);
  ASSERT_EQ(graph.GetEdgeLength(2, 3), 5);
  ASSERT_EQ(graph.GetEdgeLength(2, 1), 0);
  ASSERT_EQ(graph.GetEdgeLength(2, 2), 0);

  ASSERT_EQ(graph.GetEdgeLength(3, 2), 5);
  ASSERT_EQ(graph.GetEdgeLength(3, 0), 0);
  ASSERT_EQ(graph.GetEdgeLength(3, 1), 0);
  ASSERT_EQ(graph.GetEdgeLength(3, 3), 0);
}

TEST(Chain, GetEdges) {
  {
    Chain graph;

    ASSERT_DEATH(graph.GetEdges(0), "");
  }
  {
    std::vector<std::vector<Chain::Edge>> connections = {
        {Chain::Edge(1, 1), Chain::Edge(2, 2)},
        {Chain::Edge(0, 1)},
        {Chain::Edge(0, 2), Chain::Edge(3, 5)},
        {Chain::Edge(2, 5)},
    };

    Chain graph(connections);

    for (int i = 0; i < graph.GetSize(); ++i) {
      ASSERT_EQ(graph.GetEdges(i), connections[i]);
    }
  }
}

// returns std::nullopt if path is invalid
std::optional<int> GetPathLength(
    const Chain& graph,
    const std::vector<Chain::Edge>& path,
    int from) {
  int distance = 0;

  for (const auto& edge: path) {
    if (graph.GetEdgeLength(from, edge.to) == edge.length) {
      distance += edge.length;
      from = edge.to;
    } else {
      return std::nullopt;
    }
  }

  return distance;
}

TEST(Chain, GetAnyPath) {
  {
    Chain graph = GenerateTestChain();

    ASSERT_EQ(GetPathLength(graph, graph.GetAnyPath(1, 0), 1), 1);
    ASSERT_TRUE(graph.GetAnyPath(1, 1).empty());
    ASSERT_EQ(GetPathLength(graph, graph.GetAnyPath(1, 2), 1), 3);
    ASSERT_EQ(GetPathLength(graph, graph.GetAnyPath(1, 3), 1), 8);

    ASSERT_TRUE(graph.GetAnyPath(0, 0).empty());
    ASSERT_EQ(GetPathLength(graph, graph.GetAnyPath(0, 1), 0), 1);
    ASSERT_EQ(GetPathLength(graph, graph.GetAnyPath(0, 2), 0), 2);
    ASSERT_EQ(GetPathLength(graph, graph.GetAnyPath(0, 3), 0), 7);

    ASSERT_EQ(GetPathLength(graph, graph.GetAnyPath(2, 0), 2), 2);
    ASSERT_EQ(GetPathLength(graph, graph.GetAnyPath(2, 1), 2), 3);
    ASSERT_TRUE(graph.GetAnyPath(2, 2).empty());
    ASSERT_EQ(GetPathLength(graph, graph.GetAnyPath(2, 3), 2), 5);

    ASSERT_EQ(GetPathLength(graph, graph.GetAnyPath(3, 0), 3), 7);
    ASSERT_EQ(GetPathLength(graph, graph.GetAnyPath(3, 1), 3), 8);
    ASSERT_EQ(GetPathLength(graph, graph.GetAnyPath(3, 2), 3), 5);
    ASSERT_TRUE(graph.GetAnyPath(3, 3).empty());
  }
}

TEST(Chain, GetShortestPath) {
  {
    Chain graph(6);

    for (int from = 0; from < 6; ++from) {
      for (int to = 0; to < 6; ++to) {
        ASSERT_EQ(graph.GetShortestPath(from, to),
                  graph.GetAnyPath(from, to));
      }
    }
  }
  {
    Chain graph = GenerateTestChain();

    for (int from = 0; from < graph.GetSize(); ++from) {
      for (int to = 0; to < graph.GetSize(); ++to) {
        ASSERT_EQ(graph.GetShortestPath(from, to),
                  graph.GetAnyPath(from, to));
      }
    }
  }
}

TEST(Chain, GetShortestPaths) {
  {
    Chain graph(6);

    for (int from = 0; from < 6; ++from) {
      auto paths = graph.GetShortestPaths(from);
      std::vector<std::vector<Chain::Edge>> expected(6);
      for (int to = 0; to < 6; ++to) {
        expected[to] = graph.GetShortestPath(from, to);
      }
      ASSERT_EQ(paths, expected);
    }
  }
  {
    Chain graph = GenerateTestChain();

    for (int from = 0; from < graph.GetSize(); ++from) {
      auto paths = graph.GetShortestPaths(from);
      std::vector<std::vector<Chain::Edge>> expected(graph.GetSize());
      for (int to = 0; to < graph.GetSize(); ++to) {
        expected[to] = graph.GetShortestPath(from, to);
      }
      ASSERT_EQ(paths, expected);
    }
  }
}

TEST(Chain, GetEdgesCount) {
  {
    Chain graph;

    ASSERT_EQ(graph.GetEdgesCount(), 0);
  }
  {
    std::vector<std::vector<Chain::Edge>> connections = {};

    Chain graph(connections);

    ASSERT_EQ(graph.GetEdgesCount(), 0);
  }
  {
    std::vector<int> edges_len_list = {};

    Chain graph(edges_len_list);

    ASSERT_EQ(graph.GetEdgesCount(), 0);
  }
  {
    Chain graph(4);

    ASSERT_EQ(graph.GetEdgesCount(), 3);
  }
  {
    Chain graph = GenerateTestChain();

    ASSERT_EQ(graph.GetEdgesCount(), 3);
  }
}
