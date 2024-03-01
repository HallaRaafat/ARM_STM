/*
 * Log.h
 *
 *  Created on: Mar 2, 2024
 *      Author: Lenovo
 */

#ifndef LOG_H_
#define LOG_H_




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


#endif /* LOG_H_ */
