/*#####################################################################################################
  @file    
  @author  CS Team
  @brief   

  #####################################################################################################
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx.h"
#include "user_can.h"
#include "user_ccp.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define txRxDisable     0U
#define tx___Enable     1U
#define rx___Enable     2U

#define txAddress1     0x601UL
#define txAddress2     0x7e2UL
#define txAddress3     0x601UL

#define rxAddress1     0x581UL
#define rxAddress2     0x701UL
/* Private variables ---------------------------------------------------------*/
canConfigMatrixT canConfigG[5] = 
{
/*  address           index      enable/disable    dataSize  standard ID/extend ID   data buffer   callback function */
    {txAddress1,         0,          tx___Enable,      8,         CAN_ID_STD,          {0},        canTxMessageF1},
	  {txAddress2,         0,          txRxDisable,      8,         CAN_ID_STD,          {0},        canTxMessageF2},
		{txAddress3,         0,          tx___Enable,      8,         CAN_ID_STD,          {0},        canTxMessageF3},
	
		{rxAddress1,         0,          rx___Enable,      8,         CAN_ID_STD,          {0},        canRxMessageF1}, 
	  {rxAddress2,         0,          rx___Enable,      8,         CAN_ID_STD,          {0},        canRxMessageF2}
};
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/*##################################################################################################*/ 
/*****************************************************************************************************
  * @brief
  * @note
  * @param
  * @param
  * @retval
	***************************************************************************************************
  */
void canInitF(CAN_HandleTypeDef *hcan, canConfigMatrixT *config)
{
	uint8_t index = 0;
	uint32_t tickstart;
	CAN_TxHeaderTypeDef tem = {0};
	CAN_FilterTypeDef   filterConfig;
	
	do
	{
			if(index < 3)
			{
				tem.RTR = CAN_RTR_DATA;
				tem.IDE = config[index].IdType;
			
				/* Set up the Id */
				if (tem.IDE == CAN_ID_STD)
				{
					tem.StdId = config[index].mailboxId;
					hcan->Instance->sTxMailBox[index].TIR = ((tem.StdId << CAN_TI0R_STID_Pos) |
																														 tem.RTR);
				}
				else
				{
					tem.ExtId = config[index].mailboxId;
					hcan->Instance->sTxMailBox[index].TIR = ((tem.ExtId << CAN_TI0R_EXID_Pos) |
																														 tem.IDE |
																														 tem.RTR);
				}
				
				tem.DLC = config[index].dataSize;
				/* Set up the DLC */
				hcan->Instance->sTxMailBox[index].TDTR = (tem.DLC);
			}
			else
			{
				tem.RTR = CAN_RTR_DATA;
				tem.IDE = config[index].IdType;
				break;
			
			}
			
	}while(index++ < 5);
	
  /* Configure the CAN Filter */
  filterConfig.FilterBank = 0;
  filterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  filterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  filterConfig.FilterIdHigh = (((uint32_t)config[3].mailboxId<<21)&0xffff0000)>>16;;
  filterConfig.FilterIdLow =  (((uint32_t)config[3].mailboxId<<21)|CAN_ID_STD|CAN_RTR_DATA)&0xffff;
  filterConfig.FilterMaskIdHigh = 0xffff;
  filterConfig.FilterMaskIdLow = 0xffff;
  filterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
  filterConfig.FilterActivation = ENABLE;
  filterConfig.SlaveStartFilterBank = 14;
	HAL_CAN_ConfigFilter(hcan, &filterConfig);
	
	/* Configure the CAN Filter */
  filterConfig.FilterBank = 1;
  filterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  filterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  filterConfig.FilterIdHigh = (((uint32_t)config[4].mailboxId<<21)&0xffff0000)>>16;;
  filterConfig.FilterIdLow =  (((uint32_t)config[4].mailboxId<<21)|CAN_ID_STD|CAN_RTR_DATA)&0xffff;
  filterConfig.FilterMaskIdHigh = 0xffff;
  filterConfig.FilterMaskIdLow = 0xffff;
  filterConfig.FilterFIFOAssignment = CAN_RX_FIFO1;
  filterConfig.FilterActivation = ENABLE;
  filterConfig.SlaveStartFilterBank = 14;
	HAL_CAN_ConfigFilter(hcan, &filterConfig);
	
	HAL_CAN_Start(hcan);
	/* Request transmission */
	CAN->sTxMailBox[0].TIR |= CAN_TI0R_TXRQ;
	/* Request transmission */
	CAN->sTxMailBox[1].TIR |= CAN_TI1R_TXRQ;
	/* Request transmission */
	CAN->sTxMailBox[2].TIR |= CAN_TI2R_TXRQ;
	
	/* Get tick */
	tickstart = HAL_GetTick();	// TIM2->CNT
	/* Check for the Timeout */
	while((HAL_GetTick() - tickstart) < 10000UL);
	/* Release RX FIFO 0 */
	CAN->RF0R |= CAN_RF0R_RFOM0;
	/* Release RX FIFO 1 */
	CAN->RF1R |= CAN_RF1R_RFOM1;

}
/*****************************************************************************************************
  * @brief
  * @note
  * @param
  * @param
  * @retval
	***************************************************************************************************
  */
