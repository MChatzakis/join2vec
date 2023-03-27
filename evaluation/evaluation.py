import subprocess
import os


JOIN_TIME_INDEX = 5
PREPROCESSING_TIME_INDEX = 1


def execute_command(command):
    print(">>>", command)
    out = subprocess.getoutput(command)
    print(out)


def export_var(name, value):
    os.environ[name] = value


def process_file(dir, variable_index, runs):

    file = open(dir)

    lines = file.readlines()

    current_run = 1
    result = float(0)
    for line in lines:

        currentLine = line.split(" ")

        if currentLine == ['\n']:
            continue

        result += float(currentLine[variable_index])

        current_run += 1

        if(current_run > runs):
            break

    return result/runs


def evaluate_j2v(path, threads, runs):

    join_results = {}
    preprocessing_results = {}
    
    scalab_results = {}
    
    all_results = {}

    for thread in threads:
        exp_path = path + "threads" + str(thread)

        join = process_file(exp_path, JOIN_TIME_INDEX, runs)
        prepro = process_file(exp_path, PREPROCESSING_TIME_INDEX, runs)

        join_results[thread] = join
        preprocessing_results[thread] = prepro

        all_results[thread] = (join+prepro)
    
    #for thread in threads:
    #    scalab_results[thread] = all_results[1]/all_results[thread]

    print("All:", all_results)
    print("Join:", join_results)
    print("Prepro:", preprocessing_results)

    print("Scalalability:", scalab_results)
    return


def create_plot(xvals, yvals, xaxis, yaxis, title, path):

    plt.plot(xvals, yvals, linestyle='--', marker='o')

    plt.xlabel(xaxis)
    plt.ylabel(yaxis)
    plt.title(title)

    plt.legend()

    plt.savefig(path)
    plt.close()

    return


def main():

    threads = [48, 36, 24, 12, 8, 4,2,1]
    samples = 100000
    runs = 3
    th = 0.9

    preload = True
    simd = True
    dims = -1

    simd_n = ""
    if simd == True:
        simd_n = "-simd"

    preload_n = ""
    if preload == True:
        preload_n = "-preload"

    dims_n = ""
    if dims > 0:
        dims_n = "-dims"+str(dims)

    n = "../experiments/server42-j2v-wiki-nodup-lsize10000-rsize10000-preload-simd-data-parallel-th0.9/"
    
    evaluate_j2v(n, threads, runs)

    return


if __name__ == "__main__":
    main()
