/*#####################################################################################################
  @file    
  @author  CS Team
  @brief   

  #####################################################################################################
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef user_ccp_H
#define user_ccp_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"
/* Exported types ------------------------------------------------------------*/
typedef struct
{
	uint32_t ccpAddress; 
	uint8_t  dataType;
}ccpReadT;
/* End of exported types =====================================================*/

/* Exported constants --------------------------------------------------------*/

/* End of exported constants =================================================*/

/* Exported macros -----------------------------------------------------------*/

/* End of exported macros ====================================================*/

/* Private constants ---------------------------------------------------------*/

/* End of private constants ==================================================*/

/* Private macros ------------------------------------------------------------*/

/* End of private macros =====================================================*/

/* Exported variables --------------------------------------------------------*/
extern ccpReadT ccpReadG[];
/* End of Exported variables =================================================*/

/* Exported functions --------------------------------------------------------*/
void ccpCmdF(uint8_t *inDataBuf);
/* End of exported functions =================================================*/

/* Private functions----------------------------------------------------------*/

/* End of private functions ==================================================*/


#ifdef __cplusplus
}
#endif

#endif /* user_can_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
