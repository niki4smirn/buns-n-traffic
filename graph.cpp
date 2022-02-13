#include <cassert>
#include <numeric>
#include <utility>

#include "graph.h"

Graph::Edge::Edge(int to_, int length_) : to(to_), length(length_) {}

Graph::Graph(std::vector<std::vector<Edge>> list)
    : connections_(std::move(list)), n_(connections_.size()) {}

Graph::Graph(int n) : n_(n) {
  connections_.resize(n);

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (j != n) {
        connections_[i].emplace_back(j, 1);
      }
    }
  }
}

int Graph::GetEdgeLength(int from, int to) const {
  assert(from < n_ && from >= 0);
  assert(to < n_ && to >= 0);

  auto edge = std::find_if(connections_[from].begin(),
                           connections_[from].end(),
                           [to](Edge edge) {
                             return edge.to == to;
                           });
  if (edge != connections_[from].end()) {
    return edge->length;
  } else {
    return 0;
  }
}

const std::vector<Graph::Edge>& Graph::GetEdges(int from) const {
  assert(from < n_ && from >= 0);

  return connections_[from];
}

int Graph::GetSize() const {
  return std::accumulate(connections_.begin(),
                         connections_.end(),
                         0,
                         [](int init, const std::vector<Edge>& line) {
                           return line.size() + init;
                         });
}
