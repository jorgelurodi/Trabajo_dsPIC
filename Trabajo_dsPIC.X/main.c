
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/LCDMiniDrivers/lcd.h"
#include "mcc_generated_files/adc1.h"
#include "Func.h"

#define ROW_LEN 16

uint8_t row0_disp[ROW_LEN];    
uint8_t row1_disp[ROW_LEN];

uint8_t* Prow0 = &row0_disp;
uint8_t* Prow1 = &row1_disp;

char fila0[ROW_LEN];
char fila1[ROW_LEN];

int conversion,i=0,cuenta;
float Volt=0.0;

void __attribute__ ((weak)) TMR1_CallBack(void)
{
    LED1_Toggle();
}

void DutyNumero(int cuenta){
    if(cuenta==1){
        //incremento 0.1
    }
    else if(cuenta==2)
        //incremento
}

void __attribute__ ((weak)) DUTY_CallBack(void)
{
    cuenta++;
    DutyNumero(cuenta);
    PG2DC = 0x1534;

}

int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    
    Prow0 = (uint8_t*) &fila0;
    Prow1 = (uint8_t*) &fila1;
    
    lcd_setContrast(0x20);
    lcd_clearDisplay(); // BORRA LCD
  
  
    while (1)
    {
        
    lcd_writeString(Prow0, 0);
   
        
    }
    return 1; 
    

}


