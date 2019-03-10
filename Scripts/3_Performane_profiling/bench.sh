#!/bin/sh

mkdir -p gprof

# num days
for i in 50 100 150 200 500
do
        ./bin/stride -c run_generate_default.xml -o num_days=$i
        gprof ./bin/stride gmon.out > gprof/day_bench_$i.txt
done 

# pop size
for i in 10000 50000 100000 200000 500000
do
        ./bin/stride -c run_generate_default_$i.xml -o num_days=50
        gprof ./bin/stride gmon.out > gprof/pop_bench_$i.txt
done 

# immunity rate
for i in 0.2 0.4 0.6 0.8 0.99
do
        ./bin/stride -c run_generate_default.xml -o immunity_rate=$i
        gprof ./bin/stride gmon.out > gprof/immunity_bench_$i.txt
done 

# seeding rate
for i in 0.02 0.002 0.0002 0.00002
do
        ./bin/stride -c run_generate_default.xml -o seeding_rate=$i
        gprof ./bin/stride gmon.out > gprof/seeding_bench_$i.txt
done 

# contact log mode
for i in All Transmission Susceptible None
do
        ./bin/stride -c run_generate_default.xml -o contact_log_mode=$i
        gprof ./bin/stride gmon.out > gprof/contact_log_bench_$i.txt
done 
