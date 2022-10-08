/*
------------------------------------------------------------------------------
~ File   : ask_decoder_conf.h
~ Author : Base algorithm by Mohammad Mazarei, Edited by Majid Derhambakhsh
~ Version: V0.0.0
~ Created: 12/20/2021 09:00:00 AM
~ Brief  :
~ Support:
           E-Mail : Majid.Derhambakhsh@gmail.com (subject : Embedded Library Support)
           
           Github : https://github.com/Majid-Derhambakhsh
------------------------------------------------------------------------------
~ Description:    

~ Attention  :    

~ Changes    :    
                  
------------------------------------------------------------------------------
*/

#ifndef __ASK_DECODER_CONF_H_
#define __ASK_DECODER_CONF_H_

/* ~~~~~~~~~~~~~~~ Include files ~~~~~~~~~~~~~~~ */
/* PGMSPACE header */
//#include <avr/pgmspace.h>

/* Timer header */
//#include "timer.h"

/* HAL or MCU header */
//#include <avr/io.h>
//#include "stm32f1xx_hal.h"

/* ~~~~~~~~~~~~~~~~~~ Options ~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~ Timer ~~~~~~~~ */
#define _ASKTimer_GetTick()   Timer1_GetTick()
#define _ASK_TIMER_MAX_TICK   UINT16_MAX

/* ~~~ ASK sensitivity ~~~ */
#define _ASK_USE_LOW_SENSITIVITY // Comment this line if you need to use high sensitivity

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#endif /* __ASK_DECODER_CONF_H_ */
