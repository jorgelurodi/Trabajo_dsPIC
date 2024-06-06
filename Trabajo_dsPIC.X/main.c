#include "mcc_generated_files/system.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/LCDMiniDrivers/lcd.h"
#include "mcc_generated_files/adc1.h"
#include "Func.h"
#include "dsp.h"
#include "math.h"
#include <stdio.h>

#define ROW_LEN 16
#define  NUM_SAMPLES 256

int16_t conversion_senoidal,conversion_triangular;
int cuenta,i=0;
float volt_senoidal,volt_triangular=0.0;

uint16_t sample_count_senoidal  = 0; 
uint16_t sample_count_triangular = 0; // Contador para el índice de la muestra a almacenar

//Señales de entrada a pasar por el filtro
float samples_triangular[NUM_SAMPLES];
float samples_senoidal[NUM_SAMPLES];


float valor_medio_triangular,valor_medio_senoidal,valor_pico_triangular,valor_pico_senoidal,valor_rms_triangular,valor_rms_senoidal;


fractional entrada_triangular[NUM_SAMPLES]; //Buffer entrada
fractional entrada_senoidal[NUM_SAMPLES]; //Buffer entrada
fractional filtrada_triangular[NUM_SAMPLES]; //Buffer salida
fractional filtrada_senoidal[NUM_SAMPLES]; //Buffer salida


uint8_t row0_disp[ROW_LEN];  
uint8_t row1_disp[ROW_LEN];

uint8_t* Prow0 = &row0_disp;
uint8_t* Prow1 = &row1_disp;

char fila0[ROW_LEN];
char fila1[ROW_LEN];

bool band_filtro=false;//por defecto comienza con filtro FIR

void InitLCD()
{
    lcd_setup(); // INICIALIZA LCD
    lcd_clearDisplay(); // BORRA LCD
}

void filtrar_FIR (void){
    extern FIRStruct pasobanda_HammingFilter; //Filtro creado en archivo .s
    FIRDelayInit (&pasobanda_HammingFilter); //Inicializamos el Filtro
    FIR(NUM_SAMPLES, &filtrada_triangular[0], &entrada_triangular[0], &pasobanda_HammingFilter);
    FIR(NUM_SAMPLES, &filtrada_senoidal[0], &entrada_senoidal[0], &pasobanda_HammingFilter);
    return;
}

void filtrar_IIR(void) {
    extern IIRTransposedStruct pasobanda_IIRFilter; // Filtro IIR creado en archivo .s
    IIRTransposedInit(&pasobanda_IIRFilter); // Inicializamos el filtro IIR
    // Aplicamos el filtro IIR a la señal triangular
    IIRTransposed(NUM_SAMPLES, &filtrada_triangular[0], &entrada_triangular[0], &pasobanda_IIRFilter);
    // Aplicamos el filtro IIR a la señal senoidal
    IIRTransposed(NUM_SAMPLES, &filtrada_senoidal[0], &entrada_senoidal[0], &pasobanda_IIRFilter);
    return;
}

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


void Imprime_triangular(void){
    
    InitLCD();
    
    lcd_setContrast(0x20);
    
    sprintf(fila0, "V1:M=%.2f;m:%.2f ",valor_pico_triangular,valor_medio_triangular);
    sprintf(fila1, "V1:rms=%.2fV",valor_rms_triangular);
         
    Prow0 = (uint8_t*) &fila0;
    lcd_writeString(Prow0, 0);
    
    Prow1 = (uint8_t*) &fila1;
    lcd_writeString(Prow1, 1);
    
}

void Imprime_senoidal(void){
    
    InitLCD();
    
    lcd_setContrast(0x20);
    
    sprintf(fila0, "V2:M=%.2f;m:%.2f ",valor_pico_senoidal,valor_medio_senoidal);
    sprintf(fila1, "V2:rms=%.2fV",valor_rms_senoidal);
         
    Prow0 = (uint8_t*) &fila0;
    lcd_writeString(Prow0, 0);
    
    Prow1 = (uint8_t*) &fila1;
    lcd_writeString(Prow1, 1);
    
}

