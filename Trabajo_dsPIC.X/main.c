#include "mcc_generated_files/system.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/LCDMiniDrivers/lcd.h"
#include "mcc_generated_files/adc1.h"
//#include "Func.h"
//#include "dsp.h"
#include "math.h"
#define ROW_LEN 16
#define  NUM_SAMPLES 256

//uint16_t freca,frecb,frecc,fs;

int conversion_senoidal,conversion_triangular;
float volt_senoidal,volt_triangular=0.0;

uint16_t sample_count_senoidal, sample_count_triangular = 0; // Contador para el índice de la muestra a almacenar

/*
float sinA[NUM_SAMPLES];
float sinB[NUM_SAMPLES];
float sinC[NUM_SAMPLES];
*/
float samples_triangular[NUM_SAMPLES];
float samples_senoidal[NUM_SAMPLES];


float valor_medio_triangular,valor_medio_senoidal,valor_pico_triangular,valor_pico_senoidal,valor_rms_triangular,valor_rms_senoidal;

//fractional entrada[NUM_SAMPLES]; //Buffer entrada
//fractional filtrada[NUM_SAMPLES]; //Buffer salida

/*
void generaonda(void)
{
    uint16_t n = 0;
 
    fs = 10000;    // Frecuencia de Muestreo de 10 KHz
    freca   = 	720;    // Senoide de 720 Hz 
    frecb	=	367;    // Senoide de 367 Hz
    frecc 	= 	123;    // Senoide de 123 Hz
    for (n=0; n < NUM_SAMPLES; n++)
    {
      sinA[n] = sin((2*PI*freca*n)/fs);            // Creamos una senoide por puntos de la frecuencia A
      sinB[n] = sin((2*PI*frecb*n)/fs);            // Creamos una senoide por puntos de la frecuencia B
      sinC[n] = sin((2*PI*frecc*n)/fs);            // Creamos una senoide por puntos de la frecuencia C
      entrada[n] = ((sinA[n]+sinB[n]+sinC[n])/3)*0x8000;  // Escalamos el flotante a fraccional
      
    }
    return;
}   
*/

uint8_t row0_disp[ROW_LEN];    
uint8_t row1_disp[ROW_LEN];

uint8_t* Prow0 = &row0_disp;
uint8_t* Prow1 = &row1_disp;

char fila0[ROW_LEN];
char fila1[ROW_LEN];


/*
void filtrar (void){
    extern FIRStruct FPBFilter; //Filtro creado en archivo .s
    FIRDelayInit (&FPBFilter); //Inicializamos el Filtro
    FIR(NUM_SAMPLES, &filtrada[0], &entrada[0], &FPBFilter);
return;
}
*/
int cuenta=0;

float Valor_RMS(float* samples, int length) {
    if (length <= 0) {
        return 0.0; // En caso de longitud no válida, retornar 0.0
    }

    float sum_of_squares = 0.0;
    for (int i = 0; i < length; i++) {
        sum_of_squares += samples[i] * samples[i]; // Sumar el cuadrado de cada muestra
    }

    float mean_square = sum_of_squares / length; // Calcular el promedio del cuadrado de las muestras
    float rms_value = sqrt(mean_square); // Calcular la raíz cuadrada del promedio

    return rms_value; // Retornar el Valor RMS
}

float Valor_Medio(float* samples, int length) {
    if (length <= 0) {
        return 0.0; // En caso de longitud no válida, retornar 0.0
    }

    float sum = 0.0;
    for (int i = 0; i < length; i++) {
        sum += samples[i]; // Sumar todos los elementos del vector
    }

    return sum / length; // Calcular el promedio dividiendo la suma por la longitud
}

float Valor_Pico(float* samples, int length) {
    if (length <= 0) {
        return 0.0; // En caso de longitud no válida, retornar 0.0
    }

    float max_value = samples[0]; // Suponer que el primer elemento es el máximo inicialmente
    for (int i = 1; i < length; i++) {
        if (fabs(samples[i]) > fabs(max_value)) {
            max_value = samples[i]; // Actualizar el máximo si encontramos un valor mayor
        }
    }

    return max_value; // Retornar el valor máximo absoluto
}

void __attribute__ ((weak)) TMR1_CallBack(void)//500ms 
{
    
    //rutina de interrupción del tmr cada 500ms 
    valor_medio_triangular = Valor_Medio(samples_triangular, NUM_SAMPLES);
    valor_medio_senoidal = Valor_Medio(samples_senoidal, NUM_SAMPLES);
 
    valor_pico_triangular = Valor_Pico(samples_triangular, NUM_SAMPLES);
    valor_pico_senoidal = Valor_Pico(samples_senoidal, NUM_SAMPLES);

    valor_rms_triangular = Valor_RMS(samples_triangular, NUM_SAMPLES);
    valor_rms_senoidal = Valor_RMS(samples_senoidal, NUM_SAMPLES);
    
    

}


