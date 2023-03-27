#include "./join2vec.h"

j2v_parameters_t j2v_params;

// to remove
void join2vec_sequential()
{
    /*INPUT_TIME_START
    std::vector<std::string> words = load_data(j2v_params.dataset_filename);
    INPUT_TIME_END

    // std::cout << "Total words to join: " << words.size() << std::endl;

    MODEL_LOAD_TIME_START
    FastText fasttext;
    fasttext.loadModel(c_string(j2v_params.model_filename));
    MODEL_LOAD_TIME_END

    std::vector<simjoin_res> results;

    // preloaded vectors
    PREPROCESSING_TIME_START
    std::vector<Vector> word_vectors;
    if (j2v_params.preload_vectors)
    {
        for (int i = 0; i < words.size(); i++)
        {
            Vector vec(fasttext.getDimension());
            std::string w = words[i];
            fasttext.getWordVector(vec, w);

            if (j2v_params.dims > 0)
                vec.keep_dims(j2v_params.dims);

            // if (j2v_params.decimals >= 0)
            //     vec.set_decimals(j2v_params.decimals);
            if (j2v_params.decimals > 0)
                vec.set_decimals(j2v_params.decimals);
            else if (j2v_params.decimals == 0)
                vec.round_vals();

            word_vectors.push_back(vec);
        }
    }
    PREPROCESSING_TIME_END

    JOIN_TIME_START
    Vector vec1(fasttext.getDimension()), vec2((fasttext.getDimension()));
    for (int i = 0; i < words.size(); i++)
    {
        if ((i + 1) % LOG_POINT == 0 && j2v_params.verbose)
        {
            std::cout << "Word " << (i + 1) << " of " << words.size() << "." << std::endl;
            std::cout.flush();
        }

        std::string w1 = words[i];

        if (j2v_params.preload_vectors)
            vec1 = word_vectors[i];
        else
            fasttext.getWordVector(vec1, w1);

        for (int j = i + 1; j < words.size(); j++)
        {
            std::string w2 = words[j];

            if (j2v_params.preload_vectors)
                vec2 = word_vectors[j];
            else
                fasttext.getWordVector(vec2, w2);

            COSINE_TIME_START
            double sim;
            if (j2v_params.simd)
                sim = SIMD_cosine_similarity(vec1.data(), vec2.data(), fasttext.getDimension());
            else
                sim = cosine_similarity(vec1.data(), vec2.data(), fasttext.getDimension());
            COSINE_TIME_END

            if (sim >= j2v_params.th)
            {
                // std::cout << "(" << w1 << "," << w2 << ")=" << sim << std::endl;
                simjoin_res res;
                // res.w1 = std::string(w1);
                // res.w2 = std::string(w2);
                res.id_1 = i;
                res.id_2 = j;
                res.score = sim;

                results.push_back(res);
            }
        }
    }
    JOIN_TIME_END

    log_join_results(fasttext.getDimension(), words, results);

    id_2_str(words, results);

    if (j2v_params.results_filename)
    {
        results_to_file(j2v_params.results_filename, results);
    }*/

    return;
}

void join2vec()
{
    std::vector<std::string> l_words;
    std::vector<Vector> l_word_vectors;

    std::vector<std::string> r_words;
    std::vector<Vector> r_word_vectors;

    MODEL_LOAD_TIME_START
    FastText fasttext;
    fasttext.loadModel(c_string(j2v_params.model_filename));
    MODEL_LOAD_TIME_END

    INPUT_TIME_START
    if (j2v_params.self_join)
    {
        l_words = load_data(j2v_params.dataset_filename);
        r_words = load_data(j2v_params.dataset_filename);
    }
    else
    {
        l_words = load_data(j2v_params.ldataset_filename);
        r_words = load_data(j2v_params.rdataset_filename);
    }
    INPUT_TIME_END

    PREPROCESSING_TIME_START
    if (j2v_params.preload_vectors)
    {
        preload_vectors(l_words, l_word_vectors, fasttext);
        preload_vectors(r_words, r_word_vectors, fasttext);
    }
    PREPROCESSING_TIME_END

    JOIN_TIME_START
    int thread_num = j2v_params.threads;
    volatile int word_counter = 0;

    pthread_t threads[thread_num];
    j2v_thread_data data[thread_num];

    std::vector<simjoin_res> results;
    pthread_mutex_t results_lock = PTHREAD_MUTEX_INITIALIZER;

    int word_chunk_size = l_words.size() / thread_num;
    for (int i = 0; i < thread_num; i++)
    {
        data[i].thread_id = i;

        data[i].word_counter = &word_counter; // for FAI

        data[i].from = i * word_chunk_size;
        if (i != thread_num - 1)
            data[i].to = (i + 1) * word_chunk_size;
        else
            data[thread_num - 1].to = l_words.size();

        data[i].r_words = r_words;
        data[i].r_word_vectors = r_word_vectors;

        data[i].l_words = l_words;
        data[i].l_word_vectors = l_word_vectors;

        data[i].fasttext = &fasttext;

        data[i].results = &results;
        data[i].results_lock = &results_lock;
    }

    for (int i = 0; i < thread_num; i++)
    {
        pthread_create(&threads[i], NULL, j2v_worker_routine, (void *)&(data[i]));
    }

    for (int i = 0; i < thread_num; i++)
    {
        pthread_join(threads[i], NULL);
    }

    JOIN_TIME_END

    log_join_results(fasttext.getDimension(), l_words, r_words, results);
    // log_thread_times(data);

    id_2_str(l_words, r_words, results);
    if (j2v_params.results_filename)
    {
        results_to_file(j2v_params.results_filename, results);
    }

    return;
}

