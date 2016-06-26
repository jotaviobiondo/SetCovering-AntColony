#!/bin/sh 


for ((t=1; t<=10; t++)) do (time ./aco scp41.txt) done > testes3/41_seq.txt 2>&1

for ((t=1; t<=10; t++)) do (time ./aco scp55.txt) done > testes3/55_seq.txt 2>&1

for ((t=1; t<=10; t++)) do (time ./aco scpa1.txt) done > testes3/a1_seq.txt 2>&1

for ((t=1; t<=10; t++)) do (time ./aco scpd1.txt) done > testes3/d1_seq.txt 2>&1
