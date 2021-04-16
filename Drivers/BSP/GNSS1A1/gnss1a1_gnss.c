/**
 ******************************************************************************
 * @file    gnss1a1_gnss.c
 * @author  SRA
 * @brief   This file provides a set of functions needed to manage the GNSS
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics International N.V.
 * All rights reserved.</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted, provided that the following conditions are met:
 *
 * 1. Redistribution of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of other
 *    contributors to this software may be used to endorse or promote products
 *    derived from this software without specific written permission.
 * 4. This software, including modifications and/or derivative works of this
 *    software, must execute solely and exclusively on microcontroller or
 *    microprocessor devices manufactured by or for STMicroelectronics.
 * 5. Redistribution and use of this software other than as permitted under
 *    this license is void and will automatically terminate your rights under
 *    this license.
 *
 * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
 * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
 * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "gnss1a1_gnss.h"

/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup GNSS1A1 GNSS1A1
 * @{
 */

/** @defgroup GNSS1A1_GNSS GNSS1A1 GNSS
 * @{
 */

/** @defgroup GNSS1A1_GNSS_Exported_Variables GNSS1A1 GNSS Exported Variables
 * @{
 */
extern void
*GNSSCompObj[GNSS1A1_GNSS_INSTANCES_NBR]; /* This "redundant" line is here to fulfil MISRA C-2012 rule 8.4 */
void *GNSSCompObj[GNSS1A1_GNSS_INSTANCES_NBR];

/**
 * @}
 */

/** @defgroup GNSS1A1_GNSS_Private_Variables GNSS1A1 GNSS Private Variables
 * @{
 */

static GNSS_Drv_t *GNSSDrv[GNSS1A1_GNSS_INSTANCES_NBR];

/**
 * @}
 */

/** @defgroup GNSS1A1_GNSS_Private_Function_Prototypes GNSS1A1 GNSS Private Function Prototypes
 * @{
 */

#if (USE_GNSS1A1_GNSS_TESEO_LIV3F == 1)
static int32_t TESEO_LIV3F_Probe(void);

static void GNSS1A1_GNSS_Rst(void);
static void GNSS1A1_GNSS_RegisterCallbacks(void);
#endif

/**
 * @}
 */

/** @defgroup GNSS1A1_GNSS_Exported_Functions GNSS1A1_GNSS Exported Functions
 * @{
 */

 __weak int32_t GNSS1A1_GNSS_GetTick(void)
{
  return BSP_ERROR_NONE;
}

#if (USE_GNSS1A1_GNSS_TESEO_LIV3F == 1)
#if (USE_I2C == 1)
/* I2C CBs */
void GNSS1A1_GNSS_I2C_RxCb(I2C_HandleTypeDef *hi2c);
void GNSS1A1_GNSS_I2C_ErrorCb(I2C_HandleTypeDef *hi2c);
void GNSS1A1_GNSS_I2C_AbortCb(I2C_HandleTypeDef *hi2c);

__weak int32_t GNSS1A1_GNSS_I2C_Transmit_IT(uint16_t DevAddr, uint8_t *pData, uint16_t Length)
{
  UNUSED(DevAddr);
  UNUSED(pData);
  UNUSED(Length);
  return BSP_ERROR_NONE;
}
__weak int32_t GNSS1A1_GNSS_I2C_Receive_IT(uint16_t DevAddr, uint8_t *pData, uint16_t Length)
{
  UNUSED(DevAddr);
  UNUSED(pData);
  UNUSED(Length);
  return BSP_ERROR_NONE;
}

#else
/* UART CBs */
void GNSS1A1_GNSS_UART_RxCb(UART_HandleTypeDef *huart);
void GNSS1A1_GNSS_UART_ErrorCb(UART_HandleTypeDef *huart);

__weak int32_t GNSS1A1_GNSS_UART_Transmit_IT(uint16_t DevAddr, uint8_t *pData, uint16_t Length)
{
  UNUSED(DevAddr);
  UNUSED(pData);
  UNUSED(Length);
  return BSP_ERROR_NONE;
}
__weak int32_t GNSS1A1_GNSS_UART_Receive_IT(uint16_t DevAddr, uint8_t *pData, uint16_t Length)
{
  UNUSED(DevAddr);
  UNUSED(pData);
  UNUSED(Length);
  return BSP_ERROR_NONE;
}
__weak void GNSS1A1_GNSS_UART_ClearOREF(void)
{
}

