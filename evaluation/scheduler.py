import subprocess
import os

MODEL_PATH = "/scratch/chatzaki/result-embeddings/fasttext-wikipedia-big-corpus/wiki-vector.bin"

#
#
# Utilities
#
#


def execute_command(command):
    print(">>>", command)
    out = subprocess.getoutput(command)
    print(out)


def export_var(name, value):
    os.environ[name] = value


def clean_dir(path):
    execute_command("rm -rf " + path)


#
#
# j2v Experiments
#
#

def j2v_experiments_join(name, ldataset_path, rdataset_path, threads, runs, th, preload, simd, dims, parallelism):

    base_path = str(name) + "-th" + str(th) + "/"

    execute_command("mkdir " + base_path)

    l_dataset_name = ""
    r_dataset_name = ""
    if(rdataset_path != ""):
        l_dataset_name = " --ldataset " + ldataset_path
        r_dataset_name = " --rdataset " + rdataset_path
    else:
        l_dataset_name = " --dataset " + ldataset_path
        r_dataset_name = ""

    for thread in threads:
        thread_path = base_path + "threads" + str(thread) + ""

        for run in range(runs):

            command = "./j2v --model " + MODEL_PATH + l_dataset_name + r_dataset_name + \
                " --threads " + str(thread) + " --threshold " + str(th) + \
                " --timers-file " + thread_path + " --parallelism " + parallelism

            if preload == True:
                command += " --preload-vectors"

            if simd == True:
                command += " --simd"

            if dims > 0:
                command += " --dims " + str(dims)

            execute_command(command)

    return


def j2v_experiment_join(lsamples, rsamples, threads, th, runs, ldataset_path, rdataset_path, preload, simd, dims, parallelism):
    simd_n = ""
    if simd == True:
        simd_n = "-simd"

    preload_n = ""
    if preload == True:
        preload_n = "-preload"

    dims_n = ""
    if dims > 0:
        dims_n = "-dims"+str(dims)

    rsamples_n = ""
    rdataset_path_n = ""
    if(rsamples > 0):
        rsamples_n = "-rsize"+str(rsamples)

    name = "../experiments/server42-j2v-wiki-nodup-lsize" + \
        str(lsamples)+rsamples_n + \
        preload_n+simd_n+dims_n+"-"+parallelism

    j2v_experiments_join(name, ldataset_path, rdataset_path,
                         threads, runs, th, preload, simd, dims, parallelism)

    return


#
#
# bf Experiments
#
#
def bf_experiments_join(lsamples, rsamples, threads, th, runs, ldataset_path, rdataset_path, parallelism):
    rsamples_n = ""
    if(rsamples > 0):
        rsamples_n = "-rsize"+str(rsamples)

    name = "../experiments/bruteforce-wiki-nodup-lsize" + \
        str(lsamples)+rsamples_n+"-"+str(parallelism)

    bf_experiment_join(name, ldataset_path, rdataset_path,
                       threads, runs, th, parallelism)
    return


def bf_experiment_join(name, ldataset_path, rdataset_path, threads, runs, th, parallelism):
    parallelism_n = {}
    parallelism_n["fai"] = 0
    parallelism_n["data-parallel"] = 1

    base_path = str(name) + "-th" + str(th) + "/"

    execute_command("mkdir " + base_path)

    l_dataset_name = ""
    r_dataset_name = ""
    if(rdataset_path != ""):
        l_dataset_name = " " + ldataset_path
        r_dataset_name = " " + rdataset_path
    else:
        l_dataset_name = " " + ldataset_path
        r_dataset_name = ""

    for thread in threads:
        thread_path = base_path + "threads" + str(thread) + ""

        for run in range(runs):
            command = "./bf " + l_dataset_name + r_dataset_name + " " + \
                str(th) + " " + str(parallelism_n[parallelism]) + \
                " " + str(thread) + " " + thread_path
            execute_command(command)

    return


def main():
    lsamples = 10000
    rsamples = 10000

    threads = [48, 36, 24, 12, 8, 4, 2, 1]
    th = 0.9
    th_ed = 2
    runs = 3

    dataset_path = "/scratch/chatzaki/test-datasets/english-words/rs-sim-join/v1-wiki-nodup-" + \
        str(lsamples)+""
    ldataset_path = "/scratch/chatzaki/test-datasets/english-words/rs-sim-join/v1-wiki-nodup-10000"
    rdataset_path = "/scratch/chatzaki/test-datasets/english-words/rs-sim-join/v2-wiki-nodup-10000"

    dims = -1
    
    preloads = [False]
    simds = [True]
    for preload in preloads:
        for simd in simds:
            parallelism = "fai"
            j2v_experiment_join(lsamples, rsamples, threads, th, runs,
                        ldataset_path, rdataset_path, preload, simd, dims, parallelism)  # R-S Join
             
            j2v_experiment_join(lsamples, -1, threads, th, runs,
                        ldataset_path, "", preload, simd, dims, parallelism)  # Self Join
            
            parallelism = "data-parallel"
            j2v_experiment_join(lsamples, rsamples, threads, th, runs,
                        ldataset_path, rdataset_path, preload, simd, dims, parallelism)  # R-S Join
            j2v_experiment_join(lsamples, -1, threads, th, runs,
                        dataset_path, "", preload, simd, dims, parallelism)  # Self Join


    return


if __name__ == "__main__":
    main()
