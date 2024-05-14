#include <p33CH512MP508.h>
#include "Func.h"

void ConfigIO()
{
    TRISEbits.TRISE0 = 0;
    TRISEbits.TRISE1 = 0;
}


void CambiaRGB(float v)
{
    if (v <= 1)
    {
        BLUE_SetLow();              
        RED_SetHigh();             
        GREEN_SetLow();             
    }
    
    else if(v > 1 && v <= 2)
    {
        BLUE_SetHigh();              
        RED_SetLow();             
        GREEN_SetLow(); 
    }
    
    else
    {
        BLUE_SetLow();              
        RED_SetLow();             
        GREEN_SetHigh(); 
    }
    

    return;
}