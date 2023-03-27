import nltk 
from nltk.corpus import wordnet 
from nltk.corpus import stopwords

#nltk.download("wordnet", "/scratch/chatzaki/nltk_data/")
nltk.data.path.append('/scratch/chatzaki/nltk_data/')

MAX_WORDS = 20000
MAX_SYNONYMS = 1
ED_TH = 4

WORD_PATH = "/scratch/chatzaki/test-datasets/english-words/enwiki-20190320-words-frequency.txt"
SAVE_PATH = "/scratch/chatzaki/test-datasets/english-words/enwiki-synonyms.mw"+str(MAX_WORDS)+".ms"+str(MAX_SYNONYMS)+".edth"+str(ED_TH)


stopwords = stopwords.words('english')

def is_verb_or_noun(word):
    
    if len(wordnet.synsets(word)) == 0:
        return False

    tmp = wordnet.synsets(word)[0].pos()
    if tmp == "v" or tmp == "n":
        return True

    return False

def get_synonyms(word, max):
    synonym = []
    
    for synset in wordnet.synsets(word): 
        count = 0 
        for i in synset.lemmas(): 
            if(nltk.edit_distance(i.name().lower(), word) > ED_TH):
                synonym.append(i.name().lower())
                count += 1
                if count >= max:
                    break
        if count >= max:
                break
        
    synonym = set(synonym)
    if word in synonym:
        synonym.remove(word)
    return synonym


output_file = open(SAVE_PATH, 'w')

count = 0
with open(WORD_PATH, 'r') as fp:
    for line in fp:
        word = line.strip().split(' ')[0]
        #print(word)
        #word = "national"

        if word in stopwords:
            continue

        if is_verb_or_noun(word) == False:
            continue

        
        synonyms = get_synonyms(word, MAX_SYNONYMS)
        if (len(synonyms) > 0):
            str = word + " "
            for s in synonyms:
                str += s + " "
            output_file.write(str + "\n")
            #print(str)
        else:
            continue

        count += 1
        if count >= MAX_WORDS:
            break

        if count % 10000 == 0:
            print(">>Generated", count, "pairs")
