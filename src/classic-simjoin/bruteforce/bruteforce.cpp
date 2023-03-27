#include <iostream>
#include <vector>
#include <string>

#include "../../common/utilfuncs.h"
#include "../../common/timers.h"
#include "../../common/concurrency.h"

#define FAI 0
#define DATA_PARALLEL 1

double THRESHOLD = 3;
int PARALLELISM = 0;
int THREADS = 1;
char *TIMERS_FILENAME;

typedef struct bf_result
{
    std::string w1;
    std::string w2;

    int id_1;
    int id_2;

    double sim;
} bf_result;

typedef struct thread_data
{
    int thread_id;

    volatile int *word_counter; // to be used for FAI

    int from, to;

    std::vector<std::string> l_words;
    std::vector<std::string> r_words;

    std::vector<bf_result> *results;
    pthread_mutex_t *results_lock;

    double *thread_time;
} thread_data;

void bruteforce_sequential(char *filename);
void nested_loop(char *l_filename, char *r_filename);
void *nested_loop_routine(void *args);
void process_join(thread_data *data, int curr_word_index);

int main(int argc, char **argv)
{
    TOTAL_TIME_START
    if (argc < 3)
    {
        return -1;
    }

    char *l_filename = argv[1];
    char *r_filename = argv[2];

    THRESHOLD = atoi(argv[3]);
    PARALLELISM = atoi(argv[4]);
    THREADS = atoi(argv[5]);
    TIMERS_FILENAME = argv[6];

    nested_loop(l_filename, r_filename);
    TOTAL_TIME_END

    log_timers();
    timers_to_file(TIMERS_FILENAME);

    return 0;
}

void *nested_loop_routine(void *args)
{
    thread_pin(((thread_data *)args)->thread_id, THREADS);

    thread_data *data = (thread_data *)args;

    int curr_word_index;

    if (PARALLELISM == FAI)
    {
        while (1)
        {
            curr_word_index = __sync_fetch_and_add(data->word_counter, 1);
            if (curr_word_index >= data->l_words.size())
                break;

            process_join(data, curr_word_index);
        }
    }
    else if (PARALLELISM == DATA_PARALLEL)
    {
        for (curr_word_index = data->from; curr_word_index < data->to; curr_word_index++)
        {
            process_join(data, curr_word_index);
        }
    }

    pthread_exit(NULL);
}

void process_join(thread_data *data, int curr_word_index)
{
    std::string w1 = data->l_words[curr_word_index];

    for (int j = 0; j < data->r_words.size(); j++)
    {
        std::string w2 = data->r_words[j];

        int sim = edit_distance(w1, w2);
        if (sim <= THRESHOLD)
        {
            bf_result res;
            res.id_1 = curr_word_index;
            res.id_2 = j;
            res.sim = sim;

            pthread_mutex_lock(data->results_lock);
            data->results->push_back(res);
            pthread_mutex_unlock(data->results_lock);
        }
    }
}

void nested_loop(char *l_filename, char *r_filename)
{
    PREPROCESSING_TIME_START
    std::vector<std::string> l_words = load_data(l_filename);
    std::vector<std::string> r_words = load_data(r_filename);
    PREPROCESSING_TIME_END

    JOIN_TIME_START
    int thread_num = THREADS;
    volatile int word_counter = 0;

    pthread_t threads[thread_num];
    thread_data data[thread_num];

    std::vector<bf_result> results;
    pthread_mutex_t results_lock = PTHREAD_MUTEX_INITIALIZER;

    int word_chunk_size = l_words.size() / thread_num;
    for (int i = 0; i < thread_num; i++)
    {
        data[i].thread_id = i;
        data[i].word_counter = &word_counter;

        data[i].from = i * word_chunk_size;
        if (i != thread_num - 1)
            data[i].to = (i + 1) * word_chunk_size;
        else
            data[thread_num - 1].to = l_words.size();

        data[i].r_words = r_words;
        data[i].l_words = l_words;

        data[i].results = &results;
        data[i].results_lock = &results_lock;
    }

    for (int i = 0; i < thread_num; i++)
    {
        pthread_create(&threads[i], NULL, nested_loop_routine, (void *)&(data[i]));
    }

    for (int i = 0; i < thread_num; i++)
    {
        pthread_join(threads[i], NULL);
    }

    JOIN_TIME_END

    log_timers();

    return;
}

void bruteforce_sequential(char *filename)
{
    TOTAL_TIME_START

    std::vector<std::string> words = load_data(filename);
    std::vector<bf_result> results;
    int word_counter = 0;

    for (int i = 0; i < words.size(); i++)
    {
        auto w1 = words[i];
        std::cout << "Word " << (i + 1) << " of " << words.size() << std::endl;
        for (int j = i; j < words.size(); j++)
        {
            auto w2 = words[j];
            int ed = edit_distance(w1, w2);
            // std::cout << "Pair(" << w1 << "," << w2 << ")=" << edit_distance(w1, w2) << std::endl;

            if (ed < THRESHOLD)
            {
                bf_result res;
                res.w1 = w1;
                res.w2 = w2;
                res.sim = ed;

                results.push_back(res);
            }
        }
    }

    TOTAL_TIME_END

    log_timers();

    return;
}
