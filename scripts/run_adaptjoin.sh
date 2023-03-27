DATASET_PATH=/scratch/chatzaki/test-datasets/english-words/rs-sim-join/v1-wiki-nodup-1000
GRAMMED_DATASET_PATH=/scratch/chatzaki/test-datasets/english-words/rs-sim-join/v1-wiki-nodup-1000-gram2

cd ../bin

time ./AdaptJoin cosine 0.9 $GRAMMED_DATASET_PATH
#time ./AdaptJoin editdist gram 2 1 $DATASET_PATH

cd ../scripts