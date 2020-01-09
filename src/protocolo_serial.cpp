#include "protocolo_serial.h"
#include "table_var.h"
#include "log.h"
#include <HardwareSerial.h>
#include <BluetoothSerial.h>

/***************************************************************************//**
 *  Defines
 ******************************************************************************/
#define TAG                             "[protocolo_serial]"
#define VERSION                         "v1.0"
#define DATE                            "07/1/2020"


#define BAUDRATE_SERIAL                 56000
#define SERIAL_MODE                     SERIAL_8N1
#define PIN_RX_SERIAL                   16 // 23  //13
#define PIN_TX_SERIAL                   17 // 19  //14

#define LENGTH_BUFFER_RX                256

#define LENGTH_MESSAGE_DATA             9

#define uS_TO_S_FACTOR  1000000
#define TIME_TO_SLEEP 15 //time for sleep
#define wakeup_threshold 3 //time for sleep



/***************************************************************************//**
 *  Typedef
 ******************************************************************************/
enum CommandsList
{
    DISABLE_ESP32,
    ENABLE_ESP32,
    SLEEP_ESP32,
    DISABLE_WEBSERVER,
    ENABLE_WEBSERVER,
    DISABLE_BT,
    ENABLE_BT,
    READ_TABLE,
    WRITE_TABLE,
    NUM_COMMANDS
};

/***************************************************************************//**
 *  Global Variables 
 ******************************************************************************/
#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_PROTOCOLO_SERIAL)
protocolo_serial ProtocoloSerial;
#endif

static uint8_t buf_RX[LENGTH_BUFFER_RX];
static uint32_t i_readRx;
static uint32_t i_readBuf;
boolean Mode_BT = false, Mode_WS = false, restxmode = false;

HardwareSerial SerialProtocolo(2);
BluetoothSerial SerialBT;
Log LogProtocoloSerial;
/***************************************************************************//**
 *  Local Functions 
 ******************************************************************************/
static void ReadDataFromModuleViaUart();
static void ReadBufferReceived();
static void RunCommand( uint8_t command, uint8_t  address,
                        uint8_t  data_0, uint8_t  data_1, uint8_t  data_2,uint8_t  data_3 );
static void ResponseMsg( uint8_t command, uint8_t  address,
                            uint8_t  data_0, uint8_t  data_1, uint8_t  data_2,uint8_t  data_3 );


protocolo_serial::protocolo_serial()
{
}

protocolo_serial::~protocolo_serial()
{
}

/***************************************************************************//**
 * @brief   Version()
 * 
 * @details This function return the  version. 
 *
 * @param   none
 * @return  String
 ******************************************************************************/
String protocolo_serial::Version()
{
    String ret = "Protocolo Serial module: ";

    ret += VERSION;
    ret += "    ";
    ret += DATE;

    return(ret);
}

/***************************************************************************//**
 * @brief   Init()
 * 
 * @details Set the output type log
 *          Must be call in setup().
 *
 * @param   OutPutType_T type
 * @return  none
 ******************************************************************************/
void protocolo_serial:: SetOutLog(OutPutType_T type)
{
    LogProtocoloSerial.SetOutLog(type);
}

/***************************************************************************//**
 * @brief   Init()
 * 
 * @details This function initialized the gas module. Must be call in setup()
 *
 * @param   none
 * 
 * @return  none
 ******************************************************************************/
void protocolo_serial::Init()
{
    LogProtocoloSerial.Init("[protocolo_serial]", LOG_OUTPUT_TYPE_DISABLE);

    SerialProtocolo.begin(BAUDRATE_SERIAL, SERIAL_MODE, PIN_RX_SERIAL, PIN_TX_SERIAL);

    //////////// Wakeups sources
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_4,1);                     // wakeup por GPIO designado
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);  // wakeup por timer

    i_readRx    = 0;
    i_readBuf   = 0;
}

/***************************************************************************//**
 * @brief   Task()
 * 
 * @details This function is the task the gas module. Must be call in loop()
 *
 * @param   none
 * 
 * @return  none
 ******************************************************************************/
void protocolo_serial::Task()
{
    ReadDataFromModuleViaUart();
    ReadBufferReceived();
}

/***************************************************************************//**
 * @brief   ReadDataFromModuleViaUart()
 * 
 * @details This function read data from uart (serial) and load in local buffer.
 *
 * @param   none
 * @return  none
 ******************************************************************************/
static void ReadDataFromModuleViaUart()
{
    i_readRx    = 0;
    i_readBuf   = 0;

    if(Mode_BT == true)
    {
        if( SerialBT.available() > 0 )
        {
            while( SerialBT.available() != 0 )
            {
                buf_RX[i_readRx] = SerialBT.read();

                LogProtocoloSerial.Verbose(buf_RX[i_readRx]);

                if( i_readRx < (LENGTH_BUFFER_RX - 1) )
                {
                    i_readRx++;
                }
            }        
        }
    }
    else 
    {
        if( SerialProtocolo.available() > 0 )
        {
            while( SerialProtocolo.available() != 0 )
            {
                buf_RX[i_readRx] = SerialProtocolo.read();

                LogProtocoloSerial.Verbose(buf_RX[i_readRx]);

                if( i_readRx < (LENGTH_BUFFER_RX - 1) )
                {
                    i_readRx++;
                }
            }        
        }
    }
}