void __attribute__ ((weak)) SCCP1_COMPARE_CallBack(void)
{
    // Configurar el CCP para disparar el ADC
    // Asegurar previamente que la configuración esté hecha en la inicialización del sistema
    /* El SCCP1 se configura como:
     * Modo 16 bits
     * PRA = 0x0000
     * PRB = 0x009F 
     * PRL = 0x013F (79.82us)
     * Double-edge,PWM
     * Output: special-event trigger (para que el conversor AD se lance).
     * Interrupciones habilitadas  
     */
    
    // Verificar y leer el resultado del ADC para la señal senoidal
    if (ADC1_IsConversionComplete(senoidal)){
        conversion_senoidal = ADC1_ConversionResultGet(senoidal);
        volt_senoidal = conversion_senoidal * (3.3 / 4096); // Convertir a voltios

        // Almacenar la muestra de la señal senoidal en el vector correspondiente
        if (sample_count_senoidal < NUM_SAMPLES) {
            samples_senoidal[sample_count_senoidal] = volt_senoidal;
            entrada_senoidal[sample_count_senoidal] = conversion_senoidal * 0x0100; // Escalar a fraccional
            sample_count_senoidal++; // Incrementar el contador de muestras
        }
    }
    
    valor_medio_senoidal = Valor_Medio(samples_senoidal, NUM_SAMPLES);
    valor_pico_senoidal = Valor_Pico(samples_senoidal, NUM_SAMPLES);
    valor_rms_senoidal = Valor_RMS(samples_senoidal, NUM_SAMPLES);
        
    // Verificar y leer el resultado del ADC para la señal triangular
    if (ADC1_IsConversionComplete(triangular)){
        conversion_triangular = ADC1_ConversionResultGet(triangular);
        volt_triangular = conversion_triangular * (3.3 / 4096); // Convertir a voltios

        // Almacenar la muestra de la señal triangular en el vector correspondiente
        if (sample_count_triangular < NUM_SAMPLES) {
            samples_triangular[sample_count_triangular] = volt_triangular;
            entrada_triangular[sample_count_triangular] = conversion_triangular * 0x0100; // Escalar a fraccional
            sample_count_triangular++; // Incrementar el contador de muestras
        }
    }
    
    valor_medio_triangular = Valor_Medio(samples_triangular, NUM_SAMPLES);
    valor_pico_triangular = Valor_Pico(samples_triangular, NUM_SAMPLES);
    valor_rms_triangular = Valor_RMS(samples_triangular, NUM_SAMPLES);
    
    if(!band_filtro){//pasa vector de muestras al filtro seleccionado
        filtrar_FIR();
    }
    else{
        filtrar_IIR();
    }
          
}
       
void __attribute__ ((weak)) TRIANGULAR_CallBack(void)//RE7 muestra triangular
{
    LED1_Toggle();
    
    if(band_filtro==true){    
        band_filtro=false;//cambia a filtro FIR
    }    
    
    
   Imprime_triangular();    
}

void __attribute__ ((weak)) SENOIDAL_CallBack(void){//RE8 muestra senoidal
    
    LED1_Toggle();
    
    if(band_filtro==false){
      //band_filtro=true;//cambia a filtro IIR
    }

    Imprime_senoidal();
}

void __attribute__ ((weak)) DUTY_CallBack(void)//RE9 cambia ciclo de trabajo de la PWM
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
    
    
     // Es necesario habilitar el CAD y seleccionar el canal
    ADC1_Initialize();
    ADC1_Enable();
    ADC1_ChannelSelect(triangular);
    ADC1_ChannelSelect(senoidal);
   
  
    while (1)
    {
 
    }
    return 1; 
    

}