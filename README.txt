To make Makefiles using CMake:
    cmake -S . -B build


To make project using made Makefiles:
    cmake --build build

To run project:
    build/main

To run tests:
    gcc $(find *.c ! -name "main.c") -lcriterion && ./a.out

