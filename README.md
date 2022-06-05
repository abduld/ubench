# ubench

## Compile

```
mkdir build
cd build
cmake .. -G Ninja
ninja
```

## Run

```
ninja && ./bin/ubench --benchmark_out=../results/output.json --benchmark_out_format=json --benchmark_repetitions=5 --benchmark_enable_random_interleaving=true
```
