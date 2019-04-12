#!/bin/bash

echo "Bash script running"
gcc test1.c -o test1 RW.c -std=c99
./test1

echo "This is what the vdisk looks like: "
hexdump -C vdisk

gcc test2.c -o test2 RW.c -std=c99
./test2

echo "This is what the vdisk looks like now: "
hexdump -C vdisk

gcc test3.c -o test3 RW.c -std=c99
./test3

echo "End of script"