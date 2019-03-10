#!/bin/sh

mkdir -p gprof

# bench days
for i in 50 100 150 200
do
        ./bin/stride -c run_default.xml -o num_days=$i
        gprof ./bin/stride gmon.out > gprof/day_bench_$i.txt
done 
