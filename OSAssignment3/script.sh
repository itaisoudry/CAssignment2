#!/bin/bash
gcc dispatcher.c -o dispatcher
gcc counter.c -o counter
./dispatcher a a.txt

