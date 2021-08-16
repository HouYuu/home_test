#!/bin/bash

# build
gcc main.c solution.c -o home_test

# run
chmod +x ./home_test
./home_test

# clean
rm ./home_test
