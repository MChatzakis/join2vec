#ifndef _JOIN2VEC_H_
#define _JOIN2VEC_H_

#include <unistd.h>
#include <iostream>
#include <vector>
#include <string>
#include <utility>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./fasttext_lib.h"

#include "../common/timers.h"
#include "../common/utilfuncs.h"
#include "../common/concurrency.h"

#define LOG_POINT 1000

enum parallelism_type{
    fai = 0,
    data_parallel
};

typedef struct j2v_parameters_t
{
    int threads = 1;

    char *dataset_filename = NULL;

    char *ldataset_filename = NULL;
    char *rdataset_filename = NULL;

    char *model_filename = NULL;
    char *results_filename = NULL;
    char *timers_filename = NULL;

    double th = 0.8;

    bool verbose = false;
    bool preload_vectors = false;
    bool simd = false;
    bool self_join = false;

    int dims = -1;
    int decimals = -1;

    parallelism_type parallelism;

} j2v_parameters_t;

typedef struct j2v_result
{
    std::string w1;
    std::string w2;
    double score;
} j2v_result;

typedef struct j2v_thread_data
{
    int thread_id;

    volatile int *word_counter;

    int from, to;

    FastText *fasttext;

    std::vector<std::string> l_words;
    std::vector<Vector> l_word_vectors;
    std::vector<std::string> r_words;
    std::vector<Vector> r_word_vectors;

    std::vector<simjoin_res> *results;
    pthread_mutex_t *results_lock;

    double thread_time;

} j2v_thread_data;

extern j2v_parameters_t j2v_params;

void join2vec_sequential();
void join2vec();

void *j2v_worker_routine(void *args);

void log_j2v_params();
void log_join_results(int dimensions, std::vector<std::string> l_words, std::vector<std::string> r_words, std::vector<simjoin_res> results);
void log_thread_times(j2v_thread_data *data);

void preload_vectors(std::vector<std::string> words, std::vector<Vector> &word_vectors, FastText &fasttext);
void process_word_join(j2v_thread_data *data, int curr_word_index, Vector vec1, Vector vec2);


#endif