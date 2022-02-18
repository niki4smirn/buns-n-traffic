#include <cassert>
#include <cmath>
#include <limits>
#include <numeric>
#include <queue>
#include <utility>

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
  assert(0 <= from && from < n_);
  assert(0 <= to && to < n_);

  auto edge = std::find_if(GetEdges(from).begin(),
                           GetEdges(from).end(),
                           [to](Edge edge) {
                             return edge.to == to;
                           });

  if (edge == GetEdges(from).end()) {
    return 0;
  }

  return edge->length;
}

const std::vector<Graph::Edge>& Graph::GetEdges(int from) const {
  assert(0 <= from && from < n_);

  return connections_[from];
}

int Graph::GetSize() const {
  return n_;
}

std::vector<Graph::Edge> Graph::GetAnyPath(int from, int to) const {
  assert(0 <= from && from < n_);
  assert(0 <= to && to < n_);

  // stores vertices, that will be explored later on
  std::queue<Edge> q;
  // stores vertices, that were explored
  std::vector<bool> is_used(n_, false);
  // used to restore path
  std::vector<std::pair<Edge, int>>
      ancestors(n_, std::make_pair(Edge(-1, -1), -1));

  is_used[from] = true;

  q.push(Edge(from, -1));

  while (!q.empty()) {
    Edge temp = q.front();
    q.pop();

    for (const auto& edge : GetEdges(temp.to)) {
      if (!is_used[edge.to]) {
        is_used[edge.to] = true;
        q.push(edge);
        ancestors[edge.to] = std::make_pair(edge, temp.to);
      }
    }
  }

  return RestorePath(ancestors, to);
}

std::vector<Graph::Edge> Graph::RestorePath(
    const std::vector<std::pair<Edge, int>>& ancestors,
    int to) {
  assert(0 <= to && to < ancestors.size());

  std::vector<Edge> path;

  for (int i = to; ancestors[i].second != -1; i = ancestors[i].second) {
    path.push_back(ancestors[i].first);
  }

  std::reverse(path.begin(), path.end());

  return path;
}

std::vector<std::pair<Graph::Edge, int>> Graph::GenerateShortestPathAncestors(
    int from) const {
  int edges_count = GetEdgesCount();

  if (n_ * n_ + edges_count < edges_count * std::log(n_)) {
    return DijkstraForDense(from);
  } else {
    return DijkstraForSparse(from);
  }
}

std::vector<Graph::Edge> Graph::GetShortestPath(int from, int to) const {
  assert(0 <= from && from < n_);
  assert(0 <= to && to < n_);

  return RestorePath(GenerateShortestPathAncestors(from), to);
}

std::vector<std::vector<Graph::Edge>> Graph::GetShortestPaths(int from) {
  assert(0 <= from && from < n_);

  auto ancestors = GenerateShortestPathAncestors(from);

  std::vector<std::vector<Edge>> paths;
  paths.reserve(n_);

  for (int i = 0; i < n_; ++i) {
    paths.push_back(RestorePath(ancestors, i));
  }

  return paths;
}

int Graph::GetEdgesCount() const {
  return std::accumulate(connections_.begin(),
                         connections_.end(),
                         0,
                         [](int init, const auto& edges) {
                           return init + edges.size();
                         }) / 2;
}

std::vector<std::pair<Graph::Edge, int>> Graph::DijkstraForSparse(
    int from) const {
  assert(0 <= from && from < n_);

  const int kInf = std::numeric_limits<int>::max();

  // stores vertices, that will be explored later on
  std::priority_queue<std::pair<int, int>,
                      std::vector<std::pair<int, int>>,
                      std::greater<>> p_q;
  std::vector<bool> is_used(n_, false);
  // used to restore path
  std::vector<std::pair<Edge, int>>
      ancestors(n_, std::make_pair(Edge(-1, -1), -1));
  // dist[i] stores distance from 'from'-th vertex to i-th
  std::vector<int> dist(n_, kInf);

  dist[from] = 0;
  p_q.push(std::make_pair(0, from));

  for (int i = 0; i < n_; ++i) {
    int vertex = p_q.top().second;

    p_q.pop();

    if (dist[vertex] == kInf) {
      // break;
    }

    for (const auto& edge : GetEdges(vertex)) {
      if (dist[vertex] + edge.length < dist[edge.to]) {
        dist[edge.to] = dist[vertex] + edge.length;
        ancestors[edge.to] = std::make_pair(edge, vertex);
        p_q.push(std::make_pair(dist[edge.to], edge.to));
      }
    }
  }

  return ancestors;
}

std::vector<std::pair<Graph::Edge, int>> Graph::DijkstraForDense(
    int from) const {
  assert(0 <= from && from < n_);

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

    for (const auto& edge : GetEdges(vertex)) {
      if (dist[vertex] + edge.length < dist[edge.to]) {
        dist[edge.to] = dist[vertex] + edge.length;
        ancestors[edge.to] = std::make_pair(edge, vertex);
      }
    }
  }

  return ancestors;
}
