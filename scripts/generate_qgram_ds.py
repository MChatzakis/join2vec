INPUT = "/scratch/chatzaki/test-datasets/english-words/rs-sim-join/v1-wiki-nodup-1000"
OUTPUT = "/scratch/chatzaki/test-datasets/english-words/rs-sim-join/v1-wiki-nodup-1000-gram2"

NGRAMS = 2

def generate_ngram_str(str, ngram, char="_"):
    return char.join(str[i:i+ngram] for i in range(0, len(str), ngram))


with open(INPUT, 'r') as fr, open(OUTPUT, 'w') as fw:
    for line in fr:
        word = line.strip()
        ngword = generate_ngram_str(word, NGRAMS)
        fw.write(ngword + "\n")
