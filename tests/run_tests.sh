#!/bin/bash

for filename in ./*.cc; do
        echo "Running test $filename"
		g++ -Wfatal-errors -std=c++11 $filename && ./a.out
done