void userCanPollRoutineF(CAN_HandleTypeDef *hcan)
{
  uint32_t errorcode = HAL_CAN_ERROR_NONE;
  uint32_t msrflags = READ_REG(hcan->Instance->MSR);
  uint32_t tsrflags = READ_REG(hcan->Instance->TSR);
  uint32_t rf0rflags = READ_REG(hcan->Instance->RF0R);
  uint32_t rf1rflags = READ_REG(hcan->Instance->RF1R);
  uint32_t esrflags = READ_REG(hcan->Instance->ESR);

	/* Transmit Mailbox 0 management *****************************************/
	if ((tsrflags & CAN_TSR_RQCP0) != 0U)
	{
		/* Clear the Transmission Complete flag (and TXOK0,ALST0,TERR0 bits) */
		__HAL_CAN_CLEAR_FLAG(hcan, CAN_FLAG_RQCP0);

		//canConfigG[0].fun(canTxBuf);
		
		if ((tsrflags & CAN_TSR_TXOK0) != 0U)
		{
			 canConfigG[0].fun(canConfigG[0].dataBuf);
		}
		else
		{
			if ((tsrflags & CAN_TSR_ALST0) != 0U)
			{
				/* Update error code */
				errorcode |= HAL_CAN_ERROR_TX_ALST0;
			}
			else if ((tsrflags & CAN_TSR_TERR0) != 0U)
			{
				/* Update error code */
				errorcode |= HAL_CAN_ERROR_TX_TERR0;
			}
			else
			{
				/* Transmission Mailbox 0 abort callback */
#if USE_HAL_CAN_REGISTER_CALLBACKS == 1
				/* Call registered callback*/
				hcan->TxMailbox0AbortCallback(hcan);
#else
				/* Call weak (surcharged) callback */
				HAL_CAN_TxMailbox0AbortCallback(hcan);
#endif /* USE_HAL_CAN_REGISTER_CALLBACKS */
			}
		}
	}

	/* Transmit Mailbox 1 management *****************************************/
	if ((tsrflags & CAN_TSR_RQCP1) != 0U)
	{
		/* Clear the Transmission Complete flag (and TXOK1,ALST1,TERR1 bits) */
		__HAL_CAN_CLEAR_FLAG(hcan, CAN_FLAG_RQCP1);

		if ((tsrflags & CAN_TSR_TXOK2) != 0U)
		{
			canConfigG[2].fun(canConfigG[2].dataBuf);
		}
		else
		{
			if ((tsrflags & CAN_TSR_ALST1) != 0U)
			{
				/* Update error code */
				errorcode |= HAL_CAN_ERROR_TX_ALST1;
			}
			else if ((tsrflags & CAN_TSR_TERR1) != 0U)
			{
				/* Update error code */
				errorcode |= HAL_CAN_ERROR_TX_TERR1;
			}
			else
			{
				/* Transmission Mailbox 1 abort callback */
#if USE_HAL_CAN_REGISTER_CALLBACKS == 1
				/* Call registered callback*/
				hcan->TxMailbox1AbortCallback(hcan);
#else
				/* Call weak (surcharged) callback */
				HAL_CAN_TxMailbox1AbortCallback(hcan);
#endif /* USE_HAL_CAN_REGISTER_CALLBACKS */
			}
		}
	}

	/* Transmit Mailbox 2 management *****************************************/
	if ((tsrflags & CAN_TSR_RQCP2) != 0U)
	{
		/* Clear the Transmission Complete flag (and TXOK2,ALST2,TERR2 bits) */
		__HAL_CAN_CLEAR_FLAG(hcan, CAN_FLAG_RQCP2);

		if ((tsrflags & CAN_TSR_TXOK2) != 0U)
		{
			canConfigG[2].fun(canConfigG[2].dataBuf);
		}
		else
		{
			if ((tsrflags & CAN_TSR_ALST2) != 0U)
			{
				/* Update error code */
				errorcode |= HAL_CAN_ERROR_TX_ALST2;
			}
			else if ((tsrflags & CAN_TSR_TERR2) != 0U)
			{
				/* Update error code */
				errorcode |= HAL_CAN_ERROR_TX_TERR2;
			}
			else
			{
				/* Transmission Mailbox 2 abort callback */
#if USE_HAL_CAN_REGISTER_CALLBACKS == 1
				/* Call registered callback*/
				hcan->TxMailbox2AbortCallback(hcan);
#else
				/* Call weak (surcharged) callback */
				HAL_CAN_TxMailbox2AbortCallback(hcan);
#endif /* USE_HAL_CAN_REGISTER_CALLBACKS */
			}
		}
	}

  /* Receive FIFO 0 overrun interrupt management *****************************/
	if ((rf0rflags & CAN_RF0R_FOVR0) != 0U)
	{
		/* Set CAN error code to Rx Fifo 0 overrun error */
		errorcode |= HAL_CAN_ERROR_RX_FOV0;

		/* Clear FIFO0 Overrun Flag */
		__HAL_CAN_CLEAR_FLAG(hcan, CAN_FLAG_FOV0);
	}

  /* Receive FIFO 0 full interrupt management ********************************/
	if ((rf0rflags & CAN_RF0R_FULL0) != 0U)
	{
		/* Clear FIFO 0 full Flag */
		__HAL_CAN_CLEAR_FLAG(hcan, CAN_FLAG_FF0);

		/* Receive FIFO 0 full Callback */
#if USE_HAL_CAN_REGISTER_CALLBACKS == 1
		/* Call registered callback*/
		hcan->RxFifo0FullCallback(hcan);
#else
		/* Call weak (surcharged) callback */
		HAL_CAN_RxFifo0FullCallback(hcan);
#endif /* USE_HAL_CAN_REGISTER_CALLBACKS */
	}

  /* Receive FIFO 0 message pending interrupt management *********************/
	/* Check if message is still pending */
	if ((hcan->Instance->RF0R & CAN_RF0R_FMP0) != 0U)
	{
		canConfigG[3].fun(canConfigG[3].dataBuf);
	}

  /* Receive FIFO 1 overrun interrupt management *****************************/
	if ((rf1rflags & CAN_RF1R_FOVR1) != 0U)
	{
		/* Set CAN error code to Rx Fifo 1 overrun error */
		errorcode |= HAL_CAN_ERROR_RX_FOV1;

		/* Clear FIFO1 Overrun Flag */
		__HAL_CAN_CLEAR_FLAG(hcan, CAN_FLAG_FOV1);
	}

  /* Receive FIFO 1 full interrupt management ********************************/
	if ((rf1rflags & CAN_RF1R_FULL1) != 0U)
	{
		/* Clear FIFO 1 full Flag */
		__HAL_CAN_CLEAR_FLAG(hcan, CAN_FLAG_FF1);

		/* Receive FIFO 1 full Callback */
#if USE_HAL_CAN_REGISTER_CALLBACKS == 1
		/* Call registered callback*/
		hcan->RxFifo1FullCallback(hcan);
#else
		/* Call weak (surcharged) callback */
		HAL_CAN_RxFifo1FullCallback(hcan);
#endif /* USE_HAL_CAN_REGISTER_CALLBACKS */
	}

  /* Receive FIFO 1 message pending interrupt management *********************/
    /* Check if message is still pending */
	if ((hcan->Instance->RF1R & CAN_RF1R_FMP1) != 0U)
	{
		canConfigG[4].fun(canConfigG[4].dataBuf);
	}

  /* Sleep interrupt management *********************************************/
	if ((msrflags & CAN_MSR_SLAKI) != 0U)
	{
		/* Clear Sleep interrupt Flag */
		__HAL_CAN_CLEAR_FLAG(hcan, CAN_FLAG_SLAKI);

		/* Sleep Callback */
#if USE_HAL_CAN_REGISTER_CALLBACKS == 1
		/* Call registered callback*/
		hcan->SleepCallback(hcan);
#else
		/* Call weak (surcharged) callback */
		HAL_CAN_SleepCallback(hcan);
#endif /* USE_HAL_CAN_REGISTER_CALLBACKS */
	}

  /* WakeUp interrupt management *********************************************/
	if ((msrflags & CAN_MSR_WKUI) != 0U)
	{
		/* Clear WakeUp Flag */
		__HAL_CAN_CLEAR_FLAG(hcan, CAN_FLAG_WKU);

		/* WakeUp Callback */
#if USE_HAL_CAN_REGISTER_CALLBACKS == 1
		/* Call registered callback*/
		hcan->WakeUpFromRxMsgCallback(hcan);
#else
		/* Call weak (surcharged) callback */
		HAL_CAN_WakeUpFromRxMsgCallback(hcan);
#endif /* USE_HAL_CAN_REGISTER_CALLBACKS */
	}

  /* Error interrupts management *********************************************/
	if ((msrflags & CAN_MSR_ERRI) != 0U)
	{
		/* Check Error Warning Flag */
		if ((esrflags & CAN_ESR_EWGF) != 0U)
		{
			/* Set CAN error code to Error Warning */
			errorcode |= HAL_CAN_ERROR_EWG;

			/* No need for clear of Error Warning Flag as read-only */
		}

		/* Check Error Passive Flag */
		if ((esrflags & CAN_ESR_EPVF) != 0U)
		{
			/* Set CAN error code to Error Passive */
			errorcode |= HAL_CAN_ERROR_EPV;

			/* No need for clear of Error Passive Flag as read-only */
		}

		/* Check Bus-off Flag */
		if ((esrflags & CAN_ESR_BOFF) != 0U)
		{
			/* Set CAN error code to Bus-Off */
			errorcode |= HAL_CAN_ERROR_BOF;

			/* No need for clear of Error Bus-Off as read-only */
		}

		/* Check Last Error Code Flag */
		if (((esrflags & CAN_ESR_LEC) != 0U))
		{
			switch (esrflags & CAN_ESR_LEC)
			{
				case (CAN_ESR_LEC_0):
					/* Set CAN error code to Stuff error */
					errorcode |= HAL_CAN_ERROR_STF;
					break;
				case (CAN_ESR_LEC_1):
					/* Set CAN error code to Form error */
					errorcode |= HAL_CAN_ERROR_FOR;
					break;
				case (CAN_ESR_LEC_1 | CAN_ESR_LEC_0):
					/* Set CAN error code to Acknowledgement error */
					errorcode |= HAL_CAN_ERROR_ACK;
					break;
				case (CAN_ESR_LEC_2):
					/* Set CAN error code to Bit recessive error */
					errorcode |= HAL_CAN_ERROR_BR;
					break;
				case (CAN_ESR_LEC_2 | CAN_ESR_LEC_0):
					/* Set CAN error code to Bit Dominant error */
					errorcode |= HAL_CAN_ERROR_BD;
					break;
				case (CAN_ESR_LEC_2 | CAN_ESR_LEC_1):
					/* Set CAN error code to CRC error */
					errorcode |= HAL_CAN_ERROR_CRC;
					break;
				default:
					break;
			}

			/* Clear Last error code Flag */
			CLEAR_BIT(hcan->Instance->ESR, CAN_ESR_LEC);
		}
		/* Clear ERRI Flag */
		__HAL_CAN_CLEAR_FLAG(hcan, CAN_FLAG_ERRI);
	}

  /* Call the Error call Back in case of Errors */
  if (errorcode != HAL_CAN_ERROR_NONE)
  {
    /* Update error code in handle */
    hcan->ErrorCode |= errorcode;

    /* Call Error callback function */
#if USE_HAL_CAN_REGISTER_CALLBACKS == 1
    /* Call registered callback*/
    hcan->ErrorCallback(hcan);
#else
    /* Call weak (surcharged) callback */
    HAL_CAN_ErrorCallback(hcan);
#endif /* USE_HAL_CAN_REGISTER_CALLBACKS */
  }
}
/*****************************************************************************************************
  * @brief
  * @note
  * @param
  * @param
  * @retval
	***************************************************************************************************
  */

