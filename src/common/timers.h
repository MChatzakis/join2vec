#ifndef _TIMERS_H_
#define _TIMERS_H_

#include <chrono>
#include <iostream>
#include <fstream>

typedef std::chrono::steady_clock::time_point tpoint;

// Timepoints
extern tpoint total_time_start;
extern tpoint preprocessing_time_start;
extern tpoint input_time_start;
extern tpoint output_time_start;
extern tpoint model_load_time_start;
extern tpoint join_time_start;
extern tpoint cosine_time_start;

// Time counters
extern double TOTAL_TIME;
extern double PREPROCESSING_TIME;
extern double INPUT_TIME;
extern double OUTPUT_TIME;
extern double MODEL_LOAD_TIME;
extern double JOIN_TIME;
extern double COSINE_TIME;

#define T_UNIT milliseconds

// Convinience Defines
#define TOTAL_TIME_START (total_time_start = std::chrono::steady_clock::now());
#define TOTAL_TIME_END (TOTAL_TIME += std::chrono::duration_cast<std::chrono::T_UNIT>(std::chrono::steady_clock::now() - total_time_start).count());

#define PREPROCESSING_TIME_START (preprocessing_time_start = std::chrono::steady_clock::now());
#define PREPROCESSING_TIME_END (PREPROCESSING_TIME += std::chrono::duration_cast<std::chrono::T_UNIT>(std::chrono::steady_clock::now() - preprocessing_time_start).count());

#define INPUT_TIME_START (input_time_start = std::chrono::steady_clock::now());
#define INPUT_TIME_END (INPUT_TIME += std::chrono::duration_cast<std::chrono::T_UNIT>(std::chrono::steady_clock::now() - input_time_start).count());

#define OUTPUT_TIME_START (output_time_start = std::chrono::steady_clock::now());
#define OUTPUT_TIME_END (OUTPUT_TIME += std::chrono::duration_cast<std::chrono::T_UNIT>(std::chrono::steady_clock::now() - output_time_start).count());

#define MODEL_LOAD_TIME_START (model_load_time_start = std::chrono::steady_clock::now());
#define MODEL_LOAD_TIME_END (MODEL_LOAD_TIME += std::chrono::duration_cast<std::chrono::T_UNIT>(std::chrono::steady_clock::now() - model_load_time_start).count());

#define JOIN_TIME_START (join_time_start = std::chrono::steady_clock::now());
#define JOIN_TIME_END (JOIN_TIME += std::chrono::duration_cast<std::chrono::T_UNIT>(std::chrono::steady_clock::now() - join_time_start).count());

#define COSINE_TIME_START (cosine_time_start = std::chrono::steady_clock::now());
#define COSINE_TIME_END (COSINE_TIME += std::chrono::duration_cast<std::chrono::T_UNIT>(std::chrono::steady_clock::now() - cosine_time_start).count());

void log_timers();
void init_timers();

void timers_to_file(std::string filename);

#endif
