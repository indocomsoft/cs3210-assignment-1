# import sys
# from collections import defaultdict


# def parse(filename):
#     try:
#         with open(filename, "r") as f:
#             results = f.readlines()
#             print_by_line_by_time(
#                 list(filter(lambda x: x[0] == "|", results[1:])))
#     except Exception as e:
#         print(e)


# def print_by_line_by_time(results):
#     # Taking advantage of the fact that built in sort is stable

#     # Sort by time
#     results.sort(key=lambda r: int([x.strip()
#                                     for x in filter(None, r.split("|"))][0]))

#     # Sort by train id
#     results.sort(key=lambda r: int([x.strip()
#                                     for x in filter(None, r.split("|"))][1][1]))

#     # Group by line
#     results.sort(key=lambda r: [x.strip()
#                                 for x in filter(None, r.split("|"))][1][0])

#     for r in results:
#         print(r.strip())


# if __name__ == "__main__":
#     filename = sys.argv[1]
#     parse(filename)


import os
import csv
import re

intfinder = re.compile(r'\d+')

lines = ["green", "yellow", "blue"]
stat_headers = "numGreen,numYellow,numBlue,avgGreen,maxGreen,minGreen,avgYellow,maxYellow,minYellow,avgBlue,maxBlue,minBlue".split(",")
perf_headers = "branch-misses,cache-misses,instructions,cycles,context-switches,time".split(",")
headers = stat_headers + perf_headers

results = []


def parse():
    # files = list(filter(lambda x: not (x.endswith(".py")
                                    #    or x.endswith(".csv")), os.listdir(".")))
    files = os.listdir(".")
    for f in files:
        process(f)

    write_results()


def write_results():
    with open("perf_results.csv", "w") as out_file:
        csv_w = csv.writer(out_file)
        csv_w.writerow(headers)
        csv_w.writerows(results)


def process(f):
    try:
        key = tuple([int(i) for i in f.split('-')])
    except:
        return

    if sum(key) <= 0:
        return

    contents = list(filter(None, open(f, 'r').readlines()))
    entry = [-1 for i in range(len(headers))]
    threads = 0
    for row in contents:
        for line in lines:
            if row.startswith(line):
                offset = lines.index(line)
                timings = [float(i)
                           for i in row.split("->")[1].strip().split(",")]
                entry[offset] = key[offset]

                for i in range(3):
                    entry[(offset+1)*3+i] = timings[i]

        for perf_stat in perf_headers:
            if perf_stat in row:
                val = float("".join([e.split(",") for e in row.split() if e][0]))
                entry[headers.index(perf_stat)] = val

        if "threads" in row:
            found = intfinder.findall(row)
            if found:
                threads = int(found[0])

    if threads == 0 or threads != sum(key):
        print("error in %s: mismatch in thread number. expected: %d, actual: %d" % (
            f, sum(key), threads))
    elif any([e == -1 for e in entry]):
        for i, e in enumerate(entry):
            if e == -1:
                print("error in %s: value not found for column %s" % (f, headers[i]))
    else:
        results.append(entry)


if __name__ == "__main__":
    parse()
