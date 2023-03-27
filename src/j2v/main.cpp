#include <unistd.h>
#include <iostream>
#include <vector>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

#include "./join2vec.h"
#include "../common/timers.h"
#include "../common/utilfuncs.h"

void parse_params(int, char **);
void print_usage();

int main(int argc, char **argv)
{
    TOTAL_TIME_START

    parse_params(argc, argv);

    log_j2v_params();

    join2vec();

    free(j2v_params.model_filename);
    free(j2v_params.dataset_filename);

    TOTAL_TIME_END

    log_timers();
    
    if (j2v_params.timers_filename)
    {
        timers_to_file(c_string(j2v_params.timers_filename));
    }
    

    return 0;
}

void parse_params(int argc, char **argv)
{
    int opt;
    char *mode;
    while (1)
    {
        static struct option long_options[] = {
            {"dataset", required_argument, 0, 'a'},
            {"model", required_argument, 0, 'b'},
            {"threads", required_argument, 0, 'c'},
            {"threshold", required_argument, 0, 'd'},
            {"verbose", no_argument, 0, 'e'},
            {"results-file", required_argument, 0, 'f'},
            {"timers-file", required_argument, 0, 'g'},
            {"preload-vectors", no_argument, 0, 'h'},
            {"simd", no_argument, 0, 'i'},
            {"dims", required_argument, 0, 'j'},
            {"save-ids", no_argument, 0, 'k'},
            {"decimals", required_argument, 0, 'l'},
            {"help", no_argument, 0, 'm'},
            {"ldataset", required_argument, 0, 'n'},
            {"rdataset", required_argument, 0, 'o'},
            {"parallelism", required_argument, 0, 'p'}};

        int option_index = 0;
        opt = getopt_long(argc, argv, "", long_options, &option_index);

        if (opt == -1)
            break;

        switch (opt)
        {
        case 'a':
            j2v_params.dataset_filename = strdup(optarg);
            j2v_params.self_join = true;
            break;
        case 'b':
            j2v_params.model_filename = strdup(optarg);
            break;
        case 'c':
            j2v_params.threads = atoi(optarg);
            break;
        case 'd':
            j2v_params.th = atof(optarg);
            break;
        case 'e':
            j2v_params.verbose = true;
            break;
        case 'f':
            j2v_params.results_filename = strdup(optarg);
            break;
        case 'g':
            j2v_params.timers_filename = strdup(optarg);
            break;
        case 'h':
            j2v_params.preload_vectors = true;
            break;
        case 'i':
            j2v_params.simd = true;
            break;
        case 'j':
            j2v_params.dims = atoi(optarg);
            break;
        case 'l':
            j2v_params.decimals = atoi(optarg);
            break;
        case 'm':
            print_usage();
            exit(EXIT_FAILURE);
        case 'n':
            j2v_params.ldataset_filename = strdup(optarg);
            j2v_params.self_join = false;
            break;
        case 'o':
            j2v_params.rdataset_filename = strdup(optarg);
            j2v_params.self_join = false;
            break;
        case 'p':
            mode = strdup(optarg);
            if(strcmp(mode, "fai") == 0){
                j2v_params.parallelism = fai;
            }
            else if(strcmp(mode, "data-parallel") == 0){
                j2v_params.parallelism = data_parallel;
            }
            else{
                std::cout << "Use --help for instructions." << std::endl;
                exit(EXIT_FAILURE);
            }
            break;
        default:
            std::cout << "Use --help for instructions." << std::endl;
            exit(EXIT_FAILURE);
        }
    
    }
}

void print_usage(){
    
    std::cout << "join2vec usage" << std::endl;
    
    std::cout << "  [--dataset]         <string> " << "Path of the dataset to perform self-join" << std::endl;
    std::cout << "  [--ldataset]        <string> " << "Path of the R-dataset to perform classic join" << std::endl;
    std::cout << "  [--rdataset]        <string> " << "Path of the S-dataset to perform classic join" << std::endl;
    std::cout << "  [--parallelism]     <string> " << "Parallelism type (fai,data-parallel)" << std::endl;
    std::cout << "  [--model]           <string> " << "Path of the model file containing the vectors" << std::endl;
    std::cout << "  [--threads]         <int>    " << "Number of j2v worker threads" << std::endl;
    std::cout << "  [--threshold]       <real>   " << "Similarity threshold in range [0,1]" << std::endl;
    std::cout << "  [--verbose]                  " << "Print debug messages while running" << std::endl;
    std::cout << "  [--preload-vectors]          " << "Preload the word vectors" << std::endl;
    std::cout << "  [--simd]                     " << "Use SIMD instructions for cosine similarity" << std::endl;
    std::cout << "  [--dims]            <int>    " << "Restrict the dimensions of the word vectors" << std::endl;
    std::cout << "  [--save-ids]                 " << "Save word IDs instead of actual strings" << std::endl;
    std::cout << "  [--decimals]        <int>    " << "Restrict the decimal points of the vector dimensions" << std::endl;

    return;
}