#ifndef LOG_H
#define LOG_H

#include <Arduino.h>


typedef enum OutPutType_Tag
{
    LOG_OUTPUT_TYPE_DISABLE,
    LOG_OUTPUT_TYPE_SERIAL,
    LOG_OUTPUT_TYPE_SD,
    LOG_OUTPUT_TYPE_WEB
} OutPutType_T;

#define LOG_VARIABLES             \
    X( bool                       )\
    X( int8_t                     )\
    X( uint8_t                    )\
    X( int16_t                    )\
    X( uint16_t                   )\
    X( int32_t                    )\
    X( uint32_t                   )\
    X( float                      )\
    X( String                     )\

class Log 
{
    private:
      String        tag = "[..]";
      OutPutType_T  outType = LOG_OUTPUT_TYPE_DISABLE;
      bool          flagEnableVerbose = false;

      #undef X
      #define X(a)                          \
      void log_##a(a n);
      LOG_VARIABLES

      void log_uint64_t(uint64_t n);
      void log_Str(const char str[]);
      void log_Str_uint64_t(const char str[], uint64_t n);

      #undef X
      #define X(a)                          \
      void log_Str_##a(const char str[],a n);
      LOG_VARIABLES

    public:
        Log();
        ~Log();
        String Version();
        void Init(String TagName, OutPutType_T outType);
        void SetOutLog(OutPutType_T outType);
        void EnableVerbose(bool enable);


        #undef X
        #define X(a)                          \
        void Verbose(a n);
        LOG_VARIABLES

        void Verbose(uint64_t n);
        void Verbose(const char str[]);
        void Verbose(const char str[], uint64_t n);

        #undef X
        #define X(a)                          \
        void Verbose(const char str[],a n);
        LOG_VARIABLES
        


        #undef X
        #define X(a)                          \
        void Info(a n);
        LOG_VARIABLES

        void Info(uint64_t n);
        void Info(const char str[]);
        void Info(const char str[], uint64_t n);

        #undef X
        #define X(a)                          \
        void Info(const char str[],a n);
        LOG_VARIABLES

};


#endif

