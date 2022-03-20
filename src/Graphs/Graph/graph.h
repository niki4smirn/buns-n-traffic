#pragma once

#include <vector>

#include "../AbstractGraph/abstract_graph.h"

class Graph : public AbstractGraph {
 public:
  Graph() = default;
  explicit Graph(std::vector<std::vector<Edge>> list);
  // creates complete graph with n vertices
  explicit Graph(int n);

  std::vector<Edge> GetEdges(int from) const override;

  int GetEdgesCount() const override;

  std::vector<Edge> GetAnyPath(int from, int to) const override;
  std::vector<Edge> GetShortestPath(int from, int to) const override;
  std::vector<std::vector<Edge>> GetShortestPaths(int from) const override;

 private:
  std::vector<std::pair<Edge, int>> GenerateShortestPathAncestors(
      int from) const;
  std::vector<std::pair<Edge, int>> DijkstraForDense(int from) const;
  std::vector<std::pair<Edge, int>> DijkstraForSparse(int from) const;
  static std::vector<Edge> RestorePath(
      const std::vector<std::pair<Edge, int>>& ancestors,
      int to);

  std::vector<std::vector<Edge>> connections_;
};

