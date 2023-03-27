# join2vec: Semantic-Rich and Efficient String Similarity Joins with Vector Embeddings

Manos Chatzakis (emmanouil.chatzakis@epfl.ch), Viktor Sanca, Anastasia Ailamaki

join2vec is a framework that utilizes deep learning word embeddings to perform semantic rich similarity joins in relational databases. It was developed during summer@EPFL program and during a semester project in collaboration with DIAS lab, advised by Viktor Sanca and Anastasia Ailamaki.

Part of join2vec algorithms and implementations are included in the paper "Context-Enhanced Relational Operators via Neural Embeddings", Viktor Sanca, Manos Chatzakis and Anastasia Ailamaki, to be submitted in VLDB.

This repository contains the initial code implementing j2v, which was written during summer@EPFL and semester project at EPFL. It also contains the report and presentations dedicated completely to join2vec.

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
Guidance and resources were provided by DIAS lab of EPFL.