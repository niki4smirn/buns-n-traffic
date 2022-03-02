#include <benchmark/benchmark.h>
#include <iostream>

#include "../traffic_manager.h"
#include "../graph.h"
#include "../clique.h"

static void RunTransport(benchmark::State& state) {
  for (auto _: state) {
    int64_t iters_count = state.range(0);
    Graph graph(2);
    TrafficManager traffic_manager(&graph, {42, 31}, {42, 1}, 42);
    for (int i = 0; i < iters_count; ++i) {
      traffic_manager.Transport(0, 1, 42);
      traffic_manager.Transport(1, 0, 42);
    }
  }
}

BENCHMARK(RunTransport)->Unit(benchmark::kMillisecond)->Arg(50000)->Arg(100000)->Arg(500000);

BENCHMARK_MAIN();