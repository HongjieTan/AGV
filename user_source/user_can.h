/*#####################################################################################################
  @file    
  @author  CS Team
  @brief   

  #####################################################################################################
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef user_can_H
#define user_can_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"
/* Exported types ------------------------------------------------------------*/
typedef struct
{
	uint32_t mailboxId;    // mailbox address
	
	uint8_t mailboxIndex;  // mailbox index
	
	uint8_t  mailboxSwitch;// enable or disable
	
	uint8_t  dataSize;     /* This parameter must be a number between 
	                          Min_Data = 0 and Max_Data = 8 
	                       */
	
	uint8_t  IdType;       // standard ID  or  extend ID
	
	uint8_t  dataBuf[8];
	
	void (*fun)(uint8_t *data);
	
}canConfigMatrixT;
/* End of exported types =====================================================*/

/* Exported constants --------------------------------------------------------*/

/* End of exported constants =================================================*/

/* Exported macros -----------------------------------------------------------*/

/* End of exported macros ====================================================*/

/* Private constants ---------------------------------------------------------*/

/* End of private constants ==================================================*/

/* Private macros ------------------------------------------------------------*/

/* End of private macros =====================================================*/

/* Exported variables ------------------------------------------------------------*/
extern canConfigMatrixT canConfigG[];
/* End of Exported variables =====================================================*/

/* Exported functions --------------------------------------------------------*/
void canInitF(CAN_HandleTypeDef *hcan, canConfigMatrixT *config);
void userCanPollRoutineF(CAN_HandleTypeDef *hcan);
/* End of exported functions =================================================*/

/* Private functions----------------------------------------------------------*/
void canTxMessageF1(uint8_t *data);
void canTxMessageF2(uint8_t *data);
void canTxMessageF3(uint8_t *data);

void canRxMessageF1(uint8_t *data);
void canRxMessageF2(uint8_t *data);
/* End of private functions ==================================================*/


#ifdef __cplusplus
}
#endif

#endif /* user_can_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
