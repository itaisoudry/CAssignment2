#!/bin/bash
gcc dispatcher.c -o dispatcher
gcc counter.c -o counter
./dispatcher b file.txt

