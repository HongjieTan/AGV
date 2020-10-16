/*#####################################################################################################
  @file    
  @author  CS Team
  @brief   

  #####################################################################################################
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef user_uart_H
#define user_uart_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"
/* Exported types ------------------------------------------------------------*/

/* End of exported types =====================================================*/

/* Exported constants --------------------------------------------------------*/

/* End of exported constants =================================================*/

/* Exported macros -----------------------------------------------------------*/

/* End of exported macros ====================================================*/

/* Private constants ---------------------------------------------------------*/

/* End of private constants ==================================================*/

/* Private macros ------------------------------------------------------------*/

/* End of private macros =====================================================*/

/* Exported functions --------------------------------------------------------*/
void 	usart1DmaRxInitF(UART_HandleTypeDef pUart, DMA_HandleTypeDef pDma, uint8_t *buf, uint8_t size);
void	usart1DmaRxF(UART_HandleTypeDef pUart, DMA_HandleTypeDef pDma, uint8_t *buf, uint8_t size);
void 	usart1DmaTxInitF(UART_HandleTypeDef pUart, DMA_HandleTypeDef pDma, uint8_t *buf, uint8_t size);
void	usart1DmaTxF(UART_HandleTypeDef pUart, DMA_HandleTypeDef pDma, uint8_t *buf, uint8_t size);
/* End of exported functions =================================================*/

/* Private functions----------------------------------------------------------*/

/* End of private functions ==================================================*/


#ifdef __cplusplus
}
#endif

#endif /* user_uart_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
