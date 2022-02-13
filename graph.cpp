#include <utility>

#include "graph.h"

Graph::Edge::Edge(int to_, int length_) : to(to_), length(length_) {}

Graph::Graph(std::vector<std::vector<Edge>> list)
    : connections_(std::move(list)), n_(connections_.size()) {}

Graph::Graph(int a) : n_(a) {
  connections_.resize(a);

  for (int i = 0; i < a; ++i) {
    for (int j = 0; j < a; ++j) {
      if (j != a) {
        connections_[i].emplace_back(j, 1);
      }
    }
  }
}
