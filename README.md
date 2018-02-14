# Map-Maker

## Problem Overview
John is a map maker. He is standing at point (0,0) and must explore the area. John can move one space at a time: left, right, up, or down. If John is standing at (x,y), he can move to (x+1, y), (x-1, y), (x, y+1), or (x, y-1). 

Bill is an assassin, and must stop John from exploring. Therefore, Bill decided to plant mines. Bill has placed mines at points where the sum of the absolute value of the coordinate's digits is greater than 21. For example, the point (59, -79) has a mine, because 5 + 9 + 7 + 9 > 21. But the point (-113, -104) has no mine because 1 + 1 + 3 + 1 + 0 + 4 <= 21.

If John steps on a mine he will die. John cannot jump over the mines, so he must walk around the mines to avoid them. How many points can John access while making his map, given that he starts at (0, 0).

## Assumptions
Given the digit sum is the absolute value of the coordinate system, we can simply calculate the number of valid points in one quadrant and multiply that value by 4. We will then remove the overlapping points (those at the axis) to get our final answer.

Our program will use unsigned values, and perform calculations in Quadrant I.

## Usage via Makefile

#### In `/src` directory:

To compile:

- prereq: `mapmaker` requires g++ 5.x and c++14 to compile and run.
- `make` will compile the program with all of the following flags `-pthread -lpthread -c -std=c++14 -Wall -Werror`, and put the program in the `/bin` directory.
- `make debug` will compile with the flags listed above, as well as `-debug -g` for debugging with, e.g., `gdb`.
- `make bench` will compile with the `#ifdef BENCH` preprocessors in the code, which will benchmark and output the runtime of the program.

To run:

- `make test` will run the program serially with default constraint value. 
- `make test_c ARG1=[constraint]` will run the program serially with constraint value passed in as an argument.
- `make test_t ARG1=[num_threads]` will run the program concurrently with number of threads passed in as an argument.
- `make test_d` will run the program serially with default constraint value in `gdb`.
- `make test_cd ARG1=[constraint]` will run with `_c` and `_d` combined. 
- `make test_ct ARG1=[num_threads]` will run with `_c` and `_t` combined. 
- `make test_td ARG1=[num_threads]` will run with `_t` and `_d` combined.
- `make test_ctd ARG1=[constraint] ARG2=[num_threads]` will run with `_c`, `_t`, and `_d` combined.