#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include <time.h>

typedef enum
{
Log_Info,
Log_Warning,
Log_Error,
Log_Critical
}Level_t;


// Enter the output file name 

void Log_ConfigFile(const char * logFileName);


void Log_Write(Level_t severity ,const char * format, ... );
/* @brief : if critical only output the critical logs 
*           if error , log out the critical and error masseges 
*           if warrnings , log out warnings , errors, critical
*           if  Info    , info and warrnings ,errors and critical   
*/            
void Log_SetSeverity(Level_t severity);
void Log_Close ();
