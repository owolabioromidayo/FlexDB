#!/bin/bash
for filename in ./*.cc; do
		g++ -std=c++11 $filename && ./a.out
done
