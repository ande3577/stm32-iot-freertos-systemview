/**
  ******************************************************************************
  * @file           : stm32l4xx_nucleo_bus.h
  * @brief          : source file for the BSP BUS IO driver
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

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_nucleo_bus.h"

__weak HAL_StatusTypeDef MX_UART4_UART_Init(UART_HandleTypeDef* huart);
__weak HAL_StatusTypeDef MX_I2C1_Init(I2C_HandleTypeDef* hi2c);

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32L4XX_NUCLEO
  * @{
  */

/** @defgroup STM32L4XX_NUCLEO_BUS STM32L4XX_NUCLEO BUS
  * @{
  */
#define DIV_ROUND_CLOSEST(x, d)  (((x) + ((d) / 2U)) / (d))

#define I2C_ANALOG_FILTER_ENABLE	1U
#define I2C_ANALOG_FILTER_DELAY_MIN	50U	/* ns */
#define I2C_ANALOG_FILTER_DELAY_MAX	260U	/* ns */
#define I2C_ANALOG_FILTER_DELAY_DEFAULT	2U	/* ns */

#define VALID_PRESC_NBR     100U
#define PRESC_MAX			16U
#define SCLDEL_MAX			16U
#define SDADEL_MAX			16U
#define SCLH_MAX			256U
#define SCLL_MAX			256U
#define I2C_DNF_MAX			16U

#define NSEC_PER_SEC	                1000000000UL

/**
 * struct i2c_charac - private i2c specification timing
 * @rate: I2C bus speed (Hz)
 * @rate_min: 80% of I2C bus speed (Hz)
 * @rate_max: 100% of I2C bus speed (Hz)
 * @fall_max: Max fall time of both SDA and SCL signals (ns)
 * @rise_max: Max rise time of both SDA and SCL signals (ns)
 * @hddat_min: Min data hold time (ns)
 * @vddat_max: Max data valid time (ns)
 * @sudat_min: Min data setup time (ns)
 * @l_min: Min low period of the SCL clock (ns)
 * @h_min: Min high period of the SCL clock (ns)
 */
struct i2c_specs 
{
	uint32_t rate;
	uint32_t rate_min;
	uint32_t rate_max;
	uint32_t fall_max;
	uint32_t rise_max;
	uint32_t hddat_min;
	uint32_t vddat_max;
	uint32_t sudat_min;
	uint32_t l_min;
	uint32_t h_min;
};

enum i2c_speed 
{
	I2C_SPEED_STANDARD = 0U, 	/* 100 kHz */
	I2C_SPEED_FAST = 1U, 		/* 400 kHz */
	I2C_SPEED_FAST_PLUS = 2U, 	/* 1 MHz */
};

/**
 * struct i2c_setup - private I2C timing setup parameters
 * @rise_time: Rise time (ns)
 * @fall_time: Fall time (ns)
 * @dnf: Digital filter coefficient (0-16)
 * @analog_filter: Analog filter delay (On/Off)
 */
struct i2c_setup 
{
	uint32_t rise_time;
	uint32_t fall_time;
	uint32_t dnf;
	uint32_t analog_filter;
};

/**
 * struct i2c_timings - private I2C output parameters
 * @node: List entry
 * @presc: Prescaler value
 * @scldel: Data setup time
 * @sdadel: Data hold time
 * @sclh: SCL high period (master mode)
 * @scll: SCL low period (master mode)
 */
struct i2c_timings 
{
	uint32_t presc;
	uint32_t scldel;
	uint32_t sdadel;
	uint32_t sclh;
	uint32_t scll;
};

