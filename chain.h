#pragma once

#include <optional>

#include "abstract_graph.h"

class Chain : public AbstractGraph{
 public:
  Chain() = default;
  // creates chain graph with n vertices and every edge's len = 1
  explicit Chain(int n);
  explicit Chain(const std::vector<std::vector<Edge>>& list);
  explicit Chain(const std::vector<int>& edges_len_list);


  std::vector<Edge> GetEdges(int from) const override;

  int GetEdgesCount() const override;

  std::vector<Edge> GetAnyPath(int from, int to) const override;
  std::vector<Edge> GetShortestPath(int from, int to) const override;
  std::vector<std::vector<Edge>> GetShortestPaths(int from) const override;

 private:
  void ResizeInternalVectors(int size);
  void FillInternalVectors(int cur_index,
                           const std::vector<std::vector<Edge>>& list);
  void AddMappingPair(int input_index, int internal_index);

  struct ChainNode {
    std::optional<int> left_len{std::nullopt};
    std::optional<int> right_len{std::nullopt};
  };

  std::vector<ChainNode> nodes_list_;
  std::vector<int> from_input_to_internal_;
  std::vector<int> from_internal_to_input_;
};
