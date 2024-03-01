/*
 * Log.c
 *
 *  Created on: Mar 2, 2024
 *      Author: Lenovo
 */

#include "LIB/Log.h"

#include <stdio.h>
#include <stdarg.h>

#include <time.h>


typedef struct
{FILE *filePtr;
 Level_t Severity;
}Log_t;
Log_t File_Severity;
void Log_ConfigFile(const char * logFileName)
{


    // Open the file in write mode, clearing its content if it exists or creating a new file if it doesn't exist
    File_Severity.filePtr = fopen(logFileName, "w");

  /*  if (filePtr == NULL) {
        printf("Unable to create the file.\n");

    }

    printf("File created successfully.\n");

   */

}
void Log_SetSeverity(Level_t severity)
{
  File_Severity.Severity=severity;
}

void Log_Write(Level_t severity ,const char * format, ... )
{
/*
        if critical only output the critical logs
*           if error , log out the critical and error masseges
*           if warrnings , log out warnings , errors, critical
*           if  Info    , info and warrnings ,errors and critical
*/
if (File_Severity.filePtr!= NULL)
{

// if the configured sevirity >= the entered severity it will be printed in file only
if (severity >= File_Severity.Severity )
{
     //input arguments
    va_list args;
    va_start(args, format);

    // for time stamp
    time_t t ;
    time(&t);

    char *levelseverity;
    switch (severity)
    {
    case Log_Info: levelseverity= "INFO";
                    break;
    case Log_Warning:levelseverity= "WARNING";
                     break;
    case Log_Error:levelseverity= "ERROR";
                    break;
    case Log_Critical:levelseverity= "CRITICAL";
                      break;

    default:
        break;
    }




    fprintf(File_Severity.filePtr, "[%s] [%s] ", ctime(&t), levelseverity);
    vfprintf(File_Severity.filePtr, format, args);
    fprintf(File_Severity.filePtr, "\n");


    va_end(args);
}
else
{

}
}

}



void Log_Close ()
{
// Close the file
 if (File_Severity.filePtr!=NULL){   fclose(File_Severity.filePtr);}
}



