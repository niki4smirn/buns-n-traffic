#include "chain.h"

#include <cassert>

Chain::Chain(int n) : AbstractGraph(n) {
  ResizeInternalVectors(n_);
  for (int i = 0; i < n_; ++i) {
    dist_prefix_[i] = i;
    AddMappingPair(i, i);
  }
}

int GetBoundIndex(
    const std::vector<std::vector<Chain::Edge>>& list) {
  int cur_index = 0;
  std::vector<bool> used(list.size());
  while (list[cur_index].size() == 2) {
    used[cur_index] = true;
    int next_index = list[cur_index][0].to;
    if (used[next_index]) {
      next_index = list[cur_index][1].to;
    }
    cur_index = next_index;
  }
  return cur_index;
}

Chain::Chain(const std::vector<std::vector<Edge>>& list) :
    AbstractGraph(list.size()){
  for (int i = 0; i < n_; ++i) {
    assert(list[i].size() == 1 ||
        list[i].size() == 2);
  }
  if (n_ != 0) {
    std::vector<bool> used(n_);
    int bound_index = GetBoundIndex(list);
    used.assign(n_, false);
    FillInternalVectors(bound_index, list);
  }
}

void Chain::ResizeInternalVectors(int size) {
  dist_prefix_.resize(size);
  from_input_to_internal_.resize(size);
  from_internal_to_input_.resize(size);
}

void Chain::FillInternalVectors(
    int start_index,
    const std::vector<std::vector<Edge>>& list) {
  ResizeInternalVectors(n_);
  int internal_index = 0;
  AddMappingPair(start_index, internal_index);
  if (list.size() == 1) {
    return;
  }

  std::vector<bool> used(n_);
  int cur_index = start_index;
  do {
    used[cur_index] = true;
    Edge next_edge = list[cur_index][0];
    if (used[next_edge.to]) {
      next_edge = list[cur_index][1];
    }
    cur_index = next_edge.to;

    dist_prefix_[internal_index + 1] =
        dist_prefix_[internal_index] + next_edge.length;
    ++internal_index;
    AddMappingPair(cur_index, internal_index);
  } while (list[cur_index].size() == 2);
}

void Chain::AddMappingPair(int input_index, int internal_index) {
  from_input_to_internal_[input_index] = internal_index;
  from_internal_to_input_[internal_index] = input_index;
}

Chain::Chain(const std::vector<int>& edges_len_list) :
    AbstractGraph(edges_len_list.size() + 1) {
  ResizeInternalVectors(n_);
  for (int i = 0; i < n_; ++i) {
    AddMappingPair(i, i);
    if (i > 0) {
      dist_prefix_[i] = edges_len_list[i - 1] + dist_prefix_[i - 1];
    }
  }
}
