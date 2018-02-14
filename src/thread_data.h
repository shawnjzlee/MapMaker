#ifndef THREAD_DATA_H
#define THREAD_DATA_H

class ThreadData {
    public:
        virtual void thread_data_init(unsigned int, unsigned int) = 0;
        virtual short get_thread_id() const = 0;
        virtual void set_thread_id(int) = 0;
        
    protected:
        short thread_id;
        
};

#endif /* thread_data.h */