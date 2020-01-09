#include "table_var.h"
#include "log.h"
#include <Preferences.h>

/***************************************************************************//**
 *  Defines
 ******************************************************************************/
#define TAG                             "[table_var]"
#define VERSION                         "v1.0"
#define DATE                            "06/1/2020"


/***************************************************************************//**
 *  Typedef
 ******************************************************************************/

/***************************************************************************//**
 *  Global Variables 
 ******************************************************************************/
#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_TABLE_VARIABLES)
table_variables TableVar;
#endif

Preferences Preference_Com;
static uint32_t table_var[256];

/***************************************************************************//**
 *  Local Functions 
 ******************************************************************************/

table_variables::table_variables()
{
}

table_variables::~table_variables()
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
String table_variables::Version()
{
    String ret = "Table Variables module: ";

    ret += VERSION;
    ret += "    ";
    ret += DATE;

    return(ret);
}

/***************************************************************************//**
 * @brief   Read()
 * 
 * @details 
 *
 * @param   none
 * @return  none
 ******************************************************************************/
uint32_t table_variables::Read(uint8_t address)
{
    if( address < 256 )
    {
        return( table_var[address] );
    }
    else
    {
        return(0);
    }
}

/***************************************************************************//**
 * @brief   Write()
 * 
 * @details 
 *
 * @param   none
 * @return  none
 ******************************************************************************/
void table_variables::Write(uint8_t address, uint32_t value)
{
    // Preference_Com.putUInt(table_var[address],value);
    table_var[address] = value;
}

