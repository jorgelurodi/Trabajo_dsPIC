/* 
 * File:   Func.h
 * Author: SEA - Aitor VA
 *
 * Created on March 13, 2023, 4:28 PM
 */

#ifndef FUNC_H
#define	FUNC_H

#define FCY (4000000)       // FCY = FOSC/2 (no PLL)
#include <libpic30.h>
#include "mcc_generated_files/pin_manager.h"

void ConfigIO(void);
void CambiaRGB(float);


#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* FUNC_H */

