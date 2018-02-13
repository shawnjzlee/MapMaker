#include <vector>
#include <iostream>
#include <chrono>

#include "./lib/cmdline.h"

typedef unsigned int uint;

bool digit_sum_within_constraints(uint, uint, uint);

// Returns the number of valid, accessible points in Quadrant I, and returns the calculated number of points valid, accessible points for all four quadrants (final answer).
uint find_valid_points(uint ceiling, uint constraint) {
    std::vector<std::vector<bool>> valid_cells(ceiling, std::vector<bool>(ceiling, false));
    
    uint raw_count = 0;
    
    for (int x = 0; x < ceiling; x++) {
        for (int y = 0; y < ceiling; y++) {
            if (digit_sum_within_constraints(constraint, x, y)) {
                if (x == 0 || y == 0) {
                    valid_cells[x][y] = true;
                    raw_count += 1;
                }
                else if (valid_cells[x-1][y] == true || valid_cells[x][y-1] == true) {
                    valid_cells[x][y] = true;
                    raw_count += 1;
                }
            }
        }
    }
    
    uint count = (raw_count * 4) - (ceiling * 4) + 1;
    return count;
}

// Returns true if the sum of digits, `sum`, given x-coord `x` and y-coord `y` are less than or equal to constraint `constraint`.
bool digit_sum_within_constraints(uint constraint, uint x, uint y = 0) {
    int sum(0);
    
    while (x != 0) {
        sum = sum + (x % 10);
        x /= 10;
        if (sum > constraint) return false;
    }
    while (y != 0) {
        sum = sum + (y % 10);
        y /= 10;
        if (sum > constraint) return false;
    }
    return true;
}

// Returns the smallest excluding number, `n` with given sum of digits `c`
uint get_ceiling(uint constraint) {
    int val(0);
    while (digit_sum_within_constraints(constraint, val))
        val++;
    return val;
}

// Main driver
// ./mapmaker [-c, --constraint]
// -c, --constraint (unsigned int): defines the min digit sum of the absolute value of the coordinate's digits (default: 21).
int main(int argc, char * argv[]) {
    cmdline::parser flags;
    flags.add<uint>("constraint", 'c', "defines minimum digit sum of the absolute value of the coordinate's digits (unsigned int)", false, 21);
    flags.parse_check(argc, argv);
    
    uint constraint(flags.get<uint>("constraint"));
    uint ceiling = get_ceiling(constraint);
    
    #ifdef BENCH
    auto start = std::chrono::high_resolution_clock::now();
    #endif
    
    uint count = find_valid_points(ceiling, constraint);
    
    #ifdef BENCH
    auto end = std::chrono::high_resolution_clock::now();
    auto runtime = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
    
    std::cout << "John took " << runtime.count() << "s to calculate all valid points.\n";
    #endif
    
    std::cout << "John can safely access " << count << " points on the map, given that he starts at (0,0).\n";
}