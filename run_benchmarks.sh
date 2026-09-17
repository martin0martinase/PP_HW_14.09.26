#!/bin/bash
# run_benchmarks.sh

for threads in 1 2 4 8 16 32 64 128 256 512 1024
do
    for run in $(seq 1 5)
    do
        ./main $threads
    done
done