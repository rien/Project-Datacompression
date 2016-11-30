#!/usr/bin/env bash

set -o errexit
set -o pipefail
set -o nounset

testfiles=("1K"
           "5K"
           "10K"
           "50K"
           "100K"
           "500K"
           "1.000K"
           "5.000K"
           "10.000K"
           "50.000K")

blocksize=(1028 2048 3076 4096 6144 8192 12288 16384 24576 32768 49152)

# Test default options, random sum
for file in ${testfiles[@]}
do
    (
    export orig="files/${file}_randsum.json"
    export comp="compressed/${file}_randsum.json"
    export rest="decompressed/${file}_randsum.json"
    BENCHMARKFILE=extra/testresults/randsum_standaard_compression.csv ./out/standaard -c $orig $comp
    BENCHMARKFILE=extra/testresults/randsum_standaard_decompression.csv ./out/standaard -d $comp $rest
    cmp --silent $orig $rest
    BENCHMARKFILE=extra/testresults/randsum_specifiek_compression.csv ./out/specifiek -c $orig $comp
    BENCHMARKFILE=extra/testresults/randsum_specifiek_decompression.csv ./out/specifiek -d $comp $rest
    cmp --silent $orig $rest
    )
done

# Test default options, uniform distribution
for file in ${testfiles[@]}
do
    (
    export orig="files/${file}_uniform.json"
    export comp="compressed/${file}_uniform.json"
    export rest="decompressed/${file}_uniform.json"
    BENCHMARKFILE=extra/testresults/uniform_standaard_compression.csv ./out/standaard -c $orig $comp
    BENCHMARKFILE=extra/testresults/uniform_standaard_decompression.csv ./out/standaard -d $comp $rest
    cmp --silent $orig $rest
    BENCHMARKFILE=extra/testresults/uniform_specifiek_compression.csv ./out/specifiek -c $orig $comp
    BENCHMARKFILE=extra/testresults/uniform_specifiek_decompression.csv ./out/specifiek -d $comp $rest
    cmp --silent $orig $rest
    )
done

# Test blocksize
for bs in ${blocksize[@]}
do
    (
    file="10.000K"
    export BLOCKSIZE=$bs
    export orig="files/${file}_uniform.json"
    export comp="compressed/${file}_uniform.json"
    export rest="decompressed/${file}_uniform.json"
    BENCHMARKFILE=extra/testresults/blocksize_standaard_compression.csv ./out/standaard -c $orig $comp
    BENCHMARKFILE=extra/testresults/blocksize_standaard_decompression.csv ./out/standaard -d $comp $rest
    cmp --silent $orig $rest
    BENCHMARKFILE=extra/testresults/blocksize_specifiek_compression.csv ./out/specifiek -c $orig $comp
    BENCHMARKFILE=extra/testresults/blocksize_specifiek_decompression.csv ./out/specifiek -d $comp $rest
    cmp --silent $orig $rest
    )
done
