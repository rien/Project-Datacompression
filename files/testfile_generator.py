#!/bin/python3
import random

def uniform_random(k,v, file):
    print("Generating " + k + " uniform random numbers")

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

def random_sum(k, v, file):
    last = 0

    print("Adding " + k + " random numbers.")

    file.write('[')
    for i in range(v):
        last += random.randint(0, 2**32-1)
        file.write(str(last))
        if i != v-1:
            file.write(',')
    file.write(']')

    print("Done")

sizes = {'10.000K':10000000}#'1K'     : 1000,
         #'2K'     : 2000,
         #'3K'     : 3000,
         #'5K'     : 5000,
         #'10K'    : 10000,
         #'20K'    : 20000,
         #'30K'    : 30000,
         #'50K'    : 50000,
         #'100K'   : 100000,
         #'200K'   : 200000,
         #'300K'   : 300000,
         #'500K'   : 500000,
         #'1.000K' : 1000000,
         #'2.000K' : 2000000,
         #'3.000K' : 3000000,
         #'5.000K' : 5000000}

for k, v in sizes.items():

    uni_file = open(k + '_uniform.json', 'w')
    uniform_random(k, v, uni_file)

    randsum_file = open(k + '_randsum.json', 'w')
    random_sum(k,v, randsum_file)


