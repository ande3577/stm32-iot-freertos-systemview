/**
  *******************************************************************************
  * @file    teseo_liv3f_queue.h
  * @author  APG/SRA
  * @brief   Teseo III buffer queue manager.
  *
  *******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2018 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        www.st.com/software_license_agreement_liberty_v2
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
  ********************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef TESEO_LIV3F_QUEUE_H
#define TESEO_LIV3F_QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "teseo_liv3f_conf.h"

#ifdef USE_FREE_RTOS
#include <FreeRTOS.h>
#include <event_groups.h>
#include "queue.h"
#include <semphr.h>
#endif /* USE_FREE_RTOS */

/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup COMPONENT COMPONENT
 * @{
 */

/** @addtogroup TESEO_LIV3F
 * @{
 */

/** @addtogroup TESEO_LIV3F_Exported_Constants
 * @{
 */

/* Exported defines ----------------------------------------------------------*/
/**
 * @brief Constant for the maximum message queue size
 */
#define MAX_MSG_QUEUE    (8)

/**
 * @brief Constant for the maximum number of messages in the queue
 */
#define MAX_MSG_BUF     (80)

/**
 * @}
 */

/** @addtogroup TESEO_LIV3F_Exported_Types
 * @{
 */
typedef int32_t (*TESEO_LIV3F_Transmit_ptr)(void *, uint8_t *, uint16_t);
typedef int32_t (*TESEO_LIV3F_Receive_ptr)(void *, uint8_t *, uint16_t);
typedef uint32_t (*TESEO_LIV3F_GetTick_ptr)(void *);
typedef void (*TESEO_LIV3F_ClearOREF_ptr)(void *);

typedef struct
{
  uint8_t *buf;
  uint16_t len;
} TESEO_LIV3F_Msg_t;

/**
 * @brief Data structure for the Teseo Message Queue.
 */
typedef struct
{
#ifdef USE_FREE_RTOS
  SemaphoreHandle_t semaphore;
  uint32_t bitmap_unreleased_buffer_irq;
#endif /* USE_FREE_RTOS */
  uint32_t bitmap_buffer_writable;
  uint32_t bitmap_buffer_readable;
  TESEO_LIV3F_Msg_t nmea_queue[MAX_MSG_QUEUE];
  uint8_t single_message_buffer[MAX_MSG_QUEUE * MAX_MSG_BUF];
} TESEO_LIV3F_Queue_t;

typedef struct
{
  /** Component mandatory fields **/
  TESEO_LIV3F_Transmit_ptr  Transmit;
  TESEO_LIV3F_Receive_ptr   Receive;
  TESEO_LIV3F_GetTick_ptr   GetTick;
  TESEO_LIV3F_ClearOREF_ptr ClearOREF;
  TESEO_LIV3F_Queue_t       *pQueue;
  /** Customizable optional pointer **/
  void *Handle;
} TESEO_LIV3F_ctx_t;

/**
 * @}
 */

/** @addtogroup TESEO_LIV3F_Exported_Functions
 * @{
 */

/**
 * @brief  Low level driver function that initializes the TeseoIII message queue.
 * @param  None
 * @retval The pointer to the created queue
 */
TESEO_LIV3F_Queue_t *teseo_queue_init(void);

/**
 * @brief  Low level driver function to get a new buffer (queue message) to be written.
 * @param  pTeseoQueue The message queue
 * @retval The message retrieved
 */
TESEO_LIV3F_Msg_t *teseo_queue_claim_wr_buffer(TESEO_LIV3F_Queue_t *pTeseoQueue);

/**
 * @brief  Low level driver function to release a new buffer (queue message) to be written.
 * @param  pTeseoQueue The message queue
 * @param  pMsg        The message to be released for a new write
 * @retval None
 */
void teseo_queue_release_wr_buffer(TESEO_LIV3F_Queue_t *pTeseoQueue, TESEO_LIV3F_Msg_t *pMsg);

/**
 * @brief  Low level driver function to get a new buffer (queue message) to be read.
 * @param  pTeseoQueue The message queue
 * @retval The message retrieved
 */
const TESEO_LIV3F_Msg_t *teseo_queue_claim_rd_buffer(TESEO_LIV3F_Queue_t *pTeseoQueue);

/**
 * @brief  Low level driver function to release a new buffer (queue message) to be read.
 * @param  pTeseoQueue The message queue
 * @param  pMsg        The message to be released for a new read
 * @retval None
 */
void teseo_queue_release_rd_buffer(TESEO_LIV3F_Queue_t *pTeseoQueue, const TESEO_LIV3F_Msg_t *pMsg);

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

#ifdef __cplusplus
}
#endif

#endif /* TESEO_LIV3F_QUEUE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


