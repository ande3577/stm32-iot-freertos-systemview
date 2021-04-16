/**
 ******************************************************************************
 * @file    teseo_liv3f.c
 * @author  SRA
 * @brief   TESEO_LIV3F driver file
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2018 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "teseo_liv3f.h"
#include "teseo_liv3f_i2c.h"
#include "teseo_liv3f_uart.h"

/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup Component Component
 * @{
 */

/** @defgroup TESEO_LIV3F TESEO_LIV3F
 * @{
 */

/** @defgroup TESEO_LIV3F_Exported_Variables TESEO_LIV3F Exported Variables
 * @{
 */

TESEO_LIV3F_Drv_t TESEO_LIV3F_Driver =
{
  TESEO_LIV3F_Init,
  TESEO_LIV3F_DeInit,
  TESEO_LIV3F_GetMessage,
  TESEO_LIV3F_ReleaseMessage,
  TESEO_LIV3F_Send
};

/**
 * @}
 */

/** @defgroup TESEO_LIV3F_Private_Function_Prototypes TESEO_LIV3F Private Function Prototypes
 * @{
 */
static int32_t TESEO_LIV3F_Enable(TESEO_LIV3F_Object_t *pObj);
static int32_t TESEO_LIV3F_Disable(TESEO_LIV3F_Object_t *pObj);

static int32_t ReceiveWrap(void *Handle, uint8_t *pData, uint16_t Length);
static int32_t TransmitWrap(void *Handle, uint8_t *pData, uint16_t Length);
static uint32_t GetTickWrap(void *Handle);
static void ClearOREFWrap(void *Handle);

/**
 * @}
 */

/** @defgroup TESEO_LIV3F_Exported_Functions TESEO_LIV3F Exported Functions
 * @{
 */

int32_t TESEO_LIV3F_RegisterBusIO(TESEO_LIV3F_Object_t *pObj, TESEO_LIV3F_IO_t *pIO)
{
  int32_t ret = TESEO_LIV3F_OK;

  if (pObj == NULL)
  {
    ret = TESEO_LIV3F_ERROR;
  }
  else
  {
    pObj->IO.Init        = pIO->Init;
    pObj->IO.DeInit      = pIO->DeInit;
    pObj->IO.BusType     = pIO->BusType;
    pObj->IO.Address     = pIO->Address;
    pObj->IO.Transmit_IT = pIO->Transmit_IT;
    pObj->IO.Receive_IT  = pIO->Receive_IT;
    pObj->IO.Reset       = pIO->Reset;
    pObj->IO.GetTick     = pIO->GetTick;
    pObj->IO.ClearOREF   = pIO->ClearOREF;

    /* init the message queue */
    pObj->pTeseoQueue = teseo_queue_init();

    pObj->Ctx.Receive    = ReceiveWrap;
    pObj->Ctx.Transmit   = TransmitWrap;
    pObj->Ctx.GetTick    = GetTickWrap;
    pObj->Ctx.ClearOREF  = ClearOREFWrap;
    pObj->Ctx.pQueue     = pObj->pTeseoQueue;
    pObj->Ctx.Handle     = pObj;
  }

  return ret;
}

int32_t TESEO_LIV3F_Init(TESEO_LIV3F_Object_t *pObj)
{
  int32_t ret = TESEO_LIV3F_OK;

  /* Reset the device */
  pObj->IO.Reset();

  /* Init the Bus */
  if (pObj->IO.Init == NULL)
  {
    ret = TESEO_LIV3F_ERROR;
  }
  else if (pObj->IO.Init() != TESEO_LIV3F_OK)
  {
    ret = TESEO_LIV3F_ERROR;
  }
  else
  {
    /* Enable the component SM */
    if(TESEO_LIV3F_Enable(pObj) != TESEO_LIV3F_OK)
    {
      ret = TESEO_LIV3F_ERROR;
    }
    else
    {
      pObj->is_initialized = 1;
    }
  }
  return ret;
}

int32_t TESEO_LIV3F_DeInit(TESEO_LIV3F_Object_t *pObj)
{
  int32_t ret = TESEO_LIV3F_OK;

  /* Disable the component SM */
  if(TESEO_LIV3F_Disable(pObj) != TESEO_LIV3F_OK)
  {
    ret = TESEO_LIV3F_ERROR;
  }
  else
  {
    /* Deinit the Bus */
    if (pObj->IO.DeInit == NULL)
    {
      ret = TESEO_LIV3F_ERROR;
    }
    else if (pObj->IO.DeInit() != TESEO_LIV3F_OK)
    {
      ret = TESEO_LIV3F_ERROR;
    }
    else
    {
      pObj->is_initialized = 0;
    }
  }

  return ret;
}

