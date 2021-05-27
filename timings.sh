#!/usr/bin/bash
make clean
make
echo ""
echo "100"
time ./conj 100 data/100dim.txt 6
./check.py --size 100 --data data/100dim.txt
echo ""
echo "1000"
time ./conj 1000 data/1000dim.txt 6
./check.py --size 1000 --data data/1000dim.txt
echo ""
echo "2000"
time ./conj 2000 data/2000dim.txt 6
./check.py --size 2000 --data data/2000dim.txt
echo ""
echo "4000"
time ./conj 4000 data/4000dim.txt 6
./check.py --size 4000 --data data/4000dim.txt
echo ""
echo "5000"
time ./conj 5000 data/5000dim.txt 6
./check.py --size 5000 --data data/5000dim.txt
