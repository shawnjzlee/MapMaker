#ifndef MAPMAKER_H
#define MAPMAKER_H

#include "thread_data.h"

class MapMaker : public ThreadData {
    public:
        MapMaker();
        ~MapMaker();
        
        void thread_data_init(unsigned int, unsigned int);
        short get_thread_id() const;
        void set_thread_id(int);
        
        unsigned int get_lbound() const;
        unsigned int get_rbound() const;
        void set_bounds(unsigned int, unsigned int);
        
        unsigned int get_local_count() const;
        unsigned int get_ceiling() const;
        unsigned int get_constraint() const;
        void set_contraint(unsigned int);
        
        void get_valid_points();
        bool digit_sum_within_constraints(unsigned int, unsigned int);
        unsigned int get_ceiling();
        
        
    private:
        unsigned int lbound;                    /* Stores local left bound */
        unsigned int rbound;                    /* Stores local right bound */
        
        unsigned int local_count;               /* Number of valid points within bounds */
        unsigned int ceiling;                   /* Furthest reachable location */
        unsigned int constraint;                /* Sum of digits constraint */
        
        bool local_update;                      /* Checks if valid_cells are updated */
        
        char buff[100];                         /* Buffer to reduce chance of thrashing */
};

#endif /* mapmaker.h */