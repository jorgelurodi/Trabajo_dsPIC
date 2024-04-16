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

int conversion,i=0,cuenta=0;
float Volt=0.0;

void DutyNumero(int cuenta) {
    switch(cuenta) {
        case 1:
            PG2DC += (uint16_t)(0.1 * (65536 - 1)); // Incrementa 0.1 del rango del registro
            break;
        case 2:
            PG2DC += (uint16_t)(0.2 * (65536 - 1)); // Incrementa 0.2 del rango del registro
            break;
        case 3:
            PG2DC += (uint16_t)(0.3 * (65536 - 1)); // Incrementa 0.3 del rango del registro
            break;
        case 4:
            PG2DC += (uint16_t)(0.4 * (65536 - 1)); // Incrementa 0.4 del rango del registro
            break;
        case 5:
            PG2DC = (uint16_t)(0.5 * (65536 - 1)); // Reinicia a 0.5 del rango del registro
            break;
        default:
            // Manejo de otros casos
            break;
    }
}

void __attribute__ ((weak)) DUTY_CallBack(void)
{
    LED1_Toggle();
    cuenta++;
    DutyNumero(cuenta);
    IFS4bits.CNEIF = 0;
    CNFEbits.CNFE8 = 0;
    // PG2DC = valor asignado en DutyNumero(); // Asigna el valor calculado en DutyNumero()
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