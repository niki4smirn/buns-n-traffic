#pragma once

#include "abstract_graph.h"

class Clique : public AbstractGraph {
 public:
  Clique() = default;
  explicit Clique(int n);
  explicit Clique(const std::vector<std::vector<Edge>>& list);
  explicit Clique(const std::vector<std::vector<int>>& adjacency_matrix);

  std::vector<Edge> GetEdges(int from) const override;

  int GetEdgesCount() const override;

  std::vector<Edge> GetAnyPath(int from, int to) const override;
  std::vector<Edge> GetShortestPath(int from, int to) const override;
  std::vector<std::vector<Edge>> GetShortestPaths(int from) const override;

 private:
  std::vector<int> GenerateShortestPathAncestors(int from) const;
  std::vector<Edge> RestorePath(
      const std::vector<int>& ancestors,
      int to) const;

  std::vector<std::vector<int>> adjacency_matrix_;
};
