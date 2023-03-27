MODEL_PATH=/scratch/chatzaki/result-embeddings/fasttext-wikipedia-big-corpus/wiki-vector.bin

DATASET_PATH=/scratch/chatzaki/test-datasets/english-words/rs-sim-join/v1-wiki-nodup-1000

L_DATASET_PATH=/scratch/chatzaki/test-datasets/english-words/rs-sim-join/v1-wiki-nodup-10000
R_DATASET_PATH=/scratch/chatzaki/test-datasets/english-words/rs-sim-join/v2-wiki-nodup-100000

RESULTS_PATH=./j2v_join-res.txt
TIMERS_PATH=./j2v_timers.txt

cd ../build
make j2v
cd ../bin

echo "DATA PARALLEL"
./j2v --model $MODEL_PATH --ldataset $L_DATASET_PATH --rdataset $R_DATASET_PATH --threads 48 --threshold 0.9 --parallelism data-parallel --preload-vectors --simd --results-file $RESULTS_PATH --timers-file $TIMERS_PATH  

echo "FAI"
./j2v --model $MODEL_PATH --ldataset $L_DATASET_PATH --rdataset $R_DATASET_PATH --threads 48 --threshold 0.9 --parallelism fai --preload-vectors --simd --results-file $RESULTS_PATH --timers-file $TIMERS_PATH  

#./j2v --model $MODEL_PATH --dataset $DATASET_PATH --threads 48 --threshold 0.9 --preload-vectors --simd --parallelism data-parallel --results-file $RESULTS_PATH --timers-file $TIMERS_PATH


cd ../scripts