#if (USE_CUBEMX_BSP_V2 == 1)
static const struct i2c_specs i2c_specs[] = 
{
	[I2C_SPEED_STANDARD] = 
	{
		.rate = 100000,
		.rate_min = 100000,
		.rate_max = 120000,
		.fall_max = 300,
		.rise_max = 1000,
		.hddat_min = 0,
		.vddat_max = 3450,
		.sudat_min = 250,
		.l_min = 4700,
		.h_min = 4000,
	},
	[I2C_SPEED_FAST] = 
	{
		.rate = 400000,
		.rate_min = 320000,
		.rate_max = 400000,
		.fall_max = 300,
		.rise_max = 300,
		.hddat_min = 0,
		.vddat_max = 900,
		.sudat_min = 100,
		.l_min = 1300,
		.h_min = 600,
	},
	[I2C_SPEED_FAST_PLUS] = 
	{
		.rate = 1000000,
		.rate_min = 800000,
		.rate_max = 1000000,
		.fall_max = 120,
		.rise_max = 120,
		.hddat_min = 0,
		.vddat_max = 450,
		.sudat_min = 50,
		.l_min = 500,
		.h_min = 260,
	},
};

static const struct i2c_setup i2c_user_setup[] = 
{
  [I2C_SPEED_STANDARD] = 
  {
	.rise_time = 400,
	.fall_time = 100,
	.dnf = I2C_ANALOG_FILTER_DELAY_DEFAULT,
	.analog_filter = 1,
 },
 [I2C_SPEED_FAST] = 
 {
	.rise_time = 250,
	.fall_time = 100,
	.dnf = I2C_ANALOG_FILTER_DELAY_DEFAULT,
	.analog_filter = 1,
 },
 [I2C_SPEED_FAST_PLUS] = 
 { 
	.rise_time = 60,
	.fall_time = 100,
	.dnf = I2C_ANALOG_FILTER_DELAY_DEFAULT,
	.analog_filter = 1,
 },
};
#endif

/** @defgroup STM32L4XX_NUCLEO_BUS_Exported_Variables BUS Exported Variables
  * @{
  */

UART_HandleTypeDef huart4;
I2C_HandleTypeDef hi2c1;

/**
  * @}
  */

/** @defgroup STM32L4XX_NUCLEO_BUS_Private_Variables BUS Private Variables
  * @{
  */

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
static uint32_t IsUART4MspCbValid = 0;	
int32_t BSP_UART4_RegisterDefaultMspCallbacks (void);									
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */				

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
static uint32_t IsI2C1MspCbValid = 0;	
int32_t BSP_I2C1_RegisterDefaultMspCallbacks (void);									
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */

static uint32_t UART4InitCounter = 0;
static uint32_t I2C1InitCounter = 0;

/**
  * @}
  */

/** @defgroup STM32L4XX_NUCLEO_BUS_Private_FunctionPrototypes  BUS Private Function
  * @{
  */  

static void UART4_MspInit(UART_HandleTypeDef* huart); 
static void UART4_MspDeInit(UART_HandleTypeDef* huart);

static void I2C1_MspInit(I2C_HandleTypeDef* hI2c); 
static void I2C1_MspDeInit(I2C_HandleTypeDef* hI2c);
#if (USE_CUBEMX_BSP_V2 == 1)
static uint32_t I2C_GetTiming(uint32_t clock_src_hz, uint32_t i2cfreq_hz);
#endif

/**
  * @}
  */

/** @defgroup STM32L4XX_NUCLEO_LOW_LEVEL_Private_Functions STM32L4XX_NUCLEO LOW LEVEL Private Functions
  * @{
  */ 
  
/** @defgroup STM32L4XX_NUCLEO_BUS_Exported_Functions STM32L4XX_NUCLEO_BUS Exported Functions
  * @{
  */   

/* BUS IO driver over USART Peripheral */
/*******************************************************************************
                            BUS OPERATIONS INIT/DEINIT OVER USART
*******************************************************************************/
/**
  * @brief  Initializes USART HAL. 
  * @param  Init : UART initialization parameters
  * @retval BSP status
  */
int32_t BSP_UART4_Init(void) 
{
  int32_t ret = BSP_ERROR_NONE;
  
  huart4.Instance  = UART4;
  
  if(UART4InitCounter++ == 0)
  { 
    if (HAL_UART_GetState(&huart4) == HAL_UART_STATE_RESET) 
    { 
#if (USE_HAL_UART_REGISTER_CALLBACKS == 0)
      /* Init the UART Msp */
      UART4_MspInit(&huart4);
#else
      if(IsUART4MspCbValid == 0U)
      {
        if(BSP_UART4_RegisterDefaultMspCallbacks() != BSP_ERROR_NONE)
        {
          return BSP_ERROR_MSP_FAILURE;
        }
      }
#endif   
      if(ret == BSP_ERROR_NONE)
      {
        /* Init the UART */
        if (MX_UART4_UART_Init(&huart4) != HAL_OK)
        {
          ret = BSP_ERROR_BUS_FAILURE;
        }
      }
    } 
  }
  return ret;
}

