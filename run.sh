#!/usr/bin/env bash

source venv/bin/activate
cd src/cluster
make
cd ../
python predict.py -i ../data/nn_representations.csv
python new_representation.py -i ../data/nn_representations.csv
shuf -n 500 ../data/nn_representations.csv > ../data/nn_representations_random.csv
shuf -n 500 ../data/predicted.csv > ../data/predicted_random.csv
cd cluster
./cluster -i ../../data/nn_representations_random.csv -c months_cluster.conf -p -o ../../data/results_months_nn
./cluster -i ../../data/nn_representations_random.csv -c seasons_cluster.conf -p -o ../../data/results_seasons_nn
./cluster -i ../../data/predicted_random.csv -c months_cluster.conf -p -o ../../data/results_months_pr
./cluster -i ../../data/predicted_random.csv -c seasons_cluster.conf -p -o ../../data/results_seasons_pr
make clean
deactivate
