#import matplotlib
#matplotlib.use('Agg')

from curses.ascii import isalnum

#import matplotlib.pyplot as plt
import fasttext
import decimal
from scipy import spatial


MACHINE_CORES = 32
VECTORS_PATH = "/scratch/chatzaki/result-embeddings/fasttext-wikipedia-big-corpus/wiki-vector.bin"
PRETRAINED_VECTORS_PATH = "/scratch/chatzaki/pretrained-embeddings/fasttext/wiki.en.bin"
SYNONYMS_PATH = "/scratch/chatzaki/test-datasets/english-words/enwiki-synonyms.mw20000.ms1.edth5"
MISPELLINGS_PATH = ""
PRETRAINED_VECTORS_PATH_CRAWL = "/scratch/chatzaki/pretrained-embeddings/fasttext/cc.en.300.bin"

def cos_sim(v1, v2):
    return 1 - spatial.distance.cosine(v1, v2)


def train_fastText(path, threads):
    return fasttext.train_unsupervised(path, thread=threads)


def save_fastText(model, path):
    model.save_model(path)


def load_fastText(path):
    return fasttext.load_model(path)


def produce_fastText_wiki_vectors():
    corpus_path = "/scratch/chatzaki/train-datasets/wikipedia/big-corpus/wiki-corpus"
    save_path = "/scratch/chatzaki/result-embeddings/fasttext-wikipedia-big-corpus/wiki-vector.bin"

    model = train_fastText(corpus_path, MACHINE_CORES)

    save_fastText(model, save_path)


def evaluate_synonyms_fastText(model, samples, threshold, dataset_path):

    hits = 0
    samples_processed = 0

    with open(dataset_path, 'r') as fp:
        for line in fp:
            data = line.strip().split(' ')

            target_word = data[0]
            synonym = data[1].replace("_", " ")  # ! check again

            tw_vector = model.get_word_vector(target_word)
            sy_vector = model.get_word_vector(synonym)

            sim = cos_sim(tw_vector, sy_vector)

            if sim >= threshold:
                hits += 1

            samples_processed += 1

            if samples_processed >= samples:
                break

            #if samples_processed % 20000 == 0:
            #    print(">>Evaluated", samples_processed, "samples")

    hit_perc = 100.0 * float(hits) / float(samples)

    #print("samples:", samples)
    #print("threshold:", threshold)
    #print("hits:", hits)
    #print("hit_perc:", hit_perc, "%")

    return hit_perc


def synonym_evaluation_th_vs_ratio_models():
    samples = 20000
    ed_th = 5
    max_synonyms = 1
    synonyms_path = "/scratch/chatzaki/test-datasets/english-words/enwiki-synonyms.mw"+str(samples)+".ms"+str(max_synonyms)+".edth"+str(ed_th)

    thresholds = [0.4, 0.5, 0.6, 0.7, 0.8, 0.9]

    model = load_fastText(VECTORS_PATH)
    print(">>Model loaded, starting the evaluation...")

    results_fastText = {}

    for th in thresholds:
        res = evaluate_synonyms_fastText(model, samples, th, synonyms_path)
        results_fastText[th] = res

    plt.plot(list(results_fastText.keys()), list(results_fastText.values()),
             linestyle='--', marker='o', color='black', label="FastText")

    plt.ylabel("% Success Ratio")
    plt.xlabel("Similarity Threshold")
    plt.title("Success Rate: Synonyms - Sample=" + str(samples))
    plt.legend()
    plt.savefig("plots/synonyms.models.samples"+str(samples)+".ed"+str(ed_th)+".ms"+str(max_synonyms)+".png")
    plt.close()

    return


def synonym_evaluation_th_vs_ratio_ed():
    samples = 20000
    ed_ths = [0,1,2,3,4,5]
    max_synonyms = 1
    thresholds = [0.4, 0.5, 0.6, 0.7, 0.8, 0.9]
    synonyms_path = "/scratch/chatzaki/test-datasets/english-words/enwiki-synonyms.mw"+str(samples)+".ms"+str(max_synonyms)+".edth"+str(ed_th)

    model = load_fastText(VECTORS_PATH)
    print(">>Model loaded, starting the evaluation...")

    for ed_th in ed_ths:
        synonyms_path = "/scratch/chatzaki/test-datasets/english-words/enwiki-synonyms.mw"+str(samples)+".ms"+str(max_synonyms)+".edth"+str(ed_th)
        results_fastText = {}
        for th in thresholds:
            res = evaluate_synonyms_fastText(model, samples, th, synonyms_path)
            results_fastText[th] = res

        plt.plot(list(results_fastText.keys()), list(results_fastText.values()),
             linestyle='--', marker='o', label="FastText-ed"+str(ed_th))


    plt.ylabel("% Success Ratio")
    plt.xlabel("Similarity Threshold")
    plt.title("Success Rate: Synonyms - Sample=" + str(samples))
    plt.legend()
    plt.savefig("plots/synonyms.ed.samples"+str(samples)+".ms"+str(max_synonyms)+".png")
    plt.close()

    return