void __attribute__ ((weak)) SCCP1_COMPARE_CallBack(void)
{
    // Add your custom callback code here
    /* El SCCP1 se configura como:
     * Modo 32 bits
     * PRA = 0x0018 (100us)
     * Single-edge, toggle (el toggle no tiene efecto, valdría cualquiera).
     * Output: special-event trigger (para que el conversor AD se lance).
     * Interrupciones habilitadas    
     */
    conversion_senoidal = ADC1_ConversionResultGet(senoidal);// Se lee el resultados del CAD
    volt_senoidal = conversion_senoidal*(3.3/4096);                   // Se convierte a voltios
    
    // Almacenar la muestra de la señal senoidal en el vector correspondiente
    if (sample_count_senoidal < NUM_SAMPLES) {
        samples_senoidal[sample_count_senoidal] = volt_senoidal;
        sample_count_senoidal++; // Incrementar el contador de muestras
    }
    
    // Las siguientes líneas resetean el CCP para una nueva comparación
    CCP1CON1Lbits.CCPON = 0x0;
    CCP1TMRL = 0x0000;
    CCP1TMRH = 0x0000;
    CCP1STATLbits.ASEVT = 0;
    CCP1CON1Lbits.CCPON = 0x1;
    
    conversion_triangular = ADC1_ConversionResultGet(triangular);// Se lee el resultados del CAD
    volt_triangular = conversion_triangular*(3.3/4096); // Se convierte a voltios
    
    // Almacenar la muestra de la señal triangular en el vector correspondiente
    if (sample_count_triangular < NUM_SAMPLES) {
        samples_triangular[sample_count_triangular] = volt_triangular;
        sample_count_triangular++; // Incrementar el contador de muestras
    }
    
    // Las siguientes líneas resetean el CCP para una nueva comparación
    CCP1CON1Lbits.CCPON = 0x0;
    CCP1TMRL = 0x0000;
    CCP1TMRH = 0x0000;
    CCP1STATLbits.ASEVT = 0;
    CCP1CON1Lbits.CCPON = 0x1;
}

void __attribute__ ((weak)) SENAL1_CallBack(void)//RE7 triangular
{
    sprintf(fila0,"M",valor_pico_triangular);

}

void __attribute__ ((weak)) SENAL2_CallBack(void)//RE8 senoidal
{

}

void __attribute__ ((weak)) DUTY_CallBack(void)
{
    LED1_Toggle(); // Toggle LED
    PG2CONLbits.ON=0;//deshabilitar para actualizar el ciclo de trabajao 
    cuenta++; // Incrementa cuenta
    switch(cuenta) {
        case 1:
            PG2DC = (uint16_t)(0.1*(PG2PER + 1)); // Incrementa 0.1 del rango del registro
            break;
        case 2:
            PG2DC = (uint16_t)(0.2*(PG2PER + 1)); // Incrementa 0.2 del rango del registro
            break;
        case 3:
            PG2DC = (uint16_t)(0.3*(PG2PER + 1)); // Incrementa 0.3 del rango del registro
            break;
        case 4:
            PG2DC = (uint16_t)(0.4*(PG2PER +1)); // Incrementa 0.4 del rango del registro
            break;
        case 5:
            PG2DC = (uint16_t)(0.5*(PG2PER +1)); // Incrementa 0.4 del rango del registro
            break;
        case 6:
            PG2DC = (uint16_t)(0.6*(PG2PER +1)); // Incrementa 0.4 del rango del registro
            break;
        case 7:
            PG2DC = (uint16_t)(0.7*(PG2PER +1)); // Incrementa 0.4 del rango del registro
            break;
        case 8:
            PG2DC = (uint16_t)(0.8*(PG2PER +1)); // Incrementa 0.4 del rango del registro
            break;
        case 9:
            PG2DC = (uint16_t)(0.9*(PG2PER +1)); // Incrementa 0.4 del rango del registro
            break;
        case 10:
           // Reinicia a 0.1 del rango del registro
            PG2DC = (uint16_t)(0.1*(PG2PER + 1));
            cuenta = 1; // Reinicia cuenta a 1
            break;
        default:
            // Manejo de otros casos
            break;
    }
    PG2CONLbits.ON=1;//volver a habilitar el módulo
}

int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    
    //generaonda();//genera en el buffer de entrada una mezcla de las señales de entrada
    //filtrar();
    
    Prow0 = (uint8_t*) &fila0;
    Prow1 = (uint8_t*) &fila1;
    
    lcd_setContrast(0x20);
    lcd_clearDisplay(); // BORRA LCD
    
     // Es necesario habilitar el CAD y seleccionar el canal
   ADC1_Enable();
   ADC1_ChannelSelect(triangular);
   ADC1_ChannelSelect(senoidal);
  
    while (1)
    {
        
   //  lcd_writeString(Prow0, 0);
   
        
    }
    return 1; 
    

}