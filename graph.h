#pragma once

#include <vector>

class Graph {
 public:
  struct Edge {
    Edge(int to_, int length_);
    int to;
    int length;
  };

  Graph() = default;
  explicit Graph(std::vector<std::vector<Edge>> list);
  explicit Graph(int n);  // creates complete graph with n vertices

  int GetEdgeLength(int from, int to) const;
  const std::vector<Edge>& GetEdges(int from) const;

  int GetSize() const;  // returns vertices count

  std::vector<Edge> GetAnyPath(int from, int to) const;  // uses BFS algorithm
  std::vector<Edge> GetShortestPath(int from, int to) const;
  std::vector<std::vector<Edge>> GetShortestPaths(int from);

 private:
  std::vector<std::vector<Edge>> connections_{};
  int n_{0};
};

