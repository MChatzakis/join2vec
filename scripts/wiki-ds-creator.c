#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SAMPLES 1000
#define MAX_STR_SIZE 1024
#define STOPWORDS_SIZE 428

char stopwords[STOPWORDS_SIZE][MAX_STR_SIZE];
char *words[SAMPLES];

void load_stopwords(char *filename)
{
  FILE *stopwords_file = fopen(filename, "r");

  if (!stopwords_file)
  {
    perror("Could not open stopwords file.");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < STOPWORDS_SIZE; i++)
  {
    fscanf(stopwords_file, " %1023s", stopwords[i]);
  }
}

int is_stopword(char *word)
{
  for (int i = 0; i < STOPWORDS_SIZE; i++)
  {
    if (strcmp(word, stopwords[i]) == 0)
    {
      return 1;
    }
  }

  return 0;
}

int is_duplicate(char *word, int count)
{
  for (int i = 0; i < count; i++)
  {
    if (strcmp(word, words[i]) == 0)
    {
      return 1;
    }
  }

  return 0;
}

int randint(int n)
{
  if ((n - 1) == RAND_MAX)
  {
    return rand();
  }
  else
  {

    int end = RAND_MAX / n; // truncate skew
    end *= n;

    int r;
    while ((r = rand()) >= end)
      ;

    return r % n;
  }
}

int generate_probability()
{
  int num = randint(101);

  if (num >= 50)
  {
    return 1;
  }

  return 0;
}

int main()
{

  srand(getpid());

  char *stopwords_path = "/scratch/chatzaki/data/stopwords.en.size428";
  char *corpus_path = "/scratch/chatzaki/train-datasets/wikipedia/big-corpus/wiki-corpus";
  char *output_path = "/scratch/chatzaki/test-datasets/english-words/rs-sim-join/v2-wiki-nodup-1000";

  load_stopwords(stopwords_path);

  FILE *corpus_file = fopen(corpus_path, "r");
  FILE *output_file = fopen(output_path, "w");

  if (!corpus_file || !output_file)
  {
    perror("Could not open a file.");
    exit(EXIT_FAILURE);
  }

  char word[MAX_STR_SIZE];
  int sample_count = 0;
  while (fscanf(corpus_file, " %1023s", word) == 1)
  {

    if (generate_probability() && !is_stopword(word) && !is_duplicate(word, sample_count))
    {
      words[sample_count++] = strdup(word);
    }

    if (sample_count >= SAMPLES)
    {
      break;
    }
  }

  for (int i = 0; i < SAMPLES; i++)
  {
    printf("Writing %s to dataset.\n", words[i]);
    fprintf(output_file, "%s\n", words[i]);
  }

  fclose(output_file);
  fclose(corpus_file);

  return 0;
}