/**
  * @brief  DeInitializes UART HAL.
  * @retval None
  * @retval BSP status
  */
int32_t BSP_UART4_DeInit(void) 
{
  int32_t ret = BSP_ERROR_BUS_FAILURE;
  
  if (UART4InitCounter > 0)
  {       
    if (--UART4InitCounter == 0)
    { 
#if (USE_HAL_UART_REGISTER_CALLBACKS == 0)
      UART4_MspDeInit(&huart4);
#endif  
      /* DeInit the UART*/
      if (HAL_UART_DeInit(&huart4) == HAL_OK) 
      {
        ret = BSP_ERROR_NONE;
      }
    }
  }
  return ret;
}

/*******************************************************************************
                            BUS OPERATIONS INIT/DEINIT OVER I2C
*******************************************************************************/
/**
  * @brief  Initialize I2C HAL
  * @retval BSP status
  */
int32_t BSP_I2C1_Init(void) 
{

  int32_t ret = BSP_ERROR_NONE;
  
  hi2c1.Instance  = I2C1;

  if(I2C1InitCounter++ == 0)
  {     
    if (HAL_I2C_GetState(&hi2c1) == HAL_I2C_STATE_RESET)
    {  
    #if (USE_HAL_I2C_REGISTER_CALLBACKS == 0)
      /* Init the I2C Msp */
      I2C1_MspInit(&hi2c1);
    #else
      if(IsI2C1MspCbValid == 0U)
      {
        if(BSP_I2C1_RegisterDefaultMspCallbacks() != BSP_ERROR_NONE)
        {
          return BSP_ERROR_MSP_FAILURE;
        }
      }
    #endif
      if(ret == BSP_ERROR_NONE)
	  {
    	/* Init the I2C */
    	if(MX_I2C1_Init(&hi2c1) != HAL_OK)
    	{
      		ret = BSP_ERROR_BUS_FAILURE;
    	}
    	else if(HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK) 
    	{
      	  ret = BSP_ERROR_BUS_FAILURE;    		
    	}
    	else
    	{
      		ret = BSP_ERROR_NONE;
    	}
	  }	
    }
  }
  return ret;
}

/**
  * @brief  DeInitialize I2C HAL.
  * @retval BSP status
  */
int32_t BSP_I2C1_DeInit(void) 
{
  int32_t ret = BSP_ERROR_NONE;
  
  if (I2C1InitCounter > 0)
  {       
    if (--I2C1InitCounter == 0)
    {    
  #if (USE_HAL_I2C_REGISTER_CALLBACKS == 0)
    	/* DeInit the I2C */ 
    	I2C1_MspDeInit(&hi2c1);
  #endif  
  		/* DeInit the I2C */ 
  		if (HAL_I2C_DeInit(&hi2c1) != HAL_OK) 
  		{
    		ret = BSP_ERROR_BUS_FAILURE;
  		}
    }
  }
  return ret;
}

/*******************************************************************************
                            BUS OPERATIONS SEND/RECV OVER USART
*******************************************************************************/
/**
  * @brief  Write Data through UART BUS.
  * @param  pData: Pointer to data buffer to send
  * @param  Length: Length of data in byte
  * @retval BSP status
  */
int32_t BSP_UART4_Send(uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_UNKNOWN_FAILURE;
  
  if(HAL_UART_Transmit(&huart4, pData, Length, BUS_UART4_POLL_TIMEOUT) == HAL_OK)
  {
      ret = BSP_ERROR_NONE;
  }
  return ret;
}

/**
  * @brief  Receive Data from UART BUS
  * @param  pData: Pointer to data buffer to receive
  * @param  Length: Length of data in byte
  * @retval BSP status
  */
