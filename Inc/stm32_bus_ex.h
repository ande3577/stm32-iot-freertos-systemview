/**
 ******************************************************************************
 * @file    stm32_bus_ex.h
 * @author  MCD Application Team
 * @brief   Header file for GNSS1A1 Bus Extension
 ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32_BUS_EX_H
#define STM32_BUS_EX_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_nucleo_conf.h"
#include "stm32l4xx_nucleo_errno.h"
#include "stm32l475e_iot01.h"

/* Defines -------------------------------------------------------------------*/

int32_t BSP_UART4_Send_IT(uint16_t DevAddr, uint8_t *pData, uint16_t Length);
int32_t BSP_UART4_Recv_IT(uint16_t DevAddr, uint8_t *pData, uint16_t Length);
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
int32_t BSP_UART4_RegisterRxCallback(pUART_CallbackTypeDef pCallback);
int32_t BSP_UART4_RegisterErrorCallback(pUART_CallbackTypeDef pCallback);
#endif

void BSP_UART4_ClearOREF(void);

int32_t BSP_I2C1_Send_IT(uint16_t DevAddr, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C1_Recv_IT(uint16_t DevAddr, uint8_t *pData, uint16_t Length);
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
int32_t BSP_I2C1_RegisterRxCallback(pI2C_CallbackTypeDef pCallback);
int32_t BSP_I2C1_RegisterErrorCallback(pI2C_CallbackTypeDef pCallback);
int32_t BSP_I2C1_RegisterAbortCallback(pI2C_CallbackTypeDef pCallback);
#endif

#ifdef __cplusplus
}
#endif

#endif /* STM32_BUS_EX_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
