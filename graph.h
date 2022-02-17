#pragma once

#include <vector>

class Graph {
 public:
  struct Edge {
    Edge(int to_, int length_);

    bool operator==(const Edge& rhs) const = default;

    int to;
    int length;
  };

  Graph() = default;
  explicit Graph(std::vector<std::vector<Edge>> list);
  // creates complete graph with n vertices
  explicit Graph(int n);

  int GetEdgeLength(int from, int to) const;
  const std::vector<Edge>& GetEdges(int from) const;

  // returns vertices count
  int GetSize() const;

  // uses BFS algorithm
  std::vector<Edge> GetAnyPath(int from, int to) const;
  std::vector<Edge> GetShortestPath(int from, int to) const;
  std::vector<std::vector<Edge>> GetShortestPaths(int from);

 private:
  // uses Dijkstra's algorithm
  std::vector<std::pair<Edge, int>> GenerateShortestPathAncestors(
      int from) const;
  static std::vector<Edge> RestorePath(
      const std::vector<std::pair<Edge, int>>& ancestors,
      int to);

  std::vector<std::vector<Edge>> connections_;
  int n_{0};
};

