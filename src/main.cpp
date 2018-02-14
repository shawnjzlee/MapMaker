#include <vector>
#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>

#include "mapmaker.h"
#include "global.h"
#include "./lib/cmdline.h"

std::map<int, std::unique_ptr<std::mutex>> mutex_map;
std::vector<std::vector<short>> valid_cells;
pthread_barrier_t barrier_threshold;
std::mutex mutex_updated;
bool updated = true;

typedef unsigned int uint;

bool digit_sum_within_constraints(uint, uint, uint);

int get_raw_count(std::vector<MapMaker> thread_data) {
    int raw_count = 0;
    for (uint i = 0; i < thread_data.size(); i++) {
        raw_count += thread_data.at(i).get_local_count();
    }
    return raw_count;
}

void get_points(MapMaker &data) {
    #ifdef BENCH
    std::chrono::high_resolution_clock::time_point start;
    if (data.get_thread_id() == 0)
        start = std::chrono::high_resolution_clock::now();
    #endif
    
    do {
        pthread_barrier_wait (&barrier_threshold);
        {
            std::lock_guard<std::mutex> lock(mutex_updated);
            updated = false;
        }
        data.get_valid_points();
        pthread_barrier_wait (&barrier_threshold);
    } while(updated);
    
    pthread_barrier_wait (&barrier_threshold);
    #ifdef BENCH
    std::chrono::high_resolution_clock::time_point end;
    if (data.get_thread_id() == 0) {
        end = std::chrono::high_resolution_clock::now();
        auto runtime = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
        std::cout << "John took " << runtime.count() << "s to calculate all valid points.\n";
    }
    #endif
}

// Main driver
// ./mapmaker [-c, --constraint] [-t, --threads]
// -c, --constraint (unsigned int): defines the min digit sum of the absolute value of the coordinate's digits (default: 21).
// -t, --threads (unsigned int): number of threads to spawn (default: 1 (serial)).
int main(int argc, char * argv[]) {
    cmdline::parser flags;
    flags.add<uint>("constraint", 'c', "defines minimum digit sum of the absolute value of the coordinate's digits (unsigned int)", false, 21);
    flags.add<uint>("threads", 't', "number of threads to spawn (unsigned int)", false, 1);
    flags.parse_check(argc, argv);
    
    uint constraint(flags.get<uint>("constraint"));
    uint num_threads(flags.get<uint>("threads"));
    
    std::vector<std::thread> threads(num_threads);
    std::vector<MapMaker> thread_data(num_threads);
    for (uint tid = 0; tid < num_threads; ++tid)
        thread_data.at(tid).thread_data_init(tid, constraint);
    
    const uint ceiling = thread_data.at(0).get_ceiling();
    const uint work_per_thread = ceiling / num_threads;
    const uint remaining_work = ceiling % num_threads;
    
    for (uint i = 0; i < ceiling; i++)
        valid_cells.push_back(std::vector<short>(ceiling, 0));
    
    pthread_barrier_init(&barrier_threshold, NULL, num_threads);
    
    uint offset = 0;
    for (uint tid = 0; tid < num_threads; ++tid) {
        uint chunksize = (tid < remaining_work) ? work_per_thread + 1 : work_per_thread;
        
        if (tid) {
            mutex_map.emplace(offset, std::make_unique<std::mutex>()).first;
            mutex_map.emplace(offset+1, std::make_unique<std::mutex>()).first;
        }
        
        if (tid) 
            thread_data.at(tid).set_bounds(offset+1, offset+chunksize);
        else 
            thread_data.at(tid).set_bounds(offset, offset+chunksize);
            
        offset += chunksize;
        
        #ifdef VERBOSE
        std::cout << "TID: " << thread_data.at(tid).get_thread_id() << std::endl;
        std::cout << "Constraint: " << thread_data.at(tid).get_constraint() << std::endl;
        std::cout << "Size: " << thread_data.at(tid).get_ceiling() << std::endl;
        std::cout << "Lbound: " << thread_data.at(tid).get_lbound() << std::endl;
        std::cout << "Rbound: " << thread_data.at(tid).get_rbound() << std::endl;
        std::cout << "--\n";
        #endif
        
        if (num_threads != 1)
            threads[tid] = std::thread(get_points, std::ref(thread_data.at(tid)));
        else
            get_points(thread_data.at(tid));
    }
    
    if (num_threads != 1)
        for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
    
    pthread_barrier_destroy(&barrier_threshold);
        
    int raw_count = get_raw_count(thread_data);
    int count = (raw_count * 4) - (ceiling * 4) + 1;
    
    std::cout << "Raw count: " << raw_count << ", Final count: " << count << std::endl;
    std::cout << "John can safely access " << count << " points on the map, given that he starts at (0,0).\n";
    
    return 0;
}