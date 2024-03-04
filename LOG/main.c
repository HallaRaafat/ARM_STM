#include "LOG.h"



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

