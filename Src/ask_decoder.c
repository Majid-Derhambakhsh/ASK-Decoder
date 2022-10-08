/*
------------------------------------------------------------------------------
~ File   : ask_decoder.c
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

#include "ask_decoder.h"

/* ............. Global variables ............ */
#ifdef PROGMEM
	
	const uint32_t Bit_Shift[32] PROGMEM =
	{
		0x00000001,0x00000002,0x00000004,0x00000008,
		0x00000010,0x00000020,0x00000040,0x00000080,
		0x00000100,0x00000200,0x00000400,0x00000800,
		0x00001000,0x00002000,0x00004000,0x00008000,
		0x00010000,0x00020000,0x00040000,0x00080000,
		0x00100000,0x00200000,0x00400000,0x00800000,
		0x01000000,0x02000000,0x04000000,0x08000000,
		0x10000000,0x20000000,0x40000000,0x80000000,
	};
	
#else
	
	#define pgm_read_dword(addr) (*(const uint32_t *)(addr))
	
	const uint32_t Bit_Shift[32] =
	{
		0x00000001,0x00000002,0x00000004,0x00000008,
		0x00000010,0x00000020,0x00000040,0x00000080,
		0x00000100,0x00000200,0x00000400,0x00000800,
		0x00001000,0x00002000,0x00004000,0x00008000,
		0x00010000,0x00020000,0x00040000,0x00080000,
		0x00100000,0x00200000,0x00400000,0x00800000,
		0x01000000,0x02000000,0x04000000,0x08000000,
		0x10000000,0x20000000,0x40000000,0x80000000,
	};

#endif // PROGMEM

/* ................ Functions ................ */
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
uint8_t	ASK_IsValidRemote(ASK_TypeDef *ASK)
{
	return (ASK->SignalData.ReviseFlag == 1);
}

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
uint32_t ASK_GetRemoteCode(ASK_TypeDef *ASK)
{

	ASK->SignalData.ReviseFlag = 0;
	
	return ASK->ReceivedCode;
	
}

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
void ASK_GetRemoteInfo(ASK_TypeDef *ASK, uint32_t *RemoteID, uint8_t *PressedKey)
{

	ASK->SignalData.ReviseFlag = 0;
	
	*RemoteID   = (ASK->ReceivedCode >> 4) & 0xFFFFF;
	*PressedKey = ASK->ReceivedCode & 0xF;
	
}

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
void ASK_ISRHandle_Rising(ASK_TypeDef *ASK)
{
	
	/* ----- Store falling time of signal ----- */
	ASK->SignalData.RisingTimeStart = _ASKTimer_GetTick();
	
	if (ASK->SignalData.RisingTimeStart <= ASK->SignalData.FallingTimeStart)
	{
		ASK->SignalData.FallingTime = (_ASK_TIMER_MAX_TICK - ASK->SignalData.FallingTimeStart) + ASK->SignalData.RisingTimeStart;
	}
	else
	{
		ASK->SignalData.FallingTime = ASK->SignalData.RisingTimeStart - ASK->SignalData.FallingTimeStart;
	}
		
	/* -------------- Check pulse ------------- */
	if(_ASK_SIGNAL_IS_SYNC_PULSE(ASK->SignalData.RisingTime, ASK->SignalData.FallingTime)) /* Start Sync*/
	{
		
		ASK->SignalData.SyncStarted  = 1;
		ASK->SignalData.BitIndex     = 0;
		ASK->SignalData.ReceivedData = 0;
		
	}
	else
	{
		/* Start Received */
		if(ASK->SignalData.SyncStarted == 1) 
		{
			
			if(ASK->SignalData.BitIndex < 23)
			{
				
				if(_ASK_SIGNAL_BIT_IS_ZERO(ASK->SignalData.RisingTime, ASK->SignalData.FallingTime))
				{
					ASK->SignalData.BitIndex++;
				}
				else if(_ASK_SIGNAL_BIT_IS_ONE(ASK->SignalData.RisingTime, ASK->SignalData.FallingTime))
				{
					
					ASK->SignalData.ReceivedData |= pgm_read_dword(&Bit_Shift[(23 - ASK->SignalData.BitIndex)]);
					ASK->SignalData.BitIndex++;
					
				}
				else
				{
					
					ASK->SignalData.SyncStarted = 0;
					ASK->SignalData.BitIndex    = 0;
					
				}

			}
			else
			{
				
				if(_ASK_SIGNAL_BIT_IS_ZERO(ASK->SignalData.RisingTime, ASK->SignalData.FallingTime))
				{
					ASK->SignalData.BitIndex++;
				}
				else if(_ASK_SIGNAL_BIT_IS_ONE(ASK->SignalData.RisingTime, ASK->SignalData.FallingTime))
				{
					ASK->SignalData.ReceivedData |= pgm_read_dword(&Bit_Shift[(23 - ASK->SignalData.BitIndex)]);
					ASK->SignalData.BitIndex++;
				}
				else
				{
					
					ASK->SignalData.SyncStarted = 0;
					ASK->SignalData.BitIndex    = 0;
					
				}

				if(ASK->SignalData.SyncStarted)
				{
					
					/* Data bits received */
					ASK->SignalData.BitIndex    = 0;
					ASK->SignalData.SyncStarted = 0;
					ASK->ReceivedCode        = ASK->SignalData.ReceivedData;
					ASK->SignalData.ReviseFlag  = 1;
					
				}
				
			}
			
		} 
		/* End of start received */
	}
	
	/* ------------ Start sampling ------------ */
	//ASK->Control.RisingTimeStart = _Timer_GetTick();
	
}

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
void ASK_ISRHandle_Falling(ASK_TypeDef *ASK)
{
	
	/* ----- Store rising time of signal ----- */
	ASK->SignalData.FallingTimeStart = _ASKTimer_GetTick();
	
	if (ASK->SignalData.FallingTimeStart <= ASK->SignalData.RisingTimeStart)
	{
		ASK->SignalData.RisingTime = (_ASK_TIMER_MAX_TICK - ASK->SignalData.RisingTimeStart) + ASK->SignalData.FallingTimeStart;
	}
	else
	{
		ASK->SignalData.RisingTime = ASK->SignalData.FallingTimeStart - ASK->SignalData.RisingTimeStart;
	}
	
	/* ----------- Start sampling ------------ */
	//ASK->Control.FallingTimeStart = _Timer_GetTick();
	
}