int32_t  BSP_UART4_Recv(uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_UNKNOWN_FAILURE;
  
  if(HAL_UART_Receive(&huart4, pData, Length, BUS_UART4_POLL_TIMEOUT) == HAL_OK)
  {
      ret = BSP_ERROR_NONE;
  }
  return ret;
}

/*******************************************************************************
                            BUS OPERATIONS SEND/RECV OVER I2C
*******************************************************************************/
/**
  * @brief  Check whether the I2C bus is ready.
  * @param DevAddr : I2C device address
  * @param Trials : Check trials number
  *	@retval BSP status
  */
int32_t BSP_I2C1_IsReady(uint16_t DevAddr, uint32_t Trials) 
{
  int32_t ret = BSP_ERROR_NONE;
  
  if (HAL_I2C_IsDeviceReady(&hi2c1, DevAddr, Trials, BUS_I2C1_POLL_TIMEOUT) != HAL_OK)
  {
    ret = BSP_ERROR_BUSY;
  } 
  
  return ret;
}

/**
  * @brief  Write a value in a register of the device through BUS.
  * @param  DevAddr Device address on Bus.
  * @param  Reg    The target register address to write
  * @param  pData  Pointer to data buffer to write
  * @param  Length Data Length
  * @retval BSP status
  */

int32_t BSP_I2C1_WriteReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_NONE;  
  
  if (HAL_I2C_Mem_Write(&hi2c1, DevAddr,Reg, I2C_MEMADD_SIZE_8BIT,pData, Length, BUS_I2C1_POLL_TIMEOUT) != HAL_OK)
  {    
    if (HAL_I2C_GetError(&hi2c1) == HAL_I2C_ERROR_AF)
    {
      ret = BSP_ERROR_BUS_ACKNOWLEDGE_FAILURE;
    }
    else
    {
      ret =  BSP_ERROR_PERIPH_FAILURE;
    }
  }
  return ret;
}

/**
  * @brief  Read a register of the device through BUS
  * @param  DevAddr Device address on Bus.
  * @param  Reg    The target register address to read
  * @param  pData  Pointer to data buffer to read
  * @param  Length Data Length
  * @retval BSP status
  */
int32_t  BSP_I2C1_ReadReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length) 
{
  int32_t ret = BSP_ERROR_NONE;
  
  if (HAL_I2C_Mem_Read(&hi2c1, DevAddr, Reg, I2C_MEMADD_SIZE_8BIT, pData, Length, BUS_I2C1_POLL_TIMEOUT) != HAL_OK)
  { 
    if (HAL_I2C_GetError(&hi2c1) == HAL_I2C_ERROR_AF)
    {
      ret = BSP_ERROR_BUS_ACKNOWLEDGE_FAILURE;
    }
    else
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
  }
  return ret;
}

/**

  * @brief  Write a value in a register of the device through BUS.
  * @param  DevAddr Device address on Bus.
  * @param  Reg    The target register address to write

  * @param  pData  Pointer to data buffer to write
  * @param  Length Data Length
  * @retval BSP statu
  */
int32_t BSP_I2C1_WriteReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length) 
{
  int32_t ret = BSP_ERROR_NONE;
  
  
  if (HAL_I2C_Mem_Write(&hi2c1, DevAddr, Reg, I2C_MEMADD_SIZE_16BIT, pData, Length, BUS_I2C1_POLL_TIMEOUT) != HAL_OK)
  {
    if (HAL_I2C_GetError(&hi2c1) == HAL_I2C_ERROR_AF)    
    {
      ret = BSP_ERROR_BUS_ACKNOWLEDGE_FAILURE;
    }
    else
    {
      ret =  BSP_ERROR_PERIPH_FAILURE;
    }
  }
  return ret;
}

/**
  * @brief  Read registers through a bus (16 bits)
  * @param  DevAddr: Device address on BUS
  * @param  Reg: The target register address to read
  * @param  Length Data Length
  * @retval BSP status
  */
