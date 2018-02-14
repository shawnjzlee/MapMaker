#include <iostream>
#include <vector>

#include "mapmaker.h"
#include "global.h"

MapMaker::MapMaker() { }

MapMaker::~MapMaker() { }

void MapMaker::thread_data_init(unsigned int thread_id, unsigned int constraint) {
    this->thread_id = thread_id;
    this->constraint = constraint;
    this->ceiling = get_ceiling();
    this->local_count = 0;
    this->local_update = true;
}

short MapMaker::get_thread_id() const {
    return thread_id;
}

void MapMaker::set_thread_id(int thread_id) {
    this->thread_id = thread_id;
}

unsigned int MapMaker::get_lbound() const {
    return lbound;
}

unsigned int MapMaker::get_rbound() const {
    return rbound;
}

void MapMaker::set_bounds(unsigned int lbound, unsigned int rbound) {
    this->lbound = lbound;
    this->rbound = rbound;
}

unsigned int MapMaker::get_local_count() const {
    return local_count;
}

unsigned int MapMaker::get_ceiling() const {
    return ceiling;
}

unsigned int MapMaker::get_constraint() const {
    return constraint;
}

void MapMaker::set_contraint(unsigned int constraint) {
    this->constraint = constraint;
}

void MapMaker::get_valid_points() {
    // used for last else-if below to lock_guard for neighbor checking
    auto check = [this](std::vector<std::vector<short>>& valid_cells, int x, int y) {
        std::lock_guard<std::mutex> lock(*(mutex_map.at(x-1))); {
            std::lock_guard<std::mutex> lock(*(mutex_map.at(x))); {
                return (valid_cells[x-1][y] == 1 || valid_cells[x][y-1] == 1);
            }
        }
    };
    
    local_update = false;
    for (unsigned int x = lbound; x <= rbound; ++x) {
        for (unsigned y = 0; y <= ceiling; ++y) {
            if (digit_sum_within_constraints(x, y) && valid_cells[x][y] != 1) {
                // if `x` or `y` are at the axis, and `x` is not within shared boundaries
                if (x == 0 || (x != lbound && y == 0)) {
                    valid_cells[x][y] = 1;
                    local_update = true;
                    local_count++;
                }
                // else if `x` is not within shared boundaries 
                else if (x != lbound) {
                    if (valid_cells[x-1][y] == 1 || valid_cells[x][y-1] == 1) {
                        valid_cells[x][y] = 1;
                        local_update = true;
                        local_count++;
                    }
                }
                // else if cell is accessible from a neighbor within shared boundaries
                else if (check(valid_cells, x, y)) {
                    std::lock_guard<std::mutex> lock(*(mutex_map.at(x)));
                    valid_cells[x][y] = 1;
                    local_update = true;
                    local_count++;
                }
            }
        }
    }
    
    // check if local portion of valid_cells has been updated, update global if new cells updated
    std::lock_guard<std::mutex> lock(mutex_updated);
    if (!updated && local_update)
        updated = true;
}

bool MapMaker::digit_sum_within_constraints(unsigned int x, unsigned int y) {
    unsigned int sum = 0;
    
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

unsigned int MapMaker::get_ceiling() {
    int val = 0;
    while (digit_sum_within_constraints(val, 0))
        val++;
    return val;
}
