#!/usr/bin/env bash

echo 'Activating python virtual environment..'
source venv/bin/activate
cd src/cluster
echo 'Compiling cluster'
make
cd ../
echo 'Run predict.py ..'
python predict.py -i ../data/nn_representations.csv
echo 'Done'
echo 'Run new_representation.py..'
python new_representation.py -i ../data/nn_representations.csv
echo 'Done'
echo 'Creating samples..'
shuf -n 500 ../data/new_representation.csv > ../data/new_representation_random.csv
shuf -n 500 ../data/predicted.csv > ../data/predicted_random.csv
echo 'Done'
cd cluster
echo 'Run clustering for months on nn_representations.csv'
./cluster -i ../../data/new_representation_random.csv -c months_cluster.conf -p -o ../../data/results_months_nn
echo 'Run clustering for season on nn_representations.csv'
./cluster -i ../../data/new_representation_random.csv -c seasons_cluster.conf -p -o ../../data/results_seasons_nn
echo 'Run clustering for months on predicted.csv'
./cluster -i ../../data/predicted_random.csv -c months_cluster.conf -p -o ../../data/results_months_pr
echo 'Run clustering for months on predicted.csv'
./cluster -i ../../data/predicted_random.csv -c seasons_cluster.conf -p -o ../../data/results_seasons_pr
echo 'Deactivating virtual environment..'
deactivate