int32_t  BSP_I2C1_ReadReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length) 
{
  int32_t ret = BSP_ERROR_NONE;  
 
  if (HAL_I2C_Mem_Read(&hi2c1, DevAddr, Reg, I2C_MEMADD_SIZE_16BIT, pData, Length, BUS_I2C1_POLL_TIMEOUT) != HAL_OK)
  {
    if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
    {
      ret =  BSP_ERROR_BUS_ACKNOWLEDGE_FAILURE;
    }
    else
    {
      ret =  BSP_ERROR_PERIPH_FAILURE;
    }
  }
  return ret;
}

/**
  * @brief  Send an amount width data through bus (Simplex)
  * @param  DevAddr: Device address on Bus.
  * @param  pData: Data pointer
  * @param  Length: Data length
  * @retval BSP status
  */
int32_t BSP_I2C1_Send(uint16_t DevAddr, uint8_t *pData, uint16_t Length) {
  int32_t ret = BSP_ERROR_NONE;	  
  
  if (HAL_I2C_Master_Transmit(&hi2c1, DevAddr, pData, Length, BUS_I2C1_POLL_TIMEOUT) != HAL_OK)
  {
    if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
    {
      ret = BSP_ERROR_BUS_ACKNOWLEDGE_FAILURE;
    }
    else
    {
      ret =  BSP_ERROR_PERIPH_FAILURE;
    }
  }

  return ret;
}

/**
  * @brief  Receive an amount of data through a bus (Simplex)
  * @param  DevAddr: Device address on Bus.
  * @param  pData: Data pointer
  * @param  Length: Data length
  * @retval BSP status
  */
int32_t BSP_I2C1_Recv(uint16_t DevAddr, uint8_t *pData, uint16_t Length) {	
  int32_t ret = BSP_ERROR_NONE;
  
  if (HAL_I2C_Master_Receive(&hi2c1, DevAddr, pData, Length, BUS_I2C1_POLL_TIMEOUT) != HAL_OK)
  {
    if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
    {
      ret = BSP_ERROR_BUS_ACKNOWLEDGE_FAILURE;
    }
    else
    {
      ret =  BSP_ERROR_PERIPH_FAILURE;
    }
  }
  return ret;
}

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)  
/**
  * @brief Register Default BSP UART4 Bus Msp Callbacks
  * @retval BSP status
  */
int32_t BSP_UART4_RegisterDefaultMspCallbacks (void)
{

  __HAL_UART_RESET_HANDLE_STATE(&huart4);
  
  /* Register MspInit Callback */
  if (HAL_UART_RegisterCallback(&huart4, HAL_UART_MSPINIT_CB_ID, UART4_MspInit)  != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }
  
  /* Register MspDeInit Callback */
  if (HAL_UART_RegisterCallback(&huart4, HAL_UART_MSPDEINIT_CB_ID, UART4_MspDeInit) != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }
  IsUART4MspCbValid = 1;
  
  return BSP_ERROR_NONE;  
}

/**
  * @brief BSP UART4 Bus Msp Callback registering
  * @param Callbacks     pointer to UART4 MspInit/MspDeInit callback functions
  * @retval BSP status
  */
int32_t BSP_UART4_RegisterMspCallbacks (BSP_UART_Cb_t *Callbacks)
{
  /* Prevent unused argument(s) compilation warning */
  __HAL_UART_RESET_HANDLE_STATE(&huart4);  
 
   /* Register MspInit Callback */
  if (HAL_UART_RegisterCallback(&huart4, HAL_UART_MSPINIT_CB_ID, Callbacks->pMspInitCb)  != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }
  
  /* Register MspDeInit Callback */
  if (HAL_UART_RegisterCallback(&huart4, HAL_UART_MSPDEINIT_CB_ID, Callbacks->pMspDeInitCb) != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }
  
  IsUART4MspCbValid = 1;
  
  return BSP_ERROR_NONE;  
}
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)  
/**
  * @brief Register Default BSP I2C1 Bus Msp Callbacks
  * @retval BSP status
  */
int32_t BSP_I2C1_RegisterDefaultMspCallbacks (void)
{

  __HAL_I2C_RESET_HANDLE_STATE(&hi2c1);
  
  /* Register MspInit Callback */
  if (HAL_I2C_RegisterCallback(&hi2c1, HAL_I2C_MSPINIT_CB_ID, I2C1_MspInit)  != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }
  
  /* Register MspDeInit Callback */
  if (HAL_I2C_RegisterCallback(&hi2c1, HAL_I2C_MSPDEINIT_CB_ID, I2C1_MspDeInit) != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }
  IsI2C1MspCbValid = 1;
  
  return BSP_ERROR_NONE;  
}

