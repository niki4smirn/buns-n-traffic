#pragma once

#include <vector>

#include "abstract_graph.h"

class Graph : public AbstractGraph {
 public:
  Graph() = default;
  explicit Graph(std::vector<std::vector<Edge>> list);
  // creates complete graph with n vertices
  explicit Graph(int n);

  const std::vector<Edge>& GetEdges(int from) const override;

  // returns vertices count
  int GetEdgesCount() const override;

  // uses BFS algorithm
  std::vector<Edge> GetAnyPath(int from, int to) const override;
  std::vector<Edge> GetShortestPath(int from, int to) const override;
  std::vector<std::vector<Edge>> GetShortestPaths(int from) const override;

 private:
  // uses Dijkstra's algorithm
  std::vector<std::pair<Edge, int>> GenerateShortestPathAncestors(
      int from) const;
  std::vector<std::pair<Edge, int>> DijkstraForDense(int from) const;
  std::vector<std::pair<Edge, int>> DijkstraForSparse(int from) const;
  static std::vector<Edge> RestorePath(
      const std::vector<std::pair<Edge, int>>& ancestors,
      int to);

  std::vector<std::vector<Edge>> connections_;
};

