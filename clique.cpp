#include "clique.h"

#include <cassert>
#include <limits>

Clique::Clique(int n) : AbstractGraph(n) {
  adjacency_matrix_.resize(n);
  for (int i = 0; i < n; ++i) {
    adjacency_matrix_[i].resize(n);
    for (int j = 0; j < n; ++j) {
      if (i != j) {
        adjacency_matrix_[i][j] = 1;
      }
    }

  }
}

Clique::Clique(const std::vector<std::vector<Edge>>& list) :
    Clique(list.size()) {
  for (int i = 0; i < list.size(); ++i) {
    assert(list[i].size() + 1 == list.size());
    for (int j = 0; j < list[i].size(); ++j) {
      auto edge = list[i][j];
      adjacency_matrix_[i][edge.to] = edge.length;
    }
  }
}

Clique::Clique(const std::vector<std::vector<int>>& adjacency_matrix) :
    AbstractGraph(adjacency_matrix.size()) {
  for (int i = 0; i < adjacency_matrix.size(); ++i) {
    assert(adjacency_matrix[i].size() == adjacency_matrix.size());
  }
  adjacency_matrix_ = adjacency_matrix;
}

std::vector<Clique::Edge> Clique::GetEdges(int from) const {
  assert(0 <= from && from < adjacency_matrix_.size());
  std::vector<Clique::Edge> result;
  result.reserve(n_ - 1);
  for (int i = 0; i < adjacency_matrix_[from].size(); ++i) {
    if (i != from) {
      result.emplace_back(i, adjacency_matrix_[from][i]);
    }
  }
  return result;
}

int Clique::GetEdgesCount() const {
  return n_ * (n_ - 1) / 2;
}

std::vector<Clique::Edge> Clique::GetAnyPath(int from, int to) const {
  if (from == to) {
    return {};
  }
  return {{to, adjacency_matrix_[from][to]}};
}

std::vector<int> Clique::GenerateShortestPathAncestors(int from) const {
  const int kInf = std::numeric_limits<int>::max();
  std::vector<int> distance(n_, kInf);
  std::vector<int> ancestors(n_, -1);
  std::vector<bool> is_used(n_, false);
  distance[from] = 0;
  for (int i = 0; i < n_; ++i) {
    int cur_vertex = -1;
    for (int new_best = 0; new_best < n_; ++new_best) {
      if (!is_used[new_best] &&
          (cur_vertex == -1 || distance[new_best] < distance[cur_vertex])) {
        cur_vertex = new_best;
      }
    }
    if (distance[cur_vertex] == kInf) {
      break;
    }
    is_used[cur_vertex] = true;

    for (int to = 0; to < n_; ++to) {
      int len = adjacency_matrix_[cur_vertex][to];
      if (distance[cur_vertex] + len < distance[to]) {
        distance[to] = distance[cur_vertex] + len;
        ancestors[to] = cur_vertex;
      }
    }
  }
  return ancestors;
}

std::vector<Clique::Edge> Clique::RestorePath(
    const std::vector<int>& ancestors,
    int to) const {
  std::vector<Edge> path;
  int cur = to;
  while (ancestors[cur] != -1) {
    path.emplace_back(cur, adjacency_matrix_[ancestors[cur]][cur]);
    cur = ancestors[cur];
  }
  std::reverse(path.begin(), path.end());
  return path;
}

std::vector<Clique::Edge> Clique::GetShortestPath(int from, int to) const {
  assert(0 <= from && from < n_);
  assert(0 <= to && to < n_);
  return RestorePath(GenerateShortestPathAncestors(from), to);
}

std::vector<std::vector<Clique::Edge>> Clique::GetShortestPaths(
    int from) const {
  assert(0 <= from && from < n_);
  auto ancestors = GenerateShortestPathAncestors(from);
  std::vector<std::vector<Edge>> res;
  res.reserve(n_);
  for (int to = 0; to < n_; ++to) {
    res.push_back(RestorePath(ancestors, to));
  }
  return res;
}