/**
  * @brief BSP I2C1 Bus Msp Callback registering
  * @param Callbacks     pointer to I2C1 MspInit/MspDeInit callback functions
  * @retval BSP status
  */
int32_t BSP_I2C1_RegisterMspCallbacks (BSP_I2C_Cb_t *Callbacks)
{
  /* Prevent unused argument(s) compilation warning */
  __HAL_I2C_RESET_HANDLE_STATE(&hi2c1);  
 
   /* Register MspInit Callback */
  if (HAL_I2C_RegisterCallback(&hi2c1, HAL_I2C_MSPINIT_CB_ID, Callbacks->pMspInitCb)  != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }
  
  /* Register MspDeInit Callback */
  if (HAL_I2C_RegisterCallback(&hi2c1, HAL_I2C_MSPDEINIT_CB_ID, Callbacks->pMspDeInitCb) != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }
  
  IsI2C1MspCbValid = 1;
  
  return BSP_ERROR_NONE;  
}
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */

/**
  * @brief  Return system tick in ms
  * @retval Current HAL time base time stamp
  */
int32_t BSP_GetTick(void) {
  return HAL_GetTick();
}

/* UART4 init function */ 

__weak HAL_StatusTypeDef MX_UART4_UART_Init(UART_HandleTypeDef* huart)
{
  HAL_StatusTypeDef ret = HAL_OK;
  huart->Instance = UART4;
  huart->Init.BaudRate = 9600;
  huart->Init.WordLength = UART_WORDLENGTH_8B;
  huart->Init.StopBits = UART_STOPBITS_1;
  huart->Init.Parity = UART_PARITY_NONE;
  huart->Init.Mode = UART_MODE_TX_RX;
  huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart->Init.OverSampling = UART_OVERSAMPLING_16;
  huart->Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart->AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(huart) != HAL_OK)
  {
    ret = HAL_ERROR;
  }

  return ret;
}

static void UART4_MspInit(UART_HandleTypeDef* uartHandle)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  /* USER CODE BEGIN UART4_MspInit 0 */

  /* USER CODE END UART4_MspInit 0 */
    /* Enable Peripheral clock */
    __HAL_RCC_UART4_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**UART4 GPIO Configuration    
    PA0     ------> UART4_TX
    PA1     ------> UART4_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Peripheral interrupt init */
    HAL_NVIC_SetPriority(UART4_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(UART4_IRQn);
  /* USER CODE BEGIN UART4_MspInit 1 */

  /* USER CODE END UART4_MspInit 1 */
}

static void UART4_MspDeInit(UART_HandleTypeDef* uartHandle)
{
  /* USER CODE BEGIN UART4_MspDeInit 0 */

  /* USER CODE END UART4_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_UART4_CLK_DISABLE();
  
    /**UART4 GPIO Configuration    
    PA0     ------> UART4_TX
    PA1     ------> UART4_RX 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0|GPIO_PIN_1);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(UART4_IRQn);

  /* USER CODE BEGIN UART4_MspDeInit 1 */

  /* USER CODE END UART4_MspDeInit 1 */
}

/* I2C1 init function */ 

__weak HAL_StatusTypeDef MX_I2C1_Init(I2C_HandleTypeDef* hi2c)
{
  HAL_StatusTypeDef ret = HAL_OK;
  hi2c->Instance = I2C1;
  hi2c->Init.Timing = 0x10909CEC;
  hi2c->Init.OwnAddress1 = 0;
  hi2c->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c->Init.OwnAddress2 = 0;
  hi2c->Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(hi2c) != HAL_OK)
  {
    ret = HAL_ERROR;
  }

  if (HAL_I2CEx_ConfigAnalogFilter(hi2c, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    ret = HAL_ERROR;
  }

  if (HAL_I2CEx_ConfigDigitalFilter(hi2c, 0) != HAL_OK)
  {
    ret = HAL_ERROR;
  }

  return ret;
}

