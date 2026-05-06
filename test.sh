#!/bin/bash
gcc $(find *.c ! -name "main.c") -lcriterion && ./a.out
