#include <cassert>
#include <cmath>
#include <limits>
#include <numeric>
#include <queue>
#include <utility>

#include "graph.h"

Graph::Graph(std::vector<std::vector<Edge>> list)
    : AbstractGraph(list.size()), connections_(std::move(list)) {}

Graph::Graph(int n) : AbstractGraph(n) {
  connections_.resize(n);

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (j != i) {
        connections_[i].emplace_back(j, 1);
      }
    }
  }
}


std::vector<Graph::Edge> Graph::GetEdges(int from) const {
  assert(0 <= from && from < n_);

  return connections_[from];
}

std::vector<Graph::Edge> Graph::GetAnyPath(int from, int to) const {
  assert(0 <= from && from < n_);
  assert(0 <= to && to < n_);

  // stores edges to vertices, that will be explored later on
  std::queue<Edge> edges_queue;
  // stores vertices, that were explored
  std::vector<bool> is_used(n_, false);
  // used to restore path
  std::vector<std::pair<Edge, int>>
      ancestors(n_, std::make_pair(Edge(-1, -1), -1));

  is_used[from] = true;

  edges_queue.push(Edge(from, -1));

  while (!edges_queue.empty()) {
    Edge temp = edges_queue.front();
    edges_queue.pop();

    for (const auto& edge : GetEdges(temp.to)) {
      if (!is_used[edge.to]) {
        is_used[edge.to] = true;
        edges_queue.push(edge);
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

std::vector<std::vector<Graph::Edge>> Graph::GetShortestPaths(
    int from)  const {
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
                      std::greater<>> vertices_queue;
  // used to restore path
  std::vector<std::pair<Edge, int>>
      ancestors(n_, std::make_pair(Edge(-1, -1), -1));
  // dist[i] stores distance from 'from'-th vertex to i-th
  std::vector<int> dist(n_, kInf);

  dist[from] = 0;
  vertices_queue.push(std::make_pair(0, from));

  for (int i = 0; i < n_; ++i) {
    int vertex = vertices_queue.top().second;

    vertices_queue.pop();

    for (const auto& edge : GetEdges(vertex)) {
      if (dist[vertex] + edge.length < dist[edge.to]) {
        dist[edge.to] = dist[vertex] + edge.length;
        ancestors[edge.to] = std::make_pair(edge, vertex);
        vertices_queue.push(std::make_pair(dist[edge.to], edge.to));
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
