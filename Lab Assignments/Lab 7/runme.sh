#!/bin/bash
echo "Choose a number to implement the corresponding polacy:
1 for random
2 for optimal
3 for lru
4 for clock"
read policy
if [ $policy == "1" ]; then
    gcc random.c -o random 
    ./random > random.dat 
    gnuplot random.p
elif [ $policy == "2" ]; then
    gcc optimal.c -o optimal 
    ./optimal > optimal.dat 
    gnuplot optimal.p
elif [ $policy == "3" ]; then
    gcc lru.c -o lru
    ./lru > lru.dat
    gnuplot lru.p
elif [ $policy == "4" ]; then
    gcc clock.c -o clock
    ./clock > clock.dat
    gnuplot clock.p
fi