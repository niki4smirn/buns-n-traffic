#include "abstract_graph.h"

#include <cassert>

AbstractGraph::AbstractGraph(int n) : n_(n) {}

AbstractGraph::Edge::Edge(int to_, int length_) : to(to_), length(length_) {}

int AbstractGraph::GetEdgeLength(int from, int to) const {
  assert(0 <= from && from < n_);
  assert(0 <= to && to < n_);

  auto from_edges = GetEdges(from);
  auto edge = std::find_if(from_edges.begin(),
                           from_edges.end(),
                           [to](Edge edge) {
                             return edge.to == to;
                           });

  if (edge == from_edges.end()) {
    return 0;
  }

  return edge->length;
}

int AbstractGraph::GetSize() const {
  return n_;
}
