/**
 ******************************************************************************
 * @file    teseo_liv3f.h
 * @author  SRA
 * @brief   TESEO_LIV3F header driver file
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
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef TESEO_LIV3F_H
#define TESEO_LIV3F_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "teseo_liv3f_queue.h"
#include <string.h>

/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup COMPONENT COMPONENT
 * @{
 */

/** @addtogroup TESEO_LIV3F TESEO_LIV3F
 * @{
 */

/** @defgroup TESEO_LIV3F_Exported_Types TESEO_LIV3F Exported Types
 * @{
 */

typedef int32_t  (*TESEO_LIV3F_Init_Func)(void);
typedef int32_t  (*TESEO_LIV3F_DeInit_Func)(void);
typedef int32_t  (*TESEO_LIV3F_Transmit_IT_Func)(uint16_t, uint8_t *, uint16_t);
typedef int32_t  (*TESEO_LIV3F_Receive_IT_Func)(uint16_t, uint8_t *, uint16_t);
typedef void     (*TESEO_LIV3F_Reset_Func)(void);
typedef int32_t  (*TESEO_LIV3F_GetTick_Func)(void);
typedef void     (*TESEO_LIV3F_ClearOREF_Func)(void);

/**
 * @brief TESEO LIV3F IO Bus structure
 */
typedef struct
{
  TESEO_LIV3F_Init_Func          Init;
  TESEO_LIV3F_DeInit_Func        DeInit;
  uint32_t                       BusType; /* 0 means UART, 1 means I2C */
  uint16_t                       Address; /* I2C only */
  TESEO_LIV3F_Transmit_IT_Func   Transmit_IT;
  TESEO_LIV3F_Receive_IT_Func    Receive_IT;
  TESEO_LIV3F_Reset_Func         Reset;
  TESEO_LIV3F_GetTick_Func       GetTick;
  TESEO_LIV3F_ClearOREF_Func     ClearOREF;
} TESEO_LIV3F_IO_t;

/**
 * @brief TESEO LIV3F Status structure
 */
typedef struct
{
  unsigned int WakeUpStatus : 1;
} TESEO_LIV3F_Status_t;

/**
 * @brief TESEO LIV3F Object Info
 */
typedef struct
{
  TESEO_LIV3F_IO_t      IO;
  TESEO_LIV3F_ctx_t     Ctx;
  TESEO_LIV3F_Queue_t   *pTeseoQueue;
  uint8_t               is_initialized;
} TESEO_LIV3F_Object_t;

/**
 * @brief TESEO LIV3F Capabilities structure
 */
typedef struct
{
  uint8_t   Geofence;
  uint8_t   Datalog;
  uint8_t   Odemeter;
  uint8_t   AssistedGNSS;
} TESEO_LIV3F_Capabilities_t;

/**
 * @brief TESEO LIV3F Driver Instance
 */
typedef struct
{
  int32_t                  (*Init)(TESEO_LIV3F_Object_t *);
  int32_t                  (*DeInit)(TESEO_LIV3F_Object_t *);
  const TESEO_LIV3F_Msg_t* (*GetMessage)(const TESEO_LIV3F_Object_t *);
  int32_t                  (*ReleaseMessage)(const TESEO_LIV3F_Object_t *, const TESEO_LIV3F_Msg_t *);
  int32_t                  (*Send)(const TESEO_LIV3F_Object_t *, const TESEO_LIV3F_Msg_t *);
} TESEO_LIV3F_Drv_t;

/**
 * @}
 */

/** @defgroup TESEO_LIV3F_Exported_Constants TESEO_LIV3F Exported Constants
 * @{
 */

#define TESEO_LIV3F_OK                  (0)
#define TESEO_LIV3F_ERROR               (-1)

#define TESEO_LIV3F_UART_BUS            (0U)
#define TESEO_LIV3F_I2C_BUS             (1U)

#define TESEO_LIV3F_I2C_7BITS_ADDR      0x3A

/**
 * @}
 */

/** @addtogroup TESEO_LIV3F_Exported_Functions TESEO_LIV3F Exported Functions
 * @{
 */

/**
 * @brief  Register Component Bus IO operations
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t                  TESEO_LIV3F_RegisterBusIO(TESEO_LIV3F_Object_t *pObj, TESEO_LIV3F_IO_t *pIO);

/**
 * @brief  Initialize the TESEO component
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t                  TESEO_LIV3F_Init(TESEO_LIV3F_Object_t *pObj);

/**
 * @brief  Deinitialize the TESEO component
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t                  TESEO_LIV3F_DeInit(TESEO_LIV3F_Object_t *pObj);

/**
 * @brief  Get the NMEA message coming from the TESEO component
 * @param  pObj the device pObj
 * @retval The message buffer
 */
const TESEO_LIV3F_Msg_t* TESEO_LIV3F_GetMessage(const TESEO_LIV3F_Object_t *pObj);

/**
 * @brief  Release the NMEA message buffer
 * @param  pObj the device pObj
 * @param  Message The message buffer
 * @retval 0 in case of success, an error code otherwise
 */
int32_t                  TESEO_LIV3F_ReleaseMessage(const TESEO_LIV3F_Object_t *pObj, const TESEO_LIV3F_Msg_t *Message);

/**
 * @brief  Send a command to the TESEO compenent
 * @param  pObj the device pObj
 * @param  Message The message buffer
 * @retval 0 in case of success, an error code otherwise
 */
int32_t                  TESEO_LIV3F_Send(const TESEO_LIV3F_Object_t *pObj, const TESEO_LIV3F_Msg_t *Message);

/**
 * @brief  Implement the API driver function to start (or resume after a given timeout) communication via I2C.
 * @retval none
 */
void                     TESEO_LIV3F_I2C_BackgroundProcess(void);

/* I2C callbacks */
/**
 * @brief  I2C Rx completion handler
 * @retval none
 */
void TESEO_LIV3F_I2C_RxCb(void);

/**
 * @brief  I2C Rx Error handler
 * @retval none
 */
void TESEO_LIV3F_I2C_ErrorCb(void);

/**
 * @brief  I2C Rx Abort handler
 * @retval none
 */
void TESEO_LIV3F_I2C_AbortCb(void);

/* UART callbacks */
/**
 * @brief  UART Rx completion handler
 * @retval none
 */
void TESEO_LIV3F_UART_RxCb(void);

/**
 * @brief  UART Rx Error handler
 * @retval none
 */
void TESEO_LIV3F_UART_ErrorCb(void);

/**
 * @}
 */

/** @addtogroup TESEO_LIV3F_Exported_Variables TESEO_LIV3F Exported Variables
 * @{
 */

extern TESEO_LIV3F_Drv_t TESEO_LIV3F_Driver;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
