#!/bin/python3
import random

sizes = {'1K'   : 1000,
         '1M'   : 1000000,
         '10M'  : 10000000}

for k, v in sizes.items():
    file = open(k + '.json', 'w')

    print("Generating " + k + " random numbers")

    rand = random.sample(range(2**63-1), v)

    print("Sorting generated numbers")

    rand_sorted = sorted(rand)

    print("Writing to file")

    file.write("[")
    for r in rand_sorted[:-1]:
        file.write(str(r))
        file.write(",")
    file.write(str(rand_sorted[-1]))
    file.write("]")

    print("Done")
