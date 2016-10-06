
sizes = {'1kb'   : 1000,
         '1mb'   : 1000000,
         '10mb'  : 10000000,
         '100mb' : 100000000,
         '1gb'   : 1000000000}


for k, v in sizes:
    file = open(k + '.json')