void *j2v_worker_routine(void *args)
{

    thread_pin(((j2v_thread_data *)args)->thread_id, j2v_params.threads);

    tpoint total_thread_time_start = std::chrono::steady_clock::now();

    j2v_thread_data *data = (j2v_thread_data *)args;
    Vector vec1((data->fasttext)->getDimension()), vec2(((data->fasttext)->getDimension()));

    int curr_word_index;

    if (j2v_params.parallelism == fai)
    {
        while (1)
        {
            curr_word_index = __sync_fetch_and_add(data->word_counter, 1);
            if (curr_word_index >= data->l_words.size())
                break;

            process_word_join(data, curr_word_index, vec1, vec2);
        }
    }
    else if (j2v_params.parallelism == data_parallel)
    {
        for (curr_word_index = data->from; curr_word_index < data->to; curr_word_index++)
        {
            process_word_join(data, curr_word_index, vec1, vec2);
        }
    }

    data->thread_time = std::chrono::duration_cast<std::chrono::T_UNIT>(std::chrono::steady_clock::now() - total_thread_time_start).count();

    pthread_exit(NULL);
}

void process_word_join(j2v_thread_data *data, int curr_word_index, Vector vec1, Vector vec2)
{
    std::string curr_word = data->l_words[curr_word_index];

    if (j2v_params.preload_vectors)
        vec1 = (data->l_word_vectors)[curr_word_index];
    else
        (data->fasttext)->getWordVector(vec1, curr_word);

    int start_j, end_j;
    if (j2v_params.self_join)
    {
        start_j = curr_word_index + 1;
        end_j = data->l_words.size();
    }
    else
    {
        start_j = 0;
        end_j = data->r_words.size();
    }

    for (int j = start_j; j < end_j; j++)
    {
        std::string w2 = data->r_words[j];

        if (j2v_params.preload_vectors)
            vec2 = (data->r_word_vectors)[j];
        else
            (data->fasttext)->getWordVector(vec2, w2);

        double sim;
        if (j2v_params.simd)
            sim = SIMD_cosine_similarity(vec1.data(), vec2.data(), (data->fasttext)->getDimension());
        else if (j2v_params.decimals == 0)
            sim = cosine_similarity_int(vec1.r_data(), vec2.r_data(), (data->fasttext)->getDimension());
        else
            sim = cosine_similarity(vec1.data(), vec2.data(), (data->fasttext)->getDimension());

        if (sim >= j2v_params.th)
        {
            simjoin_res res;
            res.id_1 = curr_word_index;
            res.id_2 = j;
            res.score = sim;

            pthread_mutex_lock(data->results_lock);
            data->results->push_back(res);
            pthread_mutex_unlock(data->results_lock);
        }
    }
}

void log_j2v_params()
{
    std::cout << "=== j2v Params Log ===" << std::endl;

    if (j2v_params.ldataset_filename)
        std::cout << "L-Dataset: " << j2v_params.ldataset_filename << std::endl;
    if (j2v_params.rdataset_filename)
        std::cout << "R-Dataset: " << j2v_params.rdataset_filename << std::endl;
    if (j2v_params.dataset_filename)
        std::cout << "Dataset: " << j2v_params.dataset_filename << std::endl;

    std::cout << "Model: " << j2v_params.model_filename << std::endl;
    std::cout << "Threads: " << j2v_params.threads << std::endl;
    std::cout << "Threshold: " << j2v_params.th << std::endl;
    std::cout << "Verbose: " << j2v_params.verbose << std::endl;
    std::cout << "Parallelism: " << j2v_params.parallelism << std::endl;

    if (j2v_params.results_filename)
    {
        std::cout << "Results: " << j2v_params.results_filename << std::endl;
    }

    if (j2v_params.timers_filename)
    {
        std::cout << "Timers: " << j2v_params.timers_filename << std::endl;
    }

    std::cout << "Preload: " << j2v_params.preload_vectors << std::endl;
    std::cout << "SIMD: " << j2v_params.simd << std::endl;

    std::cout << "======================" << std::endl;

    return;
}

void log_join_results(int dimensions, std::vector<std::string> l_words, std::vector<std::string> r_words, std::vector<simjoin_res> results)
{
    std::cout << "=== Join Log ===" << std::endl;

    std::cout << "L-Words: " << l_words.size() << std::endl;
    if (!j2v_params.self_join)
    {
        std::cout << "R-Words: " << r_words.size() << std::endl;
    }

    std::cout << "Dims: " << dimensions; //<< std::endl;
    if (j2v_params.dims >= 0)
    {
        std::cout << "(Used " << j2v_params.dims << ")";
    }
    std::cout << std::endl;

    std::cout << "Pairs: " << results.size() << std::endl;

    std::cout << "================" << std::endl;
}

void preload_vectors(std::vector<std::string> words, std::vector<Vector> &word_vectors, FastText &fasttext)
{
    for (int i = 0; i < words.size(); i++)
    {
        Vector vec(fasttext.getDimension());
        std::string w = words[i];
        fasttext.getWordVector(vec, w);

        if (j2v_params.dims > 0)
            vec.keep_dims(j2v_params.dims);

        if (j2v_params.decimals > 0)
            vec.set_decimals(j2v_params.decimals);
        else if (j2v_params.decimals == 0)
            vec.round_vals();

        word_vectors.push_back(vec);
    }
}

void log_thread_times(j2v_thread_data *data)
{
    for (int i = 0; i < j2v_params.threads; i++)
    {
        // print("Thread1")
        std::cout << "Thread " << i << " : " << data[i].thread_time << " ms" << std::endl;
    }
}