def synonym_evaluation_th_vs_ratio_ft_models():
    samples = 50000
    ed_th = 5
    max_synonyms = 1
    thresholds = [0.4, 0.5, 0.6, 0.7, 0.8, 0.9]
    synonyms_path = "/scratch/chatzaki/test-datasets/english-words/enwiki-synonyms.mw"+str(samples)+".ms"+str(max_synonyms)+".edth"+str(ed_th)

    #print(">>Model loaded, starting the evaluation...")

    models_path = [VECTORS_PATH, PRETRAINED_VECTORS_PATH, PRETRAINED_VECTORS_PATH_CRAWL]

    for mp in models_path:
        model = load_fastText(mp)
        results_fastText = {}
        for th in thresholds:
            res = evaluate_synonyms_fastText(model, samples, th, synonyms_path)
            results_fastText[th] = res

        name = ""
        if mp == VECTORS_PATH:
            name = "custom-wiki-2022"
        elif mp == PRETRAINED_VECTORS_PATH:
            name = "pretrained-wiki-2017"
        elif mp == PRETRAINED_VECTORS_PATH_CRAWL:
            name ="pretrained-crawl-2019"

        plt.plot(list(results_fastText.keys()), list(results_fastText.values()),
             linestyle='--', marker='o', label="ft-"+name)


    plt.ylabel("% Success Ratio")
    plt.xlabel("Similarity Threshold")
    plt.title("Success Rate: Synonyms - Sample=" + str(samples))
    plt.legend()
    plt.savefig("plots/synonyms.ftmodels.samples"+str(samples)+".ms"+str(max_synonyms)+".png")
    plt.close()

    return


def mispellings_evaluation_fastText(model, samples, threshold, dataset_path):
    
    hits = 0
    samples_processed = 0

    with open(dataset_path, 'r') as fp:
        for line in fp:
            
            #if line.isalnum() == False:
            #    continue
            
            data = line.strip().split()

            if(len(data) != 2):
                #print(data)
                continue

            mispelling = data[0]
            target_word = data[1]

            if mispelling.isalnum() == False or target_word.isalnum() == False:
                continue

            #print(mispelling, "->", target_word)

            tw_vector = model.get_word_vector(target_word)
            sy_vector = model.get_word_vector(mispelling)

            sim = cos_sim(tw_vector, sy_vector)

            if sim >= threshold:
                hits += 1

            samples_processed += 1

            if samples_processed >= samples:
                break
            
            if samples_processed % 10000 == 0:
                print(">>Evaluated", samples_processed, "samples")
            

    hit_perc = 100.0 * float(hits) / float(samples)

    return hit_perc


def mispellings_evaluation_th_vs_ratio_models():
    samples = 100000
    mispellings_path = "/scratch/chatzaki/test-datasets/mispellings/moe_misspellings_train.tsv"

    thresholds = [0.4, 0.5, 0.6, 0.7, 0.8, 0.9]

    model = load_fastText(VECTORS_PATH)
    print(">>Model loaded, starting the evaluation...")

    results_fastText = {}

    for th in thresholds:
        res = mispellings_evaluation_fastText(model, samples, th, mispellings_path)
        results_fastText[th] = res

    plt.plot(list(results_fastText.keys()), list(results_fastText.values()),
             linestyle='--', marker='o', color='blue', label="FastText")

    plt.ylabel("% Success Ratio")
    plt.xlabel("Similarity Threshold")
    plt.title("Success Rate: Mispellings - Sample=" + str(samples))
    plt.legend()
    plt.savefig("plots/mispellings.models.samples"+str(samples)+".png")
    plt.close()

    return


def mispellings_evaluation_th_vs_ratio_ftmodels():
    samples = 1000000
    thresholds = [0.4, 0.5, 0.6, 0.7, 0.8, 0.9]
    mispellings_path = "/scratch/chatzaki/test-datasets/mispellings/moe_misspellings_train.tsv"

    #print(">>Model loaded, starting the evaluation...")

    models_path = [VECTORS_PATH, PRETRAINED_VECTORS_PATH_CRAWL, PRETRAINED_VECTORS_PATH]

    for mp in models_path:
        model = load_fastText(mp)
        results_fastText = {}
        for th in thresholds:
            res = mispellings_evaluation_fastText(model, samples, th, mispellings_path)
            results_fastText[th] = res

        name = ""
        if mp == VECTORS_PATH:
            name = "custom-wiki-2022"
        elif mp == PRETRAINED_VECTORS_PATH:
            name = "pretrained-wiki-2017"
        elif mp == PRETRAINED_VECTORS_PATH_CRAWL:
            name ="pretrained-crawl-2019"

        plt.plot(list(results_fastText.keys()), list(results_fastText.values()),
             linestyle='--', marker='o', label="ft-"+name)


    plt.ylabel("% Success Ratio")
    plt.xlabel("Similarity Threshold")
    plt.title("Success Rate: Mispellings - Sample=" + str(samples))
    plt.legend()
    plt.savefig("plots/mispellings.ftmodels.samples"+str(samples)+".png")
    plt.close()

    return


def vector_dims():
    model = load_fastText(VECTORS_PATH)

    words = model.get_words()
    print("Total Words:", len(words))

    progress_count = 1

    max_dim = float("-inf")
    min_dim = float("inf")
    max_exp = 0
    for word in words:
        vec = model.get_word_vector(word)

        w_max_dim = max(vec)
        w_min_dim = min(vec)

        if w_max_dim > max_dim:
            max_dim = w_max_dim

        if w_min_dim < min_dim:
            min_dim = w_min_dim

        for dim in vec:
            #print(str(dim))
            exponent = decimal.Decimal(str(dim)).as_tuple().exponent

            if(abs(exponent) > abs(max_exp)):
                max_exp = exponent

        if progress_count % 100000 == 0:
            print("Word", progress_count)

        progress_count += 1
    
    print("Max:", max_dim)
    print("Min:", min_dim)
    print("MaxExp:", max_exp)

    return


def main():
    #produce_fastText_wiki_vectors()
    
    #synonym_evaluation_th_vs_ratio_models()

    #synonym_evaluation_th_vs_ratio_ed()

    #mispellings_evaluation_th_vs_ratio_models()

    #synonym_evaluation_th_vs_ratio_ft_models()

    #mispellings_evaluation_th_vs_ratio_ftmodels()

    vector_dims()

    return 0


if __name__ == "__main__":
    main()
