#ifndef PROTOCOLO_SERIAL_H
#define PROTOCOLO_SERIAL_H

#include <Arduino.h>
#include "log.h"

class protocolo_serial {
    private:

    public:
        protocolo_serial();
        ~protocolo_serial();
        String Version();
        void SetOutLog(OutPutType_T type);
        void Init();
        void Task();
};


#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_PROTOCOLO_SERIAL)
extern protocolo_serial ProtocoloSerial;
#endif

#endif