static void I2C1_MspInit(I2C_HandleTypeDef* i2cHandle)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  /* USER CODE BEGIN I2C1_MspInit 0 */

  /* USER CODE END I2C1_MspInit 0 */
  
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration    
    PB8     ------> I2C1_SCL
    PB9     ------> I2C1_SDA 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Peripheral clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();

    /* Peripheral interrupt init */
    HAL_NVIC_SetPriority(I2C1_EV_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
    HAL_NVIC_SetPriority(I2C1_ER_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
  /* USER CODE BEGIN I2C1_MspInit 1 */

  /* USER CODE END I2C1_MspInit 1 */
}

static void I2C1_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{
  /* USER CODE BEGIN I2C1_MspDeInit 0 */

  /* USER CODE END I2C1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C1_CLK_DISABLE();
  
    /**I2C1 GPIO Configuration    
    PB8     ------> I2C1_SCL
    PB9     ------> I2C1_SDA 
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8|GPIO_PIN_9);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(I2C1_EV_IRQn);

    HAL_NVIC_DisableIRQ(I2C1_ER_IRQn);

  /* USER CODE BEGIN I2C1_MspDeInit 1 */

  /* USER CODE END I2C1_MspDeInit 1 */
}

#if (USE_CUBEMX_BSP_V2 == 1)
/**
  * @brief  Convert the I2C Frequency into I2C timing.
  * @param  clock_src_hz : I2C source clock in HZ.
  * @param  i2cfreq_hz : I2C frequency in Hz.
  * @retval Prescaler dividor
  */        
static uint32_t I2C_GetTiming(uint32_t clock_src_hz, uint32_t i2cfreq_hz)
{
  uint32_t ret = 0;
  uint32_t speed = 0;
  uint32_t is_valid_speed = 0; 
  uint32_t p_prev = PRESC_MAX;
  uint32_t i2cclk;
  uint32_t i2cspeed;
  uint32_t clk_error_prev;
  uint32_t tsync;
  uint32_t af_delay_min, af_delay_max;
  uint32_t dnf_delay;
  uint32_t clk_min, clk_max;
  int32_t sdadel_min, sdadel_max;
  int32_t scldel_min;
  struct i2c_timings *s;
  struct i2c_timings valid_timing[VALID_PRESC_NBR];
  uint32_t p, l, a, h;
  uint32_t valid_timing_nbr = 0;

  if((clock_src_hz == 0U) || (i2cfreq_hz == 0U))
  {
    ret = 0;
  }
  else
  {
    for (uint32_t i =0 ; i <=  (uint32_t)I2C_SPEED_FAST_PLUS ; i++)
    {
      if ((i2cfreq_hz >= i2c_specs[i].rate_min) &&
          (i2cfreq_hz <= i2c_specs[i].rate_max))
      {
        is_valid_speed = 1;
        speed = i;
        break;
      }
    }

    if(is_valid_speed != 0U)
    {
      i2cclk = DIV_ROUND_CLOSEST(NSEC_PER_SEC, clock_src_hz);
      i2cspeed = DIV_ROUND_CLOSEST(NSEC_PER_SEC, i2cfreq_hz);

      clk_error_prev = i2cspeed;

      /*  Analog and Digital Filters */
      af_delay_min =((i2c_user_setup[speed].analog_filter == 1U)? I2C_ANALOG_FILTER_DELAY_MIN : 0U);
      af_delay_max =((i2c_user_setup[speed].analog_filter == 1U)? I2C_ANALOG_FILTER_DELAY_MAX : 0U);

      dnf_delay = i2c_user_setup[speed].dnf * i2cclk;

      sdadel_min = (int32_t)i2c_user_setup[speed].fall_time - (int32_t)i2c_specs[speed].hddat_min -
                   (int32_t)af_delay_min - (int32_t)(((int32_t)i2c_user_setup[speed].dnf + 3) * (int32_t)i2cclk);

      sdadel_max = (int32_t)i2c_specs[speed].vddat_max - (int32_t)i2c_user_setup[speed].rise_time -
                   (int32_t)af_delay_max - (int32_t)(((int32_t)i2c_user_setup[speed].dnf + 4) * (int32_t)i2cclk);

      scldel_min = (int32_t)i2c_user_setup[speed].rise_time + (int32_t)i2c_specs[speed].sudat_min;

      if (sdadel_min <= 0)
      {
        sdadel_min = 0;
      }

      if (sdadel_max <= 0)
      {
        sdadel_max = 0;
      }

      /* Compute possible values for PRESC, SCLDEL and SDADEL */
      for (p = 0; p < PRESC_MAX; p++)
      {
        for (l = 0; l < SCLDEL_MAX; l++)
        {
          uint32_t scldel = (l + 1U) * (p + 1U) * i2cclk;

          if (scldel < (uint32_t)scldel_min)
          {
            continue;
          }

          for (a = 0; a < SDADEL_MAX; a++)
          {
            uint32_t sdadel = (a * (p + 1U) + 1U) * i2cclk;

            if (((sdadel >= (uint32_t)sdadel_min) && (sdadel <= (uint32_t)sdadel_max))&& (p != p_prev))
            {
              valid_timing[valid_timing_nbr].presc = p;
              valid_timing[valid_timing_nbr].scldel = l;
              valid_timing[valid_timing_nbr].sdadel = a;
              p_prev = p;
              valid_timing_nbr ++;

              if(valid_timing_nbr >= VALID_PRESC_NBR)
              {
                /* max valid timing buffer is full, use only these values*/
                goto  Compute_scll_sclh;
              }
            }
          }
        }
      }

      if (valid_timing_nbr == 0U)
      {
        return 0;
      }

    Compute_scll_sclh:
      tsync = af_delay_min + dnf_delay + (2U * i2cclk);
      s = NULL;
      clk_max = NSEC_PER_SEC / i2c_specs[speed].rate_min;
      clk_min = NSEC_PER_SEC / i2c_specs[speed].rate_max;

      /*
      * Among Prescaler possibilities discovered above figures out SCL Low
      * and High Period. Provided:
      * - SCL Low Period has to be higher than Low Period of tehs SCL Clock
      *   defined by I2C Specification. I2C Clock has to be lower than
      *   (SCL Low Period - Analog/Digital filters) / 4.
      * - SCL High Period has to be lower than High Period of the SCL Clock
      *   defined by I2C Specification
      * - I2C Clock has to be lower than SCL High Period
      */
      for (uint32_t count = 0; count < valid_timing_nbr; count++)
      {
        uint32_t prescaler = (valid_timing[count].presc + 1U) * i2cclk;

        for (l = 0; l < SCLL_MAX; l++)
        {
          uint32_t tscl_l = (l + 1U) * prescaler + tsync;

          if ((tscl_l < i2c_specs[speed].l_min) || (i2cclk >= ((tscl_l - af_delay_min - dnf_delay) / 4U)))
          {
            continue;
          }

          for (h = 0; h < SCLH_MAX; h++)
          {
            uint32_t tscl_h = (h + 1U) * prescaler + tsync;
            uint32_t tscl = tscl_l + tscl_h + i2c_user_setup[speed].rise_time + i2c_user_setup[speed].fall_time;

            if ((tscl >= clk_min) && (tscl <= clk_max) && (tscl_h >= i2c_specs[speed].h_min) && (i2cclk < tscl_h))
            {
              int32_t clk_error = (int32_t)tscl - (int32_t)i2cspeed;

              if (clk_error < 0)
              {
                clk_error = -clk_error;
              }

              /* save the solution with the lowest clock error */
              if ((uint32_t)clk_error < clk_error_prev)
              {
                clk_error_prev = (uint32_t)clk_error;
                valid_timing[count].scll = l;
                valid_timing[count].sclh = h;
                s = &valid_timing[count];
              }
            }
          }
        }
      }

      if (s == NULL)
      {
        return 0;
      }

      ret = ((s->presc  & 0x0FU) << 28) |
            ((s->scldel & 0x0FU) << 20) |
            ((s->sdadel & 0x0FU) << 16) |
            ((s->sclh & 0xFFU) << 8) |
            ((s->scll & 0xFFU) << 0);
    }
  }
  return ret;
}
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
