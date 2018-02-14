#ifndef GLOBAL_H
#define GLOBAL_H

#include <vector>
#include <map>
#include <memory>
#include <mutex>
#include <pthread.h>

extern std::map<int, std::vector<std::mutex>> mutex_map;
extern std::vector<std::vector<short>> valid_cells;
extern pthread_barrier_t barrier_threshold;
extern std::mutex mutex_updated;
extern bool updated;

#endif /* global.h */