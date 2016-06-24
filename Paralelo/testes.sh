#!/bin/sh 

for ((t=1; t<=5; t++)) do (time ./aco -t 1 scp41.txt) done > testes/41_1.txt 2>&1
for ((t=1; t<=5; t++)) do (time ./aco -t 2 scp41.txt) done > testes/41_2.txt 2>&1
for ((t=1; t<=5; t++)) do (time ./aco -t 4 scp41.txt) done > testes/41_4.txt 2>&1
for ((t=1; t<=5; t++)) do (time ./aco -t 8 scp41.txt) done > testes/41_8.txt 2>&1
for ((t=1; t<=2; t++)) do (time ./aco -t 16 scp41.txt) done > testes/41_16.txt 2>&1

for ((t=1; t<=5; t++)) do (time ./aco -t 1 scp55.txt) done > testes/55_1.txt 2>&1
for ((t=1; t<=5; t++)) do (time ./aco -t 2 scp55.txt) done > testes/55_2.txt 2>&1
for ((t=1; t<=5; t++)) do (time ./aco -t 4 scp55.txt) done > testes/55_4.txt 2>&1
for ((t=1; t<=5; t++)) do (time ./aco -t 8 scp55.txt) done > testes/55_8.txt 2>&1
for ((t=1; t<=2; t++)) do (time ./aco -t 16 scp55.txt) done > testes/55_16.txt 2>&1

for ((t=1; t<=5; t++)) do (time ./aco -t 1 scpd1.txt) done > testes/pd1_1.txt 2>&1
for ((t=1; t<=5; t++)) do (time ./aco -t 2 scpd1.txt) done > testes/pd1_2.txt 2>&1
for ((t=1; t<=5; t++)) do (time ./aco -t 4 scpd1.txt) done > testes/pd1_4.txt 2>&1
for ((t=1; t<=5; t++)) do (time ./aco -t 8 scpd1.txt) done > testes/pd1_8.txt 2>&1
for ((t=1; t<=2; t++)) do (time ./aco -t 16 scpd1.txt) done > testes/pd1_16.txt 2>&1