const TESEO_LIV3F_Msg_t* TESEO_LIV3F_GetMessage(const TESEO_LIV3F_Object_t *pObj)
{
  return teseo_queue_claim_rd_buffer(pObj->pTeseoQueue);
}

int32_t TESEO_LIV3F_ReleaseMessage(const TESEO_LIV3F_Object_t *pObj, const TESEO_LIV3F_Msg_t *Message)
{
  teseo_queue_release_rd_buffer(pObj->pTeseoQueue, Message);

  return TESEO_LIV3F_OK;
}

int32_t TESEO_LIV3F_Send(const TESEO_LIV3F_Object_t *pObj, const TESEO_LIV3F_Msg_t *Message)
{
  return pObj->IO.Transmit_IT((pObj->IO.Address << 1), Message->buf, Message->len);
}

void TESEO_LIV3F_I2C_BackgroundProcess(void)
{
  teseo_i2c_background_process();
}

/* I2C callbacks */

void TESEO_LIV3F_I2C_RxCb(void)
{
  teseo_i2c_rx_callback_ok();
}

void TESEO_LIV3F_I2C_ErrorCb(void)
{
  teseo_i2c_rx_callback_error();
}

void TESEO_LIV3F_I2C_AbortCb(void)
{
  teseo_i2c_rx_callback_abort();
}

/* UART callbacks */
void TESEO_LIV3F_UART_RxCb(void)
{
  teseo_uart_rx_callback_ok();
}

void TESEO_LIV3F_UART_ErrorCb(void)
{
  teseo_uart_rx_callback_error();
}

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Enable the TESEO component
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
static int32_t TESEO_LIV3F_Enable(TESEO_LIV3F_Object_t *pObj)
{
  int32_t ret = TESEO_LIV3F_OK;

  if(pObj->IO.BusType == TESEO_LIV3F_I2C_BUS)
  {
    teseo_i2c_enable(&pObj->Ctx);
  }
  else if(pObj->IO.BusType == TESEO_LIV3F_UART_BUS)
  {
    teseo_uart_rx_enable(&pObj->Ctx);
  }
  else
  {
    ret = TESEO_LIV3F_ERROR;
  }

  return ret;
}

/**
 * @brief  Disable the TESEO component
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
static int32_t TESEO_LIV3F_Disable(TESEO_LIV3F_Object_t *pObj)
{
  int32_t ret = TESEO_LIV3F_OK;

  if(pObj->IO.BusType == TESEO_LIV3F_I2C_BUS)
  {
    teseo_i2c_disable(&pObj->Ctx);
  }
  else if(pObj->IO.BusType == TESEO_LIV3F_UART_BUS)
  {
    teseo_uart_rx_disable(&pObj->Ctx);
  }
  else
  {
    ret = TESEO_LIV3F_ERROR;
  }

  return ret;
}

/**
 * @brief  Wrap Receive function to Bus IO function
 * @param  Handle the device handler
 * @param  pData the stored data pointer
 * @param  Length the length
 * @retval 0 in case of success, an error code otherwise
 */
static int32_t ReceiveWrap(void *Handle, uint8_t *pData, uint16_t Length)
{
  const TESEO_LIV3F_Object_t *pObj = (TESEO_LIV3F_Object_t *)Handle;

  return pObj->IO.Receive_IT((uint16_t)(pObj->IO.Address << 1), pData, Length);
}

/**
 * @brief  Wrap Transmit function to Bus IO function
 * @param  Handle the device handler
 * @param  pData the stored data pointer
 * @param  Length the length
 * @retval 0 in case of success, an error code otherwise
 */
static int32_t TransmitWrap(void *Handle, uint8_t *pData, uint16_t Length)
{
  const TESEO_LIV3F_Object_t *pObj = (TESEO_LIV3F_Object_t *)Handle;

  return pObj->IO.Transmit_IT((uint16_t)(pObj->IO.Address << 1), pData, Length);
}

/**
 * @brief  Wrap GetTick function to Bus IO function
 * @param  Handle the device handler
 * @param  pData the stored data pointer
 * @param  Length the length
 * @retval tick
 */
static uint32_t GetTickWrap(void *Handle)
{
  const TESEO_LIV3F_Object_t *pObj = (TESEO_LIV3F_Object_t *)Handle;

  return pObj->IO.GetTick();
}

/**
 * @brief  Wrap ClearOREFWrap function to Bus IO function
 * @param  Handle the device handler
 * @param  pData the stored data pointer
 * @param  Length the length
 * @retval tick
 */
static void ClearOREFWrap(void *Handle)
{
  const TESEO_LIV3F_Object_t *pObj = (TESEO_LIV3F_Object_t *)Handle;

  pObj->IO.ClearOREF();
}

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
