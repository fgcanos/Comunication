#include <Arduino.h>

#include "timer.h"
#include "protocolo_serial.h"

// *****************************************************************************
// Defines
// *****************************************************************************
#define LED_ALIVE                       2
#define TIME_ELAPSED_ALIVE_MS           1000

// *****************************************************************************
// Global Variables 
// *****************************************************************************
// led alive
Timer timer_alive;
static bool led_alive_status;

Log LogMain;

// *****************************************************************************
// Local Functions 
// *****************************************************************************

// *****************************************************************************
// Setup
// *****************************************************************************
void setup() 
{
    Serial.begin(115200);
    Serial.println("");

    pinMode(LED_ALIVE,OUTPUT);  
    led_alive_status = LOW;
    timer_alive.set(TIME_ELAPSED_ALIVE_MS);

    LogMain.Init("[main]",LOG_OUTPUT_TYPE_SERIAL);

    ProtocoloSerial.Init();
    ProtocoloSerial.SetOutLog(LOG_OUTPUT_TYPE_SERIAL);
    LogMain.Info(ProtocoloSerial.Version());

    delay(200);
}

// *****************************************************************************
// loop ppl
// *****************************************************************************
void loop()
{
    if( true == timer_alive.elapsed() )
    {
        timer_alive.set(TIME_ELAPSED_ALIVE_MS);
        digitalWrite(LED_ALIVE, led_alive_status);
        led_alive_status = !led_alive_status;        
    }

    ProtocoloSerial.Task();
}