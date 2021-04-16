/**
  *******************************************************************************
  * @file    Applications/GetPos/Inc/gnss_utils.h
  * @author  ADG ADD Application Software
  * @version V2.0.0
  * @date    13-Mar-2017
  * @brief   This file contains application specific utilities
  *******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
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
#ifndef _GNSS_UTILS_H_
#define _GNSS_UTILS_H_

#ifdef __cplusplus
 extern "C" {
#endif

/** @addtogroup PROJECTS
 * @{
 */

/** @addtogroup APPLICATIONS
 * @{
 */
 
/** @addtogroup GetPos
 * @{
 */
		
/** @addtogroup GetPos_PUBLIC_FUNCTIONS
 * @{
 */
/**
 * @brief  This function prints to console the command menu.
 * @param  None
 * @retval None
 */
void showCmds(void);

/**
 * @brief  This function prints to console the prompt character.
 * @param  None
 * @retval None
 */
void showPrompt(void);

/**
 * @brief  This function prints to console the help.
 * @param  None
 * @retval None
 */
void printHelp(void);

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

#endif /* _GNSS_UTILS_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

