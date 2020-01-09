#ifndef TABLE_VAR_H
#define TABLE_VAR_H

#include <Arduino.h>
#include "log.h"

class table_variables {
    private:

    public:
        table_variables();
        ~table_variables();
        String Version();
        uint32_t Read(uint8_t address);
        void Write(uint8_t address, uint32_t value);
};


#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_TABLE_VARIABLES)
extern table_variables TableVar;
#endif

#endif

