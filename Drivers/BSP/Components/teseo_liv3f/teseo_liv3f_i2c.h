/**
  *******************************************************************************
  * @file    teseo_liv3f_i2c.h
  * @author  APG/SRA
  * @brief   Teseo III I2C handler.
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
#ifndef TESEO_LIV3F_I2C_H
#define TESEO_LIV3F_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "teseo_liv3f_queue.h"

/** @addtogroup BSP
 * @{
 */

/** @addtogroup COMPONENTS
 * @{
 */

/** @addtogroup TESEOIII
 * @{
 */
    
/** @addtogroup TESEO_LIV3F_Exported_Constants
 * @{
 */
/* Exported defines ----------------------------------------------------------*/

/**
 * @}
 */

/** @addtogroup TESEO_LIV3F_Exported_Types
 * @{
 */

/**
 * @brief Enumeration structure that contains the status of the I2C recv callback
 */
typedef enum
{
  _i2c_ok,
  _i2c_error,
  _i2c_abort
} Teseo_I2C_CB_CallerTypedef;
/**
 * @}
 */

/** @addtogroup TESEO_LIV3F_Exported_Functions
 * @{
 */

/**
 * @brief  Low level driver function to enable/disable the communication with Teseo III via I2C.
 * @param  pCtx Pointer to the Teseo relevant context
 * @param  enable Flag to enable/disable the communication
 * @param  nr_msg Number of complete messages we want to receive from the GNSS module
 * @retval None
 */
void teseo_i2c_onoff(TESEO_LIV3F_ctx_t *pCtx, uint8_t enable, int8_t nr_msg);

/**
 * @brief  Low level driver function to handle the I2C recv callabck and update consistently the FSM.
 * @param  c   The type of callback
 * @retval None
 */
void teseo_i2c_rx_callback(Teseo_I2C_CB_CallerTypedef c);

/**
 * @brief  Low level driver function to start (or resume after a given timeout) communication via I2C.
 * @param  None
 * @retval None
 * NOTE: this function has to be added in 'HAL_IncTick'
 */
void teseo_i2c_background_process(void);
/**
 * @}
 */

/* Private functions ---------------------------------------------------------*/
static inline void teseo_i2c_enable(TESEO_LIV3F_ctx_t *pCtx)
{
  teseo_i2c_onoff(pCtx, 1U, -1);
}

static inline void teseo_i2c_disable(TESEO_LIV3F_ctx_t *pCtx)
{
  teseo_i2c_onoff(pCtx, 0U, -1);
}

static inline void teseo_i2c_rx_callback_ok(void)
{
  teseo_i2c_rx_callback(_i2c_ok);
}

static inline void teseo_i2c_rx_callback_error(void)
{
  teseo_i2c_rx_callback(_i2c_error);
}

static inline void teseo_i2c_rx_callback_abort(void)
{
  teseo_i2c_rx_callback(_i2c_abort);
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

#ifdef __cplusplus
}
#endif

#endif /* TESEO_LIV3F_I2C_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


