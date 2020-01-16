#!/usr/bin/env bash

for NUM in 100 250  500  1000  1500  2000  2500  3000  5000  10000
do
    shuf -n $NUM out.csv > out_random_$NUM.csv
done