#endif /* USE_I2C */
#endif

int32_t GNSS1A1_GNSS_Init(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  switch (Instance)
  {
#if (USE_GNSS1A1_GNSS_TESEO_LIV3F == 1)
    case GNSS1A1_TESEO_LIV3F:
      if (TESEO_LIV3F_Probe() != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_NO_INIT;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

int32_t GNSS1A1_GNSS_DeInit(uint32_t Instance)
{
  int32_t ret;

  if (Instance >= GNSS1A1_GNSS_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (GNSSDrv[Instance]->DeInit(GNSSCompObj[Instance]) != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  return ret;
}

const GNSS1A1_GNSS_Msg_t* GNSS1A1_GNSS_GetMessage(uint32_t Instance)
{
  const GNSS1A1_GNSS_Msg_t* msg;

  if (Instance >= GNSS1A1_GNSS_INSTANCES_NBR)
  {
    msg = NULL;
  }
  else
  {
    msg = (GNSS1A1_GNSS_Msg_t *)(GNSSDrv[Instance]->GetMessage(GNSSCompObj[Instance]));
  }

  return msg;
}

int32_t GNSS1A1_GNSS_ReleaseMessage(uint32_t Instance, const GNSS1A1_GNSS_Msg_t *Message)
{
  int32_t ret;

  if (Instance >= GNSS1A1_GNSS_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (GNSSDrv[Instance]->ReleaseMessage(GNSSCompObj[Instance], Message) != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      ret = BSP_ERROR_NONE;
    } 
  }

  return ret;
}

int32_t GNSS1A1_GNSS_Send(uint32_t Instance, const GNSS1A1_GNSS_Msg_t *Message)
{
  int32_t ret;

  if (Instance >= GNSS1A1_GNSS_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (GNSSDrv[Instance]->Send(GNSSCompObj[Instance], Message) != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      ret = BSP_ERROR_NONE;
    } 
  }

  return ret;
}

int32_t GNSS1A1_GNSS_Wakeup_Status(uint32_t Instance, uint8_t *status)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= GNSS1A1_GNSS_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    (HAL_GPIO_ReadPin(GNSS1A1_WAKEUP_PORT, GNSS1A1_WAKEUP_PIN) == GPIO_PIN_RESET) ? (*status = 0) : (*status = 1);
  }

  return ret;
}

int32_t GNSS1A1_GNSS_Reset(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= GNSS1A1_GNSS_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (GNSSDrv[GNSS1A1_TESEO_LIV3F]->DeInit(GNSSCompObj[GNSS1A1_TESEO_LIV3F]) != TESEO_LIV3F_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      if (GNSSDrv[GNSS1A1_TESEO_LIV3F]->Init(GNSSCompObj[GNSS1A1_TESEO_LIV3F]) != TESEO_LIV3F_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        GNSS1A1_GNSS_RegisterCallbacks();
      }
    }
  }

  return ret;
}

void GNSS1A1_GNSS_BackgroundProcess(uint32_t Instance)
{
  (void)Instance;

  TESEO_LIV3F_I2C_BackgroundProcess();
}

/**
 * @}
 */

/** @defgroup GNSS1A1_MOTION_SENSOR_Private_Functions IKS01A2 MOTION SENSOR Private Functions
 * @{
 */

#if (USE_GNSS1A1_GNSS_TESEO_LIV3F  == 1)
/**
 * @brief  Register Bus IOs for instance 0
 * @retval BSP status
 */
static int32_t TESEO_LIV3F_Probe(void)
{
  TESEO_LIV3F_IO_t            io_ctx;
  static TESEO_LIV3F_Object_t teseo_liv3f_obj;

  int32_t ret;

  /* Register user BUS (I2C/UART) callbacks */
#if (USE_I2C == 1)

  io_ctx.BusType     = TESEO_LIV3F_I2C_BUS;
  io_ctx.Address     = TESEO_LIV3F_I2C_7BITS_ADDR;
  io_ctx.Init        = GNSS1A1_GNSS_I2C_Init;
  io_ctx.DeInit      = GNSS1A1_GNSS_I2C_DeInit;
  io_ctx.Transmit_IT = GNSS1A1_GNSS_I2C_Transmit_IT;
  io_ctx.Receive_IT  = GNSS1A1_GNSS_I2C_Receive_IT;

#else

  io_ctx.BusType     = TESEO_LIV3F_UART_BUS;
  io_ctx.Init        = GNSS1A1_GNSS_UART_Init;
  io_ctx.DeInit      = GNSS1A1_GNSS_UART_DeInit;
  io_ctx.Transmit_IT = GNSS1A1_GNSS_UART_Transmit_IT;
  io_ctx.Receive_IT  = GNSS1A1_GNSS_UART_Receive_IT;
  io_ctx.ClearOREF   = GNSS1A1_GNSS_UART_ClearOREF;

#endif

  io_ctx.Reset         = GNSS1A1_GNSS_Rst;
  io_ctx.GetTick       = GNSS1A1_GNSS_GetTick;

//  GNSS1A1_RegisterDefaultMspCallbacks();

  if (TESEO_LIV3F_RegisterBusIO(&teseo_liv3f_obj, &io_ctx) != TESEO_LIV3F_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    GNSSCompObj[GNSS1A1_TESEO_LIV3F] = &teseo_liv3f_obj;
    /* The cast (void *) is added to bypass Misra R11.3 rule */
    GNSSDrv[GNSS1A1_TESEO_LIV3F] = (GNSS_Drv_t *)(void *)&TESEO_LIV3F_Driver;

    if (GNSSDrv[GNSS1A1_TESEO_LIV3F]->Init(GNSSCompObj[GNSS1A1_TESEO_LIV3F]) != TESEO_LIV3F_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      GNSS1A1_GNSS_RegisterCallbacks();

      ret = BSP_ERROR_NONE;
    }
  }
  return ret;
}

static void GNSS1A1_GNSS_Rst(void)
{
  HAL_GPIO_WritePin(GNSS1A1_RST_PORT, GNSS1A1_RST_PIN, GPIO_PIN_RESET);
  HAL_Delay(10);
  HAL_GPIO_WritePin(GNSS1A1_RST_PORT, GNSS1A1_RST_PIN, GPIO_PIN_SET);
  HAL_Delay(150);
}

static void GNSS1A1_GNSS_RegisterCallbacks(void)
{
#if (USE_I2C == 1)
  
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
  // FIXME: check the return value instead
  (void)GNSS1A1_RegisterRxCb(GNSS1A1_GNSS_I2C_RxCb);
  (void)GNSS1A1_RegisterErrorCb(GNSS1A1_GNSS_I2C_ErrorCb);
  (void)GNSS1A1_RegisterAbortCb(GNSS1A1_GNSS_I2C_AbortCb);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
  
#else
  
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
  // FIXME: check the return value instead
  (void)GNSS1A1_RegisterRxCb(GNSS1A1_GNSS_UART_RxCb);
  (void)GNSS1A1_RegisterErrorCb(GNSS1A1_GNSS_UART_ErrorCb);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
  
#endif /* USE_I2C */
}

#if (USE_I2C == 1)

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
void GNSS1A1_GNSS_I2C_RxCb(I2C_HandleTypeDef *hi2c)
{
  (void)(hi2c);
  TESEO_LIV3F_I2C_RxCb();
}

void GNSS1A1_GNSS_I2C_ErrorCb(I2C_HandleTypeDef *hi2c)
{
  (void)(hi2c);
  TESEO_LIV3F_I2C_ErrorCb();
}

void GNSS1A1_GNSS_I2C_AbortCb(I2C_HandleTypeDef *hi2c)
{
  (void)(hi2c);
  TESEO_LIV3F_I2C_AbortCb();
}
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */

#else

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
void GNSS1A1_GNSS_UART_RxCb(UART_HandleTypeDef *huart)
{
  (void)(huart);
  TESEO_LIV3F_UART_RxCb();
}

void GNSS1A1_GNSS_UART_ErrorCb(UART_HandleTypeDef *huart)
{
  (void)(huart);
  TESEO_LIV3F_UART_ErrorCb();
}
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */

#endif /* USE_I2C */

#endif

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
