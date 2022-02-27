#pragma once

#include <vector>

class AbstractGraph {
 public:
  struct Edge {
    Edge(int to_, int length_);

    bool operator==(const Edge& rhs) const = default;

    int to;
    int length;
  };

  AbstractGraph() = default;
  explicit AbstractGraph(int n);

  virtual std::vector<Edge> GetEdges(int from) const = 0;
  int GetEdgeLength(int from, int to) const;

  virtual int GetEdgesCount() const = 0;

  virtual std::vector<Edge> GetAnyPath(int from, int to) const = 0;
  virtual std::vector<Edge> GetShortestPath(int from, int to) const = 0;

  virtual std::vector<std::vector<Edge>> GetShortestPaths(int from) const = 0;

  int GetSize() const;

  virtual ~AbstractGraph() = default;

 protected:
  int n_{0};
};
