#include "clique.h"

#include <cassert>

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