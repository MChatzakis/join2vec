#ifndef _UTILFUNCS_H_
#define _UTILFUNCS_H_

#include <string>
#include <vector>
#include <iostream>

#include "../Simd/src/Simd/SimdLib.h"

#define c_string(str) std::string(str)
#define v_ptr(v) &v[0]

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

/* -------------------- Utility -------------------- */
typedef struct simjoin_res
{
    int id_1;
    std::string w1;

    int id_2;
    std::string w2;

    double score;
} simjoin_res;

int edit_distance(std::string s1, std::string s2);

std::vector<std::string> load_data(char *filename);

float cosine_similarity(float *A, float *B, unsigned int size);
float cosine_similarity_int(int *A, int *B, unsigned int size);

float SIMD_cosine_similarity(float *A, float *B, unsigned int size);

float format_float(float num, int decimals);

void id_2_str(std::string filename, std::vector<simjoin_res> &results);
void id_2_str(std::vector<std::string> words, std::vector<simjoin_res> &results);

void id_2_str(std::vector<std::string> l_words, std::vector<std::string> r_words, std::vector<simjoin_res> &results);

void results_to_file(char * filename, std::vector<simjoin_res> results);

/* -------------------- Terminal Colors -------------------- */
void fprintf_red(FILE *stream, const char *str, ...);
void fprintf_avm_red(const char *str, ...);
void fprintf_green(FILE *stream, const char *str, ...);
void fprintf_yellow(FILE *stream, const char *str, ...);
void fprintf_avm_yellow(const char *str, ...);
void fprintf_blue(FILE *stream, const char *str, ...);
void fprintf_magenta(FILE *stream, const char *str, ...);
void fprintf_cyan(FILE *stream, const char *str, ...);


#endif