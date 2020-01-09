#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>

class Timer {
    private:
        unsigned long timer     = 0;
        unsigned long timer_us  = 0;
    public:
        Timer();
        ~Timer();
        void set(unsigned long time_elapsed);
        void set_us(unsigned long time_elapsed);
        bool elapsed();
        bool elapsed_us();

};


#endif 