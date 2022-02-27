#pragma once

#include "abstract_graph.h"

class Chain : public AbstractGraph{
 public:
  Chain() = default;
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

  std::vector<int> dist_prefix_;
  std::vector<int> from_input_to_internal_;
  std::vector<int> from_internal_to_input_;
};


