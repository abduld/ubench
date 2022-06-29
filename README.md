# ubench

## System Setup (for Linux Only)

```
bash system_setup/setup.sh
```

## Compile



```
mkdir build
cd build
cmake -S .. -G Ninja
cmake --build build
```

## Run GEMM

```
build/bin/ubench --benchmark_out=../results/output.json --benchmark_out_format=json
```
## Run Vector Add

```
build/bin/ubench --benchmark_out=../results/output.json --benchmark_out_format=json
```
