L_DATASET_PATH=/scratch/chatzaki/test-datasets/english-words/rs-sim-join/l-wiki-nodup-100000
R_DATASET_PATH=/scratch/chatzaki/test-datasets/english-words/rs-sim-join/r-wiki-nodup-100000
DATASET_PATH=/scratch/chatzaki/test-datasets/english-words/sim-join/wiki-nodup-100
FAI=0
DATA_PARALLEL=1

cd ../build
make bf
cd ../bin

./bf $DATASET_PATH $DATASET_PATH 2 $FAI 32

cd ../scripts