/*****************************************************************************************************
  * @brief
  * @note
  * @param
  * @param
  * @retval
	***************************************************************************************************
  */

void canTxMessageF1(uint8_t *data)
{
	if(canConfigG[0].mailboxSwitch != 0)
	{
		CAN->sTxMailBox[0].TDLR = ((uint32_t*)data)[0];
		CAN->sTxMailBox[0].TDHR = ((uint32_t*)data)[1];		
		/* Request transmission */
		
	}
}

void canTxMessageF2(uint8_t *data)
{
	if(canConfigG[1].mailboxSwitch != 0)
	{
		CAN->sTxMailBox[1].TDLR = ((uint32_t*)data)[0];
		CAN->sTxMailBox[1].TDHR = ((uint32_t*)data)[1];			
		/* Request transmission */
		CAN->sTxMailBox[1].TIR |= CAN_TI0R_TXRQ;
	}
}


void canTxMessageF3(uint8_t *data)
{
	if(canConfigG[2].mailboxSwitch != 0)
	{		
		CAN->sTxMailBox[2].TDLR = ((uint32_t*)data)[0];
		CAN->sTxMailBox[2].TDHR = ((uint32_t*)data)[1];			
		/* Request transmission */
		
	}
}
/*****************************************************************************************************
  * @brief
  * @note
  * @param
  * @param
  * @retval
	****************************************************************************************************
  */
void canRxMessageF1(uint8_t *data)
{
	((uint32_t*)data)[0] = CAN->sFIFOMailBox[0].RDLR;
	((uint32_t*)data)[1] = CAN->sFIFOMailBox[0].RDHR;
	/* Release RX FIFO 0 */
	CAN->RF0R |= CAN_RF0R_RFOM0;
}
 
void canRxMessageF2(uint8_t *data)
{
	((uint32_t*)data)[0] = CAN->sFIFOMailBox[1].RDLR;
	((uint32_t*)data)[1] = CAN->sFIFOMailBox[1].RDHR;
	/* Release RX FIFO 1 */
	CAN->RF1R |= CAN_RF1R_RFOM1;
}
	/*****************************************************************************************************
  * @brief
  * @note
  * @param
  * @param
  * @retval
	******************************************************************************************************
  */
	
/*####################################################################################################*/  
/************************** (C) COPYRIGHT CRESON *****END OF FILE**************************************/











