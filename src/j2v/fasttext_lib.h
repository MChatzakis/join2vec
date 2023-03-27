#ifndef _FASTTEXT_LIB_H_
#define _FASTTEXT_LIB_H_

#include <iomanip>
#include <iostream>
#include <queue>
#include <stdexcept>

#include "./fastText/src/args.h"
#include "./fastText/src/autotune.h"
#include "./fastText/src/fasttext.h"

using namespace fasttext;

//Facebook Implementations 
void printUsage();
void printQuantizeUsage();
void printTestUsage();
void printPredictUsage();
void printTestLabelUsage();
void printPrintWordVectorsUsage();
void printPrintSentenceVectorsUsage();
void printPrintNgramsUsage();
void quantize(const std::vector<std::string> &args);
void printNNUsage();
void printAnalogiesUsage();
void printDumpUsage();
void test(const std::vector<std::string> &args);
void printPredictions(const std::vector<std::pair<real, std::string>> &predictions, bool printProb, bool multiline);
void predict(const std::vector<std::string> &args);
void printWordVectors(const std::vector<std::string> args);
void printSentenceVectors(const std::vector<std::string> args);
void printNgrams(const std::vector<std::string> args);
void nn(const std::vector<std::string> args);
void analogies(const std::vector<std::string> args);
void train(const std::vector<std::string> args);
void dump(const std::vector<std::string> &args);

//Custom Implementations

#endif