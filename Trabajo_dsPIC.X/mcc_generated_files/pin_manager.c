/**
  PIN MANAGER Generated Driver File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.c

  @Summary:
    This is the generated manager file for the PIC24 / dsPIC33 / PIC32MM MCUs device.  This manager
    configures the pins direction, initial state, analog setting.
    The peripheral pin select, PPS, configuration is also handled by this manager.

  @Description:
    This source file provides implementations for PIN MANAGER.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.171.1
        Device            :  dsPIC33CH512MP508
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.70
        MPLAB 	          :  MPLAB X v5.50
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/


/**
    Section: Includes
*/

#include <xc.h>
#include <stdio.h>
#include "pin_manager.h"

/**
 Section: File specific functions
*/
void (*TRIANGULAR_InterruptHandler)(void) = NULL;
void (*DUTY_InterruptHandler)(void) = NULL;
void (*SENOIDAL_InterruptHandler)(void) = NULL;

/**
 Section: Driver Interface Function Definitions
*/
void PIN_MANAGER_Initialize (void)
{
    /****************************************************************************
     * Setting the Output Latch SFR(s)
     ***************************************************************************/
    LATA = 0x0000;
    LATB = 0x0000;
    LATC = 0x0000;
    LATD = 0x0000;
    LATE = 0x0000;

    /****************************************************************************
     * Setting the GPIO Direction SFR(s)
     ***************************************************************************/
    TRISA = 0x001F;
    TRISB = 0xCBFD;
    TRISC = 0xFF77;
    TRISD = 0xFFE7;
    TRISE = 0xFFFC;

    /****************************************************************************
     * Setting the Weak Pull Up and Weak Pull Down SFR(s)
     ***************************************************************************/
    CNPDA = 0x0000;
    CNPDB = 0x0000;
    CNPDC = 0x0000;
    CNPDD = 0x0000;
    CNPDE = 0x0000;
    CNPUA = 0x0000;
    CNPUB = 0x0000;
    CNPUC = 0x0000;
    CNPUD = 0x0000;
    CNPUE = 0x0000;

    /****************************************************************************
     * Setting the Open Drain SFR(s)
     ***************************************************************************/
    ODCA = 0x0000;
    ODCB = 0x0000;
    ODCC = 0x0000;
    ODCD = 0x0000;
    ODCE = 0x0000;

    /****************************************************************************
     * Setting the Analog/Digital Configuration SFR(s)
     ***************************************************************************/
    ANSELA = 0x001F;
    ANSELB = 0x0384;
    ANSELC = 0x0047;
    ANSELD = 0x7C00;
    ANSELE = 0x0040;
    
    /****************************************************************************
     * Set the PPS
     ***************************************************************************/
    __builtin_write_RPCON(0x0000); // unlock PPS

    RPINR20bits.SDI1R = 0x0046;    //RD6->SPI1:SDI1
    RPOR5bits.RP42R = 0x0006;    //RB10->SPI1:SCK1
    RPOR9bits.RP51R = 0x0005;    //RC3->SPI1:SDO1

    __builtin_write_RPCON(0x0800); // lock PPS
    
    /****************************************************************************
     * Interrupt On Change: negative
     ***************************************************************************/
    CNEN1Ebits.CNEN1E7 = 1;    //Pin : RE7
    CNEN1Ebits.CNEN1E8 = 1;    //Pin : RE8
    CNEN1Ebits.CNEN1E9 = 1;    //Pin : RE9
    /****************************************************************************
     * Interrupt On Change: flag
     ***************************************************************************/
    CNFEbits.CNFE7 = 0;    //Pin : RE7
    CNFEbits.CNFE8 = 0;    //Pin : RE8
    CNFEbits.CNFE9 = 0;    //Pin : RE9
    /****************************************************************************
     * Interrupt On Change: config
     ***************************************************************************/
    CNCONEbits.CNSTYLE = 1;    //Config for PORTE
    CNCONEbits.ON = 1;    //Config for PORTE
    
    /* Initialize IOC Interrupt Handler*/
    TRIANGULAR_SetInterruptHandler(&TRIANGULAR_CallBack);
    DUTY_SetInterruptHandler(&DUTY_CallBack);
    SENOIDAL_SetInterruptHandler(&SENOIDAL_CallBack);
    
    /****************************************************************************
     * Interrupt On Change: Interrupt Enable
     ***************************************************************************/
    IFS4bits.CNEIF = 0; //Clear CNEI interrupt flag
    IEC4bits.CNEIE = 1; //Enable CNEI interrupt
}

void TRIANGULAR_SetInterruptHandler(void (* InterruptHandler)(void))
{ 
    IEC4bits.CNEIE = 0; //Disable CNEI interrupt
    TRIANGULAR_InterruptHandler = InterruptHandler; 
    IEC4bits.CNEIE = 1; //Enable CNEI interrupt
}

void TRIANGULAR_SetIOCInterruptHandler(void *handler)
{ 
    TRIANGULAR_SetInterruptHandler(handler);
}

void DUTY_SetInterruptHandler(void (* InterruptHandler)(void))
{ 
    IEC4bits.CNEIE = 0; //Disable CNEI interrupt
    DUTY_InterruptHandler = InterruptHandler; 
    IEC4bits.CNEIE = 1; //Enable CNEI interrupt
}

void DUTY_SetIOCInterruptHandler(void *handler)
{ 
    DUTY_SetInterruptHandler(handler);
}

void SENOIDAL_SetInterruptHandler(void (* InterruptHandler)(void))
{ 
    IEC4bits.CNEIE = 0; //Disable CNEI interrupt
    SENOIDAL_InterruptHandler = InterruptHandler; 
    IEC4bits.CNEIE = 1; //Enable CNEI interrupt
}

void SENOIDAL_SetIOCInterruptHandler(void *handler)
{ 
    SENOIDAL_SetInterruptHandler(handler);
}

/* Interrupt service routine for the CNEI interrupt. */
void __attribute__ (( interrupt, no_auto_psv )) _CNEInterrupt ( void )
{
    if(IFS4bits.CNEIF == 1)
    {
        if(CNFEbits.CNFE7 == 1)
        {
            if(TRIANGULAR_InterruptHandler) 
            { 
                TRIANGULAR_InterruptHandler(); 
            }
            
            CNFEbits.CNFE7 = 0;  //Clear flag for Pin - RE7

        }
        
        if(CNFEbits.CNFE9 == 1)
        {
            if(DUTY_InterruptHandler) 
            { 
                DUTY_InterruptHandler(); 
            }
            
            CNFEbits.CNFE9 = 0;  //Clear flag for Pin - RE9

        }
        
        if(CNFEbits.CNFE8 == 1)
        {
            if(SENOIDAL_InterruptHandler) 
            { 
                SENOIDAL_InterruptHandler(); 
            }
            
            CNFEbits.CNFE8 = 0;  //Clear flag for Pin - RE8

        }
        
        
        // Clear the flag
        IFS4bits.CNEIF = 0;
    }
}

