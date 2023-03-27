ds_0 = "/scratch/chatzaki/test-datasets/english-words/sim-join/wiki-nodup-1000000"
ds_1 = "/scratch/chatzaki/test-datasets/english-words/sim-join/wiki-nodup-100000"
ds_2 = "/scratch/chatzaki/test-datasets/english-words/sim-join/wiki-nodup-10000"
ds_3 = "/scratch/chatzaki/test-datasets/english-words/sim-join/wiki-nodup-1000"
ds_4 = "/scratch/chatzaki/test-datasets/english-words/sim-join/wiki-nodup-100"

ds_5 = "/scratch/chatzaki/test-datasets/english-words/sim-join/wiki-1000000"
ds_6 = "/scratch/chatzaki/test-datasets/english-words/sim-join/wiki-100000"
ds_7 = "/scratch/chatzaki/test-datasets/english-words/sim-join/wiki-10000"
ds_8 = "/scratch/chatzaki/test-datasets/english-words/sim-join/wiki-1000"
ds_9 = "/scratch/chatzaki/test-datasets/english-words/sim-join/wiki-100"

dataset_path = ds_5

size_sum = 0
word_count = 0
with open(dataset_path, 'r') as fp:
    for line in fp:
        word = line.strip()
        #print(word, end=", ")

        word_size = len(word)

        size_sum += word_size
        word_count += 1


avg_len = (size_sum*1.0)/word_count
print(dataset_path,"->",avg_len,"(",word_count,"words )")
