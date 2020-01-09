#include "log.h"


/***************************************************************************//**
 *  Defines
 ******************************************************************************/
#define VERSION                         "v1.0"
#define DATE                            "30/10/2019"

/***************************************************************************//**
 *  Typedef
 ******************************************************************************/

/***************************************************************************//**
 *  Global Variables 
 ******************************************************************************/


/***************************************************************************//**
 *  Local Functions Prototypes
 ******************************************************************************/


Log::Log()
{
}

Log::~Log()
{
}

/***************************************************************************//**
 * @brief   Version()
 * 
 * @details This function return the Log module version. 
 *
 * @param   none
 * @return  String
 ******************************************************************************/
String Log::Version()
{
    String ret = "Log module: ";

    ret += VERSION;
    ret += "    ";
    ret += DATE;

    return(ret);
}


/***************************************************************************//**
 * @brief   Init()
 * 
 * @details This function initialized the Log module. 
 *          Must be call in setup().
 *
 * @param   String TagName   
 *          OutPutType_T type       
 * @return  none
 ******************************************************************************/
void Log::Init(String TagName, OutPutType_T type)
{
    tag = TagName;
    outType = type;
}

/***************************************************************************//**
 * @brief   SetOutLog()
 * 
 * @details This function set the output log
 *
 * @param   OutPut_T outType
 * @return  none
 ******************************************************************************/
void Log::SetOutLog(OutPutType_T type)
{
    outType = type;
}


/***************************************************************************//**
 * @brief   EnableVerbose()
 * 
 * @details This function enable log verbose
 *
 * @param   bool enable
 * @return  none
 ******************************************************************************/
void Log::EnableVerbose(bool enable)
{
    flagEnableVerbose = enable;
}

/***************************************************************************//**
 * @details log function
 ******************************************************************************/
#undef X
#define X(a)                                    \
void Log::log_##a(a n)                          \
{                                               \
    switch(outType)                             \
    {                                           \
        case LOG_OUTPUT_TYPE_DISABLE:           \
            break;                              \
        case LOG_OUTPUT_TYPE_SERIAL:            \
            Serial.print(tag);                  \
            Serial.println(n);                  \
            break;                              \
        case LOG_OUTPUT_TYPE_SD:                \
            break;                              \
        case LOG_OUTPUT_TYPE_WEB:               \
            break;                              \
    }                                           \
}
    LOG_VARIABLES


void Log::log_uint64_t(uint64_t n)
{
    switch(outType)
    {
        case LOG_OUTPUT_TYPE_DISABLE:
            break;
        case LOG_OUTPUT_TYPE_SERIAL:
            Serial.print(tag);
            Serial.println((unsigned long)n);
            break;
        case LOG_OUTPUT_TYPE_SD:
            break;
        case LOG_OUTPUT_TYPE_WEB:
            break;
    }
}

void Log::log_Str(const char str[])
{
    switch(outType)
    {
        case LOG_OUTPUT_TYPE_DISABLE:
            break;
        case LOG_OUTPUT_TYPE_SERIAL:
            Serial.print(tag);
            Serial.println(str);
            break;
        case LOG_OUTPUT_TYPE_SD:
            break;
        case LOG_OUTPUT_TYPE_WEB:
            break;
    }
}

void Log::log_Str_uint64_t(const char str[], uint64_t n)
{
    switch(outType)
    {
        case LOG_OUTPUT_TYPE_DISABLE:
            break;
        case LOG_OUTPUT_TYPE_SERIAL:
            Serial.print(tag);
            Serial.print(str);
            Serial.println((unsigned long)n);
            break;
        case LOG_OUTPUT_TYPE_SD:
            break;
        case LOG_OUTPUT_TYPE_WEB:
            break;
    }
}

#undef X
#define X(a)                                    \
void Log::log_Str_##a(const char str[],a n)     \
{                                               \
    switch(outType)                             \
    {                                           \
        case LOG_OUTPUT_TYPE_DISABLE:           \
            break;                              \
        case LOG_OUTPUT_TYPE_SERIAL:            \
            Serial.print(tag);                  \
            Serial.print(str);                  \
            Serial.println(n);                  \
            break;                              \
        case LOG_OUTPUT_TYPE_SD:                \
            break;                              \
        case LOG_OUTPUT_TYPE_WEB:               \
            break;                              \
    }                                           \
}
    LOG_VARIABLES


/***************************************************************************//**
 * @details log verbose function
 ******************************************************************************/
#undef X
#define X(a)                                    \
void Log::Verbose(a n)                          \
{                                               \
    if( true == flagEnableVerbose )             \
    {                                           \
        log_##a(n);                             \
    }                                           \
}
    LOG_VARIABLES


void Log::Verbose(uint64_t n)
{
    if( true == flagEnableVerbose )
    {
        log_uint64_t(n);
    }
}

void Log::Verbose(const char str[])
{
    if( true == flagEnableVerbose )
    {
        log_Str(str);
    }
}

void Log::Verbose(const char str[], uint64_t n)
{
    if( true == flagEnableVerbose )
    {
        log_Str_uint64_t(str, n);
    }
}

#undef X
#define X(a)                                    \
void Log::Verbose(const char str[],a n)         \
{                                               \
    if( true == flagEnableVerbose )             \
    {                                           \
        log_Str_##a(str,n);                     \
    }                                           \
}
    LOG_VARIABLES

/***************************************************************************//**
 * @details log info function
 ******************************************************************************/
#undef X
#define X(a)                                    \
void Log::Info(a n)                             \
{                                               \
    log_##a(n);                                 \
}
    LOG_VARIABLES


void Log::Info(uint64_t n)
{
    log_uint64_t(n);
}

void Log::Info(const char str[])
{
    log_Str(str);
}

void Log::Info(const char str[], uint64_t n)
{
    log_Str_uint64_t(str,n);
}

#undef X
#define X(a)                                    \
void Log::Info(const char str[],a n)            \
{                                               \
    log_Str_##a(str, n);                        \
}
    LOG_VARIABLES























#if(0)
size_t Print::print(const String &s)
{
    return write(s.c_str(), s.length());
}

size_t Print::print(const char str[])
{
    return write(str);
}

size_t Print::print(char c)
{
    return write(c);
}

size_t Print::print(unsigned char b, int base)
{
    return print((unsigned long) b, base);
}

size_t Print::print(int n, int base)
{
    return print((long) n, base);
}

size_t Print::print(unsigned int n, int base)
{
    return print((unsigned long) n, base);
}

size_t Print::print(long n, int base)
{
    if(base == 0) {
        return write(n);
    } else if(base == 10) {
        if(n < 0) {
            int t = print('-');
            n = -n;
            return printNumber(n, 10) + t;
        }
        return printNumber(n, 10);
    } else {
        return printNumber(n, base);
    }
}

size_t Print::print(unsigned long n, int base)
{
    if(base == 0) {
        return write(n);
    } else {
        return printNumber(n, base);
    }
}

size_t Print::print(double n, int digits)
{
    return printFloat(n, digits);
}

#endif