/***************************************************************************//**
 * @brief   ReadBufferReceived()
 * 
 * @details This function read data received from local buffer, convert and 
 *          load the data into the correspondet variables.
 *
 * @param   none
 * @return  none
 ******************************************************************************/
static void ReadBufferReceived()
{
    if( i_readBuf != i_readRx )
    {
        if( (i_readRx - i_readBuf) >= LENGTH_MESSAGE_DATA )
        {
            if( buf_RX[i_readBuf]        == 0x49 )
            if( buf_RX[i_readBuf + 1]    == 0x4D )
            if( buf_RX[i_readBuf + 8]    == 0x53 )
            {
                RunCommand( buf_RX[i_readBuf + 2], buf_RX[i_readBuf + 3], 
                            buf_RX[i_readBuf + 4], buf_RX[i_readBuf + 5], buf_RX[i_readBuf + 6], buf_RX[i_readBuf + 7] );
            }
            i_readBuf = i_readBuf + LENGTH_MESSAGE_DATA;
        }
    }
    else
    {   
        i_readRx    = 0;
        i_readBuf   = 0;
    }  
}
/***************************************************************************//**
 * @brief   EnableBT() / DisableBT()
 * 
 * @details 
 *
 * @param   none
 * @return  none
 ******************************************************************************/
static void EnableBT()
{
    if (Mode_BT != true){
        #if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
        #error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
        #endif
        SerialBT.begin("ESP32_IoT_Indra"); //Bluetooth device name
        Serial.println("The device BT started!");
        Mode_BT = true;
    }
}

static void DisableBT()
{
    Serial.println("The device BT stopped!");
    Mode_BT = false;
    delay(10);
    SerialBT.disconnect();
}

/***************************************************************************//**
 * @brief   RunCommand()
 * 
 * @details This function run command
 *
 * @param   uint8_t command
 * @param   uint8_t address
 * @param   uint8_t data_0
 * @param   uint8_t data_1
 * @param   uint8_t data_2
 * @param   uint8_t data_3
 * @return  none
 ******************************************************************************/
static void RunCommand( uint8_t command, uint8_t  address,
                        uint8_t  data_0, uint8_t  data_1, uint8_t  data_2,uint8_t  data_3 )
{
    uint8_t data_0_resp;
    uint8_t data_1_resp;
    uint8_t data_2_resp;
    uint8_t data_3_resp;

    uint32_t value;

    switch(command)
    {
        case DISABLE_ESP32:
            // DisableEsp32();
            ResponseMsg(command + 0x80, 0, 0, 0, 0, 0 );
            break;
        case ENABLE_ESP32:
            // EnableEsp32();
            ResponseMsg(command + 0x80, 0, 0, 0, 0, 0 );
            break;
        case SLEEP_ESP32:
            ResponseMsg(command + 0x80, 0, 0, 0, 0, 0 );
            esp_deep_sleep_start();  // wakeup for timer --> 15 seconds
            break;
        case DISABLE_WEBSERVER:
            // ServidorWebClose();
            ResponseMsg(command + 0x80, 0, 0, 0, 0, 0 );
            break;
        case ENABLE_WEBSERVER:
            ResponseMsg(command + 0x80, 0, 0, 0, 0, 0 );
            // EnableWebServer(address);
            break;
        case DISABLE_BT:
            ResponseMsg(command + 0x80, 0, 0, 0, 0, 0 );
            DisableBT();
            break;
        case ENABLE_BT:
            ResponseMsg(command + 0x80, 0, 0, 0, 0, 0 );
            EnableBT();
            break;
        case READ_TABLE:
            value = TableVar.Read(address);
            data_0_resp = (uint8_t) (value >> 24 & 0xFF);
            data_1_resp = (uint8_t) (value >> 16 & 0xFF);
            data_2_resp = (uint8_t) (value >> 8  & 0xFF);
            data_3_resp = (uint8_t) (value       & 0xFF);
            ResponseMsg(command + 0x80, address, data_0_resp, data_1_resp, data_2_resp, data_3_resp );
            break;
        case WRITE_TABLE:
            value = ((uint32_t)data_0 << 24 )|((uint32_t)data_1 << 16 )|((uint32_t)data_2 << 8 )|((uint32_t)data_3 );
            TableVar.Write(address, value);
            ResponseMsg(command + 0x80, address, data_0, data_1, data_2, data_3 );
            break;
        default:
            break;
    }

}

static void ResponseMsg(    uint8_t command, uint8_t  address,
                            uint8_t  data_0, uint8_t  data_1, uint8_t  data_2,uint8_t  data_3 )
{
    uint8_t buf_tx[LENGTH_MESSAGE_DATA];
    
    buf_tx[0] = 0x49;
    buf_tx[1] = 0x4D;
    buf_tx[2] = command;
    buf_tx[3] = address;
    buf_tx[4] = data_0;
    buf_tx[5] = data_1;
    buf_tx[6] = data_2;
    buf_tx[7] = data_3;
    buf_tx[8] = 0x45;

    if (Mode_BT == false && Mode_WS == false) SerialProtocolo.write(buf_tx, LENGTH_MESSAGE_DATA);
    else if (Mode_BT == true) {SerialBT.write(buf_tx, LENGTH_MESSAGE_DATA); SerialBT.write(0x0D);SerialBT.write(0x0A);}
}
