/**
*******************************************************************************
* @file    Applications/A_GetPos/Inc/a_gnss.h
* @author  AST/CL
* @version V1.0.0
* @date    Sep-2018
* @brief   Provides high level API for Assisted GNSS
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
#ifndef A_GNSS_H
#define A_GNSS_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup PROJECTS
 * @{
 */
 
/** @addtogroup APPLICATIONS
 * @{
 */

/** @addtogroup A_GetPos
 * @{
 */

/* Includes ------------------------------------------------------------------*/
#include "gnss_parser.h"
#include "assisted_gnss.h"

/* Defines -------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/** @addtogroup A_GetPos_PUBLIC_FUNCTIONS PUBLIC FUNCTIONS
 * @{
 */

/**
 * @brief  Parse command option for constellationspecificied by the user;
 * @param  cmd The command to be parsed
 * @retval Constellation mask
 */
int AppAGNSS_ParseSeedOp(char *cmd);

/**
 * @brief  Prepare the Password generation process
 * @retval None
 */
void AppAGNSS_PassGen(void);

/**
 * @brief  Request the seed to the remote provider
 * @param  pGNSSParser_Data Handler of the GNSS data
 * @param  seedMask Constellation mask
 * @param  seedReq Type of Assisted GNSS (PR/RT)
 * @retval 0 on success -1 otherwise
 */
int AppAGNSS_DownloadSeed(GNSSParser_Data_t *pGNSSParser_Data, int seedMask, seed_req_t seedReq);

/**
 * @brief  Start process for seed conversion (PR case)
 * @param  seedMask Constellation mask
 * @retval None
 */
void AppAGNSS_ConvertPRSeed(int seedMask);

/**
 * @brief  Start process for seed conversion (RT case)
 * @param  seedMask Constellation mask
 * @retval None
 */
void AppAGNSS_ConvertRTSeed(int seedMask);

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

#endif /* A_GNSS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
