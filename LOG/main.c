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

int main()
{

    Log_ConfigFile("log.txt");
    Log_SetSeverity(Log_Critical);
    Log_Write(Log_Error,"error message " );
    Log_Write(Log_Info,"info message %d",1 );
    Log_Write(Log_Critical,"critical message %d",1 );
    Log_Write(Log_Error,"error message %d",2 );
 
    Log_Close ();

    return 0;
}



typedef struct 
{FILE *filePtr;
 Level_t Severity;
}Log_t;
Log_t File_Severity;
void Log_ConfigFile(const char * logFileName)
{


    // Open the file in write mode, clearing its content if it exists or creating a new file if it doesn't exist
    File_Severity.filePtr = fopen(logFileName, "w");

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
    char current_time[25];
    strncpy(current_time, ctime(&t), 24); // Copy the first 24 characters
    current_time[24] = '\0'; // Null-terminate the string
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

    

       
    fprintf(File_Severity.filePtr, "[%s] [%s] ", current_time, levelseverity);
    vfprintf(File_Severity.filePtr, format, args);
    fprintf(File_Severity.filePtr, "\n");
    

    va_end(args);
}
else
{}
}

}
          


void Log_Close ()
{
// Close the file
 if (File_Severity.filePtr!=NULL){   fclose(File_Severity.filePtr);}
}