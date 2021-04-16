/**
  ******************************************************************************
  * @file    teseo_liv3f_conf.h
  * @author  SRA
  * @brief   This file contains definitions for the TESEO LIV3F Component settings
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
#ifndef TESEO_LIV3F_CONF_H
#define TESEO_LIV3F_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#ifdef __GNUC__
  #ifndef __weak
    #define __weak __attribute__((weak))
  #endif
#endif

/* Defines -------------------------------------------------------------------*/
//#define USE_FREE_RTOS /* defined in the preprocessor options */

#define GNSS_DEBUG 0 /* Debug off */

#define configUSE_FEATURE 0 /* Feature off */

#define configUSE_ODOMETER 0 /* Odometer off */

#define configUSE_GEOFENCE 0 /* Geofence off */

#define configUSE_DATALOG 0 /* Datalog off */

#if (GNSS_DEBUG == 1)
#define PRINT_DBG(pBuffer)  GNSS_PRINT(pBuffer)
#else
#define PRINT_DBG(pBuffer)
#endif

#define PRINT_INFO(pBuffer) GNSS_PRINT(pBuffer)
#define PRINT_OUT(pBuffer)  GNSS_PRINT(pBuffer)
#define PUTC_OUT(pChar)     GNSS_PUTC(pChar)

/* Exported functions prototypes ---------------------------------------------*/
int GNSS_PRINT(char *pBuffer);
int GNSS_PUTC(char pChar);

#ifdef __cplusplus
}
#endif

#endif /* TESEO_LIV3F_CONF_H*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
