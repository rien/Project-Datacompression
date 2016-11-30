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
    export BENCHMARKFILE='default_randsum.csv'
    export orig="files/${file}_randsum.json"
    export comp="compressed/${file}_randsum.json"
    export rest="decompressed/${file}_randsum.json"
    BENCHMARKFILE=standaard_compression_randsum.csv ./out/standaard -c $orig $comp
    BENCHMARKFILE=standaard_decompression_randsum.csv ./out/standaard -d $comp $rest
    cmp --silent $orig $rest
    BENCHMARKFILE=specifiek_compression_randsum.csv ./out/specifiek -c $orig $comp
    BENCHMARKFILE=specifiek_decompression_randsum.csv ./out/specifiek -d $comp $rest
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
    BENCHMARKFILE=standaard_compression_uniform.csv ./out/standaard -c $orig $comp
    BENCHMARKFILE=standaard_decompression_uniform.csv ./out/standaard -d $comp $rest
    cmp --silent $orig $rest
    BENCHMARKFILE=specifiek_compression_uniform.csv ./out/specifiek -c $orig $comp
    BENCHMARKFILE=specifiek_decompression_uniform.csv ./out/specifiek -d $comp $rest
    cmp --silent $orig $rest
    )
done

# Test blocksize
for bs in ${blocksize[@]}
do
    (
    file="10.000K"
    export BENCHMARKFILE='blocksize.csv'
    export BLOCKSIZE=$bs
    export orig="files/${file}_uniform.json"
    export comp="compressed/${file}_uniform.json"
    export rest="decompressed/${file}_uniform.json"
    BENCHMARKFILE=standaard_compression_blocksize.csv ./out/standaard -c $orig $comp
    BENCHMARKFILE=standaard_decompression_blocksize.csv ./out/standaard -d $comp $rest
    cmp --silent $orig $rest
    BENCHMARKFILE=specifiek_compression_blocksize.csv ./out/specifiek -c $orig $comp
    BENCHMARKFILE=specifiek_decompression_blocksize.csv ./out/specifiek -d $comp $rest
    cmp --silent $orig $rest
    )
done
