#!/bin/bash

echo "Bash script running"
make
./test1

echo "This is what the vdisk looks like: "
hexdump -C vdisk
./test2

echo "This is what the vdisk looks like now: "
hexdump -C vdisk

./test3
echo "End of test script"