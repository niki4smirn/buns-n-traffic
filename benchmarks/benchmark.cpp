#include <benchmark/benchmark.h>
#include <iostream>
#include <random>

#include "../traffic_manager.h"
#include "../graph.h"
#include "../clique.h"
#include "../chain.h"

class RandomGenerator {
 public:
  RandomGenerator() = default;
  RandomGenerator(int64_t min_value, int64_t max_value);

  int64_t GetValue();
  std::vector<int> GetVector(int size);

 private:
  std::mt19937 gen{std::random_device()()};
  std::uniform_int_distribution<int64_t> distribution;
};

RandomGenerator::RandomGenerator(int64_t min_value, int64_t max_value) :
    distribution(std::uniform_int_distribution<int64_t>(min_value,
                                                        max_value)) {}

int64_t RandomGenerator::GetValue() {
  return distribution(gen);
}

std::vector<int> RandomGenerator::GetVector(int size) {
  std::vector<int> result(size);
  std::generate(result.begin(),
                result.end(),
                [&]() { return distribution(gen); });
  return result;
}

template<typename GraphClass>
static void BM_Transport(benchmark::State& state) {
  for (auto _: state) {
    state.PauseTiming();
    int graph_size = state.range(0);
    RandomGenerator gen(0, graph_size - 1);
    GraphClass graph(graph_size);
    std::vector<int> buns_amounts = gen.GetVector(graph_size);
    std::vector<int> vehicles = gen.GetVector(graph_size);
    int vehicle_capacity = gen.GetValue() + 1;
    TrafficManager traffic_manager(&graph,
                                   buns_amounts,
                                   vehicles,
                                   vehicle_capacity);
    int64_t iters_count = state.range(1);
    for (int i = 0; i < iters_count; ++i) {
      int from = gen.GetValue();
      int to = gen.GetValue();
      int buns_to_transport = gen.GetValue();
      int from_buns_amount = traffic_manager.GetBunsAmount(from);
      int from_vehicles = traffic_manager.GetVehicle(from);
      int max_possible = std::min(from_buns_amount,
                                  from_vehicles * vehicle_capacity);
      if (buns_to_transport <= max_possible) {
        state.ResumeTiming();
        traffic_manager.Transport(from, to, buns_to_transport);
        state.PauseTiming();
      }
    }
  }
}



int main(int argc, char** argv) {
  const int kGraphSize = 100;
  BENCHMARK(BM_Transport<Graph>)
      ->Unit(benchmark::kMillisecond)
      ->Args({kGraphSize,  50000})->Iterations(3)
      ->Args({kGraphSize, 100000})->Iterations(3)
      ->Args({kGraphSize, 500000})->Iterations(3)
      ;
  BENCHMARK(BM_Transport<Clique>)
      ->Unit(benchmark::kMillisecond)
      ->Args({kGraphSize,  50000})->Iterations(3)
      ->Args({kGraphSize, 100000})->Iterations(3)
      ->Args({kGraphSize, 500000})->Iterations(3)
  ;
  BENCHMARK(BM_Transport<Chain>)
      ->Unit(benchmark::kMillisecond)
      ->Args({kGraphSize,  50000})->Iterations(3)
      ->Args({kGraphSize, 100000})->Iterations(3)
      ->Args({kGraphSize, 500000})->Iterations(3)
  ;
  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
  benchmark::Shutdown();
}
