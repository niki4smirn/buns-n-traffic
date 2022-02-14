#include <cassert>
#include <limits>
#include <utility>
#include <queue>
#include <map>

#include "graph.h"

Graph::Edge::Edge(int to_, int length_) : to(to_), length(length_) {}

Graph::Graph(std::vector<std::vector<Edge>> list)
    : connections_(std::move(list)), n_(connections_.size()) {}

Graph::Graph(int n) : n_(n) {
  connections_.resize(n);

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (j != i) {
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
  return n_;
}

std::vector<Graph::Edge> Graph::GetAnyPath(int from, int to) const {
  // stores vertices, that will be explored later on
  std::queue<Edge> q;
  // stores vertices, that were explored
  std::vector<bool> is_used(n_, false);
  // used to restore path
  std::vector<std::pair<Edge, int>>
      previous(n_, std::make_pair(Edge(-1, -1), -1));
  std::vector<Edge> path;

  is_used[from] = true;

  q.push(Edge(from, -1));

  while (!q.empty()) {
    Edge temp = q.front();
    q.pop();

    for (const auto& edge : connections_[temp.to]) {
      if (!is_used[edge.to]) {
        is_used[edge.to] = true;
        q.push(edge);
        previous[edge.to] = std::make_pair(edge, temp.to);
      }
    }
  }

  if (is_used[to]) {
    for (int i = to; previous[i].second != -1; i = previous[i].second) {
      path.push_back(previous[i].first);
    }

    std::reverse(path.begin(), path.end());
  }

  return path;
}

std::vector<Graph::Edge> Graph::RestorePath(
    const std::vector<std::pair<Edge, int>>& ancestors,
    int to) {
  std::vector<Edge> path;

  for (int i = to; ancestors[i].second != -1; i = ancestors[i].second) {
    path.push_back(ancestors[i].first);
  }

  std::reverse(path.begin(), path.end());

  return path;
}

std::vector<std::pair<Graph::Edge, int>> Graph::CreateAncestors(
    int from) const {
  const int kInf = std::numeric_limits<int>::max();

  // stores vertices, that were explored
  std::vector<bool> is_used(n_, false);
  // used to restore path
  std::vector<std::pair<Edge, int>>
      ancestors(n_, std::make_pair(Edge(-1, -1), -1));
  // dist[i] stores distance from 'from'-th vertex to i-th
  std::vector<int> dist(n_, kInf);

  dist[from] = 0;

  for (int i = 0; i < n_; ++i) {
    int vertex = -1;
    for (int j = 0; j < n_; ++j) {
      if ((!is_used[j]) && (vertex == -1 || dist[j] < dist[vertex])) {
        vertex = j;
      }
    }
    if (dist[vertex] == kInf) {
      break;
    }

    is_used[vertex] = true;

    for (const auto& edge : connections_[vertex]) {
      if (dist[vertex] + edge.length < dist[edge.to]) {
        dist[edge.to] = dist[vertex] + edge.length;
        ancestors[edge.to] = std::make_pair(edge, vertex);
      }
    }
  }

  return ancestors;
}

std::vector<Graph::Edge> Graph::GetShortestPath(int from, int to) const {
  return RestorePath(CreateAncestors(from), to);
}

std::vector<std::vector<Graph::Edge>> Graph::GetShortestPaths(int from) {
  auto ancestors = CreateAncestors(from);

  std::vector<std::vector<Edge>> paths;
  paths.reserve(n_);

  for (int i = 0; i < n_; ++i) {
    paths.push_back(RestorePath(ancestors, i));
  }
  
  return paths;
}
