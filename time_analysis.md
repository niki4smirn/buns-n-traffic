# Work time analysis for TrafficManager::Transport

Measurements were made using
[Google Benchmark](https://github.com/google/benchmark) for different
graph types with random values for TrafficManager fields and Transport
arguments.

Graphs were created using graph(size) constructor

### Graph types:

* #### Graph

    > Measurements were made on complete graphs

    | Graph size⟍ Iterations | 50'000  | 100'000 | 500'000 |
    |:----------------------:|:-------:|:-------:|:-------:|
    |           10           | 19.1 ms | 36.2 ms | 179 ms  |
    |           20           | 31.4 ms | 64.7 ms | 287 ms  |
    |          100           | 571 ms  | 1186 ms | 5914 ms |

***

* #### Clique

    | Graph size⟍ Iterations | 50'000  | 100'000 | 500'000 |
    |:----------------------:|:-------:|:-------:|:-------:|
    |           10           | 14.9 ms | 27.9 ms | 135 ms  |
    |           20           | 24.4 ms | 53.4 ms | 248 ms  |
    |          100           | 402 ms  | 854 ms  | 4486 ms

***

* #### Chain

    > Measurements were made on chain graphs 

    | Graph size⟍ Iterations | 50'000  | 100'000 | 500'000 |
    |:----------------------:|:-------:|:-------:|:-------:|
    |           10           | 13.3 ms | 23.3 ms | 121 ms  |
    |           20           | 13.5 ms | 29.8 ms | 133 ms  |
    |          100           | 17.9 ms | 34.6 ms | 158 ms  |
