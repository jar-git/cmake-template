# Benchmarks
Benchmarks for the dummy library.

## lib_socket

### Execute

```
sudo cpupower frequency-set --governor performance
./bench/lib_socket_bench/socket_benchmark --benchmark_format=json --benchmark_out=socket.json
sudo cpupower frequency-set --governor powersave
```

### Compare

