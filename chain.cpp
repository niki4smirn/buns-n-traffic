#include "chain.h"

#include <cassert>

Chain::Chain(int n) : AbstractGraph(n) {
  ResizeInternalVectors(n_);
  for (int i = 0; i < n_; ++i) {
    if (i > 0) {
      nodes_list_[i].left_len = 1;
    }
    if (i + 1 < n) {
      nodes_list_[i].right_len = 1;
    }
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
  nodes_list_.resize(size);
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

    nodes_list_[internal_index].right_len = next_edge.length;
    ++internal_index;
    nodes_list_[internal_index].left_len = next_edge.length;
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
      nodes_list_[i].left_len = edges_len_list[i - 1];
    }
    if (i + 1 < n_) {
      nodes_list_[i].right_len = edges_len_list[i];
    }
  }
}

std::vector<Chain::Edge> Chain::GetEdges(int from) const {
  std::vector<Edge> result;
  int internal_from = from_input_to_internal_[from];
  if (internal_from > 0) {
    int to = from_internal_to_input_[internal_from - 1];
    result.emplace_back(to, nodes_list_[internal_from].left_len.value());
  }
  if (internal_from + 1 < n_) {
    int to = from_internal_to_input_[internal_from + 1];
    result.emplace_back(to, nodes_list_[internal_from].right_len.value());
  }
  return result;
}

int Chain::GetEdgesCount() const {
  return std::max(n_ - 1, 0);
}

std::vector<Chain::Edge> Chain::GetShortestPath(int from, int to) const {
  return GetAnyPath(from, to);
}

std::vector<std::vector<Chain::Edge>> Chain::GetShortestPaths(int from) const {
  std::vector<std::vector<Edge>> res;
  res.reserve(n_);
  for (int to = 0; to < n_; ++to) {
    res.push_back(GetShortestPath(from, to));
  }
  return res;
}

std::vector<Chain::Edge> Chain::GetAnyPath(int from, int to) const {
  if (from == to) {
    return {};
  }
  int internal_from = from_input_to_internal_[from];
  int internal_to = from_input_to_internal_[to];

  std::vector<Edge> res;
  if (internal_from < internal_to) {
    for (int cur = internal_from + 1; cur <= internal_to; ++cur) {
      res.emplace_back(from_internal_to_input_[cur],
                       nodes_list_[cur].left_len.value());
    }
  } else {
    for (int cur = internal_from - 1; cur >= internal_to; --cur) {
      res.emplace_back(from_internal_to_input_[cur],
                       nodes_list_[cur].right_len.value());
    }
  }
  return res;
}
