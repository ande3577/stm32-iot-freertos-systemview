/**
 ******************************************************************************
 * @file    gnss1a1_gnss.h
 * @author  SRA
 * @brief   This file provides a set of functions needed to manage the gnss
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef GNSS1A1_GNSS_H
#define GNSS1A1_GNSS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "gnss1a1_conf.h"
#include "gnss.h"

#ifndef USE_GNSS1A1_GNSS_TESEO_LIV3F
#define USE_GNSS1A1_GNSS_TESEO_LIV3F    1U
#endif

#if (USE_GNSS1A1_GNSS_TESEO_LIV3F == 1)
#include "teseo_liv3f.h"
#endif

/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup GNSS1A1_GNSS GNSS1A1 GNSS
 * @{
 */

/** @defgroup GNSS1A1_GNSS_Exported_Types GNSS1A1 GNSS Exported Types
 * @{
 */

/**
 * @brief GNSS Message
 */
typedef struct
{
  uint8_t *buf;
  uint16_t len;
} GNSS1A1_GNSS_Msg_t;

/**
 * @brief GNSS Capabilities
 */
typedef struct
{
  uint8_t   Geofence;
  uint8_t   Datalog;
  uint8_t   Odemeter;
  uint8_t   AssistedGNSS;
} GNSS1A1_GNSS_Capabilities_t;

/**
 * @}
 */

/** @defgroup GNSS1A1_GNSS_Exported_Constants GNSS1A1 GNSS Exported Constants
 * @{
 */
#if (USE_GNSS1A1_GNSS_TESEO_LIV3F == 1)
#define GNSS1A1_TESEO_LIV3F 0
#endif

#define GNSS1A1_GNSS_INSTANCES_NBR      (USE_GNSS1A1_GNSS_TESEO_LIV3F)

#if (GNSS1A1_GNSS_INSTANCES_NBR == 0)
#error "No gnss instance has been selected"
#endif

/**
 * @}
 */

/** @addtogroup GNSS1A1_GNSS_Exported_Functions GNSS1A1 GNSS Exported Functions
 * @{
 */

/**
 * @brief  Initializes GNSS
 * @param  Instance GNSS instance
 * @retval BSP status
 */
int32_t GNSS1A1_GNSS_Init(uint32_t Instance);

/**
 * @brief  Deinitialize GNSS
 * @param  Instance GNSS instance
 * @retval BSP status
 */
int32_t GNSS1A1_GNSS_DeInit(uint32_t Instance);

/**
 * @brief  Start (or resume after a given timeout) communication via I2C.
 * @param  Instance GNSS instance
 * @retval none
 */
void    GNSS1A1_GNSS_BackgroundProcess(uint32_t Instance);

/**
 * @brief  Get the buffer containing a message from GNSS
 * @param  Instance GNSS instance
 * @retval The message buffer
 */
const   GNSS1A1_GNSS_Msg_t* GNSS1A1_GNSS_GetMessage(uint32_t Instance);

/**
 * @brief  Release the NMEA message buffer
 * @param  Instance GNSS instance
 * @param  Message The message buffer
 * @retval BSP status
 */
int32_t GNSS1A1_GNSS_ReleaseMessage(uint32_t Instance, const GNSS1A1_GNSS_Msg_t *Message);

/**
 * @brief  Send a command to the GNSS
 * @param  Instance GNSS instance
 * @param  Message The message buffer
 * @retval BSP status
 */
int32_t GNSS1A1_GNSS_Send(uint32_t Instance, const GNSS1A1_GNSS_Msg_t *Message);

/**
 * @brief  Get the wake-up status
 * @param  Instance GNSS instance
 * @param  Message The message buffer
 * @retval BSP status
 */
int32_t GNSS1A1_GNSS_Wakeup_Status(uint32_t Instance, uint8_t *status);

/**
 * @brief  Reset the GNSS and reinit the FSM
 * @param  Instance GNSS instance
 * @param  Message The message buffer
 * @retval BSP status
 */
int32_t GNSS1A1_GNSS_Reset(uint32_t Instance);

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

#endif /* GNSS1A1_GNSS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
