/*
------------------------------------------------------------------------------
~ File   : ask_decoder.h
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

#ifndef __ASK_DECODER_H_
#define __ASK_DECODER_H_

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Include ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include <stdint.h>

#include "ask_decoder_conf.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#ifdef _ASK_USE_LOW_SENSITIVITY

	/* Low Sensitivity */
	#define _ASK_SIGNAL_IS_SYNC_PULSE(T1,T2)  (T2 > (T1*28) && T2 < (T1*34))
	#define _ASK_SIGNAL_BIT_IS_ZERO(T1,T2)    (T2 > (T1*1)  && T2 < (T1*5))
	#define _ASK_SIGNAL_BIT_IS_ONE(T1,T2)     (T1 > (T2*1)  && T1 < (T2*5))

#else

	/* High Sensitivity */
	#define _ASK_SIGNAL_IS_SYNC_PULSE(T1,T2)  (T2 > (T1*29) && T2 < (T1*32))
	#define _ASK_SIGNAL_BIT_IS_ZERO(T1,T2)    (T2 > (T1*2)  && T2 < (T1*4))
	#define _ASK_SIGNAL_BIT_IS_ONE(T1,T2)     (T1 > (T2*2)  && T1 < (T2*4))

#endif // _ASK_USE_LOW_SENSITIVITY

/* ---------------------------- Public ---------------------------- */
/* ---------------------- Define by compiler ---------------------- */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
typedef struct /* ASK Type */
{
	
	struct
	{
		
		uint16_t FallingTimeStart;
		uint16_t RisingTimeStart;
		uint16_t FallingTime;
		uint16_t RisingTime;
		uint16_t SyncStarted;
		uint32_t ReceivedData;
		
		uint8_t	 BitIndex;
		volatile uint8_t ReviseFlag;
		
	}SignalData;
	
	volatile uint32_t ReceivedCode;
	
}ASK_TypeDef;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/*
 * Function: ASK_IsValidRemote
 * ---------------------------
 * Get remote detecting status
 *
 * Param  : 
 *         ASK: Pointer to ASK module type
 *         
 * Returns: 
 *         returns 0 on not detecting valid remote
 *         returns 1 on detecting valid remote
 *         
 * Example: 
 *         ASK_TypeDef RF433;
 *         
 *         if (ASK_IsValidRemote(&RF433))
 *         {...}
 *         
 */
uint8_t ASK_IsValidRemote(ASK_TypeDef *ASK);

/*
 * Function: ASK_GetRemoteCode
 * ---------------------------
 * Get detected remote code
 *
 * Param  : 
 *         ASK: Pointer to ASK module type
 *         
 * Returns: 
 *         returns received remote code: 20bit[RemoteID] + 4bit[Key]
 *         
 * Example: 
 *         uint32_t RemoteCode = 0;
 *         
 *         ASK_TypeDef RF433;
 *
 *         RemoteCode = ASK_GetRemoteCode(&RF433);
 *         ...
 *         
 */
uint32_t ASK_GetRemoteCode(ASK_TypeDef *ASK);

/*
 * Function: ASK_GetRemoteInfo
 * ---------------------------
 * Get detected remote ID and Key
 *
 * Param  : 
 *         ASK       : Pointer to ASK module type
 *         RemoteID  : Pointer to get Remote ID
 *         PressedKey: Pointer to get Key code
 *         
 * Returns: 
 *         -
 *         
 * Example: 
 *         uint8_t RemoteKey = 0;
 *         uint32_t RemoteID = 0;
 *
 *         ASK_TypeDef RF433;
 *
 *         ASK_GetRemoteInfo(&RF433, &RemoteID, &RemoteKey);
 *         ...
 *         
 */
void ASK_GetRemoteInfo(ASK_TypeDef *ASK, uint32_t *RemoteID, uint8_t *PressedKey);

/* ::::: ISR Handling ::::: */
/*
 * Function: ASK_ISRHandle_Rising
 * ------------------------------
 * Handling received signal on rising edge
 *
 * Param  : 
 *         ASK: Pointer to ASK module type
 *         
 * Returns: 
 *         -
 *         
 * Example: 
 *         ASK_TypeDef RF433;
 *
 *         void GPIO_ISR_RisingEdge()
 *         {
 *           ASK_ISRHandle_Rising(&RF433);
 *           ...
 *         }
 *         ...
 *         
 */
void ASK_ISRHandle_Rising(ASK_TypeDef *ASK);

/*
 * Function: ASK_ISRHandle_Falling
 * -------------------------------
 * Handling received signal on falling edge
 *
 * Param  : 
 *         ASK: Pointer to ASK module type
 *         
 * Returns: 
 *         -
 *         
 * Example: 
 *         ASK_TypeDef RF433;
 *
 *         void GPIO_ISR_FallingEdge()
 *         {
 *           ASK_ISRHandle_Falling(&RF433);
 *           ...
 *         }
 *         ...
 *         
 */
void ASK_ISRHandle_Falling(ASK_TypeDef *ASK);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#endif /* __ASK_DECODER_H_ */
