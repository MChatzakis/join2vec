# join2vec: Semantic-Rich and Efficient String Similarity Joins with Vector Embeddings

Manos Chatzakis (emmanouil.chatzakis@epfl.ch), Viktor Sanca, Anastasia Ailamaki

join2vec is a framework that utilizes deep learning word embeddings to perform semantic rich similarity joins in relational databases. It was developed during [summer@EPFL](https://summer.epfl.ch/) program and during a semester project in collaboration with [DIAS](https://www.epfl.ch/labs/dias/) lab, advised by Viktor Sanca and Anastasia Ailamaki.



## Compilation
```bash
cd build
make
cd ../bin/
```

## Run
```bash
cd scripts
./run_j2v.sh
```

## Libraries
Join2vec makes use of the following libraries:
* [FastText](https://github.com/facebookresearch/fastText) 
* [SIMD](https://github.com/ermig1979/Simd)
* Implementations of famous similarity join algorithms, including implementations from [simjoin-paper](https://github.com/VincentShenbw/similarityjoin): AdaptJoin, FastSS, PPJoin+

## About
Guidance and resources were provided by [DIAS](https://www.epfl.ch/labs/dias/) lab of EPFL.
