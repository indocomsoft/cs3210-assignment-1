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
headers = "numGreen,numYellow,numBlue,avgGreen,maxGreen,minGreen,avgYellow,maxYellow,minYellow,avgBlue,maxBlue,minBlue,time".split(
    ",")

results = []


def parse():
    # files = list(filter(lambda x: not (x.endswith(".py")
                                    #    or x.endswith(".csv")), os.listdir(".")))
    files = os.listdir(".")
    for f in files:
        process(f)

    write_results()


def write_results():
    with open("results.csv", "w") as out_file:
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

    contents = open(f, 'r').readlines()
    entry = [0 for i in range(13)]
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

        if row.startswith("real"):
            time = row.split()[1]
            mins = float(time.split("m")[0])
            secs = float(time.split("m")[1].strip("s"))
            entry[12] = mins * 60 + secs

        if "threads" in row:
            found = intfinder.findall(row)
            if found:
                threads = int(found[0])

    if threads == 0 or threads != sum(key):
        print("error in %s: mismatch in thread number. expected: %d, actual: %d" % (
            f, sum(key), threads))
    else:
        results.append(entry)


if __name__ == "__main__":
    parse()
