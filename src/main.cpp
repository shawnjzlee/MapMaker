#include <vector>
#include <iostream>
#include <chrono>

#include "./lib/cmdline.h"

typedef unsigned int uint;

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

uint get_ceiling(uint constraint) {
    int val(0);
    while (digit_sum_within_constraints(constraint, val))
        val++;
    return val;
}

int main(int argc, char * argv[]) {
    cmdline::parser flags;
    flags.add<uint>("constraint", 'c', "defines minimum digit sum of the absolute value of the coordinate's digits (unsigned int)", false, 21);
    flags.parse_check(argc, argv);
    
    uint constraint(flags.get<uint>("constraint"));
    
    uint size = get_ceiling(constraint);
    
    std::cout << "Size: " << size << std::endl;
}