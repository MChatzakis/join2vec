DATASET_PATH=/scratch/chatzaki/test-datasets/english-words/rs-sim-join/v1-wiki-nodup-1000
TIMERS_PATH=./fastss_timers.txt
RESULTS_PATH=./fastss_results.txt

cd ../build
make fastss
cd ../bin

./fastss $DATASET_PATH 2 $TIMERS_PATH $RESULTS_PATH

cd ../scripts