/*#####################################################################################################
  @file    
  @author  CS Team
  @brief   

  #####################################################################################################
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx.h"
#include "user_uart.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
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
void usart1DmaRxInitF(UART_HandleTypeDef pUart, DMA_HandleTypeDef pDma, uint8_t *buf, uint8_t size)
{
	
	HAL_DMA_Start(&pDma, (uint32_t)&(pUart.Instance->RDR), (uint32_t)&(buf[0]), size);
	
	/* Clear the TC flag in the ICR register */
  __HAL_UART_CLEAR_FLAG(&pUart, UART_CLEAR_TCF);
	
	/* Enable the DMA transfer for the receiver request 
	by setting the DMAR bitin the UART CR3 register */
  SET_BIT(pUart.Instance->CR3, USART_CR3_DMAR);
}
/*****************************************************************************************************
  * @brief
  * @note
  * @param
  * @param
  * @retval
	***************************************************************************************************
  */
void usart1DmaTxInitF(UART_HandleTypeDef pUart, DMA_HandleTypeDef pDma, uint8_t *buf, uint8_t size)
{
	
	HAL_DMA_Start(&pDma, (uint32_t)&(buf[0]), (uint32_t)&(pUart.Instance->TDR), 6);
	
	/* Clear the TC flag in the ICR register */
  __HAL_UART_CLEAR_FLAG(&pUart, UART_CLEAR_TCF);
	
	/* Enable the DMA transfer for the receiver request 
	by setting the DMAR bitin the UART CR3 register */
  SET_BIT(pUart.Instance->CR3, USART_CR3_DMAT);
}
/*****************************************************************************************************
  * @brief
  * @note
  * @param
  * @param
  * @retval
	***************************************************************************************************
  */
void	usart1DmaRxF(UART_HandleTypeDef pUart, DMA_HandleTypeDef pDma, uint8_t *buf, uint8_t size)
{

	if(pDma.DmaBaseAddress->ISR&DMA_FLAG_TC5)
	{

		pDma.State = HAL_DMA_STATE_READY;
		
		HAL_DMA_Start(&pDma, (uint32_t)&( pUart.Instance->RDR), (uint32_t)&(buf[0]), size);
		
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
void	usart1DmaTxF(UART_HandleTypeDef pUart, DMA_HandleTypeDef pDma, uint8_t *buf, uint8_t size)
{

	if(pDma.DmaBaseAddress->ISR&DMA_FLAG_TC4)
	{

		pDma.State = HAL_DMA_STATE_READY;
		
		HAL_DMA_Start(&pDma, (uint32_t)&(buf[0]), (uint32_t)&(pUart.Instance->TDR), 6);
		
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
	
/*##################################################################################################*/  
/************************ (C) COPYRIGHT CRESON *****END OF FILE**************************************/
