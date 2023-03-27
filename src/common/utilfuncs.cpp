#include <sstream>
#include <fstream>

#include <math.h>
#include <stdarg.h>

#include "./utilfuncs.h"

/* -------------------- Utility -------------------- */
int edit_distance(std::string s1, std::string s2)
{
  if (s1.size() == 0 || s2.size() == 0)
  {
    if (s1.size() > s2.size())
      return s1.size() - s2.size();
    return s2.size() - s1.size();
  }
  if (s1[0] == s2[0])
    return edit_distance(s1.substr(1), s2.substr(1));

  int x = edit_distance(s1.substr(1), s2);

  int y = edit_distance(s1, s2.substr(1));

  int z = edit_distance(s1.substr(1), s2.substr(1));

  return 1 + std::min(x, std::min(y, z));
}

std::vector<std::string> load_data(char *filename)
{
  std::vector<std::string> line_data;

  std::string line;
  std::ifstream infile(filename);

  while (std::getline(infile, line))
  {
    // std::cout << line << std::endl;
    line_data.push_back(line);
  }

  return line_data;
}

float cosine_similarity(float *A, float *B, unsigned int size)
{
  float mul = 0.0;
  float d_a = 0.0;
  float d_b = 0.0;

  for (unsigned int i = 0; i < size; ++i)
  {
    mul += *A * *B;
    d_a += *A * *A;
    d_b += *B * *B;

    A++;
    B++;
  }

  if (d_a == 0.0f || d_b == 0.0f)
  {
    throw std::logic_error(
        "cosine similarity is not defined whenever one or both "
        "input vectors are zero-vectors.");
  }

  return mul / (sqrt(d_a) * sqrt(d_b));
}

float cosine_similarity_int(int *A, int *B, unsigned int size){
  int mul = 0.0;
  int d_a = 0.0;
  int d_b = 0.0;

  for (unsigned int i = 0; i < size; ++i)
  {
    mul += *A * *B;
    d_a += *A * *A;
    d_b += *B * *B;

    A++;
    B++;
  }

  if (d_a == 0.0f || d_b == 0.0f)
  {
    //throw std::logic_error(
    //    "cosine similarity is not defined whenever one or both "
    //    "input vectors are zero-vectors.");
  }

  return mul / (sqrt(d_a) * sqrt(d_b));
}

float SIMD_cosine_similarity(float *A, float *B, unsigned int size)
{
  float distance;

  SimdCosineDistance32f(A, B, size, &distance);

  return 1 - distance;
}

void id_2_str(std::string filename, std::vector<simjoin_res> &results)
{
  std::vector<std::string> words = load_data((char *)filename.c_str());

  for (int i = 0; i < results.size(); i++)
  {

    int id_1 = results[i].id_1;
    int id_2 = results[i].id_2;

    results[i].w1 = std::string(words[id_1]);
    results[i].w2 = std::string(words[id_2]);
  }
}

void id_2_str(std::vector<std::string> words, std::vector<simjoin_res> &results)
{
  for (int i = 0; i < results.size(); i++)
  {
    int id_1 = results[i].id_1;
    int id_2 = results[i].id_2;

    results[i].w1 = std::string(words[id_1]);
    results[i].w2 = std::string(words[id_2]);
  }
}

void id_2_str(std::vector<std::string> l_words, std::vector<std::string> r_words, std::vector<simjoin_res> &results)
{
  for (int i = 0; i < results.size(); i++)
  {
    int id_1 = results[i].id_1;
    int id_2 = results[i].id_2;

    results[i].w1 = std::string(l_words[id_1]);
    results[i].w2 = std::string(r_words[id_2]);
  }
}

void results_to_file(char *filename, std::vector<simjoin_res> results)
{
  std::ofstream outfile;

  outfile.open(filename);

  for (auto r : results)
  {
    outfile << r.w1 << " " << r.w2 << " " << r.score << "\n";
  }

  outfile.close();
}

float format_float(float num, int decimals)
{
  return floor(pow(10, decimals) * num) / pow(10, decimals);
}

/* -------------------- Terminal Colors -------------------- */

void fprintf_red(FILE *stream, const char *str, ...)
{
  va_list args;
  va_start(args, str);
  vfprintf(stream, ANSI_COLOR_RED, args);
  vfprintf(stream, str, args);
  vfprintf(stream, ANSI_COLOR_RESET, args);
  va_end(args);
}

void fprintf_avm_red(const char *str, ...)
{
  va_list args;
  va_start(args, str);
  vfprintf(stderr, ANSI_COLOR_RED, args);
  vfprintf(stderr, str, args);
  vfprintf(stderr, ANSI_COLOR_RESET, args);
  va_end(args);

  exit(EXIT_FAILURE);
}

void fprintf_avm_yellow(const char *str, ...)
{
  va_list args;
  va_start(args, str);
  vfprintf(stderr, ANSI_COLOR_YELLOW, args);
  vfprintf(stderr, str, args);
  vfprintf(stderr, ANSI_COLOR_RESET, args);
  va_end(args);
}

void fprintf_green(FILE *stream, const char *str, ...)
{
  va_list args;
  va_start(args, str);
  vfprintf(stream, ANSI_COLOR_GREEN, args);
  vfprintf(stream, str, args);
  vfprintf(stream, ANSI_COLOR_RESET, args);
  va_end(args);
}

void fprintf_yellow(FILE *stream, const char *str, ...)
{
  va_list args;
  va_start(args, str);
  vfprintf(stream, ANSI_COLOR_YELLOW, args);
  vfprintf(stream, str, args);
  vfprintf(stream, ANSI_COLOR_RESET, args);
  va_end(args);
}

void fprintf_blue(FILE *stream, const char *str, ...)
{
  va_list args;
  va_start(args, str);
  vfprintf(stream, ANSI_COLOR_BLUE, args);
  vfprintf(stream, str, args);
  vfprintf(stream, ANSI_COLOR_RESET, args);
  va_end(args);
}

void fprintf_magenta(FILE *stream, const char *str, ...)
{
  va_list args;
  va_start(args, str);
  vfprintf(stream, ANSI_COLOR_MAGENTA, args);
  vfprintf(stream, str, args);
  vfprintf(stream, ANSI_COLOR_RESET, args);
  va_end(args);
}

void fprintf_cyan(FILE *stream, const char *str, ...)
{
  va_list args;
  va_start(args, str);
  vfprintf(stream, ANSI_COLOR_CYAN, args);
  vfprintf(stream, str, args);
  vfprintf(stream, ANSI_COLOR_RESET, args);
  va_end(args);
}
