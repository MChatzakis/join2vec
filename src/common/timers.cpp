#include "./timers.h"

tpoint total_time_start;
tpoint preprocessing_time_start;
tpoint input_time_start;
tpoint output_time_start;
tpoint model_load_time_start;
tpoint join_time_start;
tpoint cosine_time_start;

double TOTAL_TIME = 0;
double PREPROCESSING_TIME = 0;
double INPUT_TIME = 0;
double OUTPUT_TIME = 0;
double MODEL_LOAD_TIME = 0;
double JOIN_TIME = 0;
double COSINE_TIME = 0;

void log_timers()
{
    std::string unit("ms");

    std::cout << "==== Timing Log ====" << std::endl;

    std::cout << "Total: " << TOTAL_TIME << unit << std::endl;
    std::cout << "Input: " << INPUT_TIME << unit << std::endl;
    std::cout << "Preprocessing: " << PREPROCESSING_TIME << unit << std::endl;
    // std::cout << "Output: " << OUTPUT_TIME << "s" << std::endl;
    std::cout << "Model Load: " << MODEL_LOAD_TIME << unit << std::endl;
    std::cout << "Join: " << JOIN_TIME << unit << std::endl;
    std::cout << "Join+Prepro:" << JOIN_TIME + PREPROCESSING_TIME << unit << std::endl;
    //std::cout << "Cosine: " << COSINE_TIME << unit << std::endl;

    std::cout << "====================" << std::endl;


    return;
}

void init_timers()
{
    TOTAL_TIME = 0;
    PREPROCESSING_TIME = 0;
    INPUT_TIME = 0;
    OUTPUT_TIME = 0;
    MODEL_LOAD_TIME = 0;
    JOIN_TIME = 0;
    COSINE_TIME = 0;
}

void timers_to_file(std::string filename)
{
    std::ofstream outfile;

    outfile.open(filename, std::ios_base::app);
    
    outfile << TOTAL_TIME << " ";
    outfile << PREPROCESSING_TIME << " ";
    outfile << INPUT_TIME << " ";
    outfile << OUTPUT_TIME << " ";
    outfile << MODEL_LOAD_TIME << " ";
    outfile << JOIN_TIME << " ";
    outfile << COSINE_TIME; 

    outfile << "\n";
}