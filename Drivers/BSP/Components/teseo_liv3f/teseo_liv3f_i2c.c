/**
  *******************************************************************************
  * @file    teseo_liv3f_i2c.c
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

/* Includes ------------------------------------------------------------------*/
#include "teseo_liv3f_i2c.h"

/* Private defines -----------------------------------------------------------*/

/*
 * Constant for I2C read
 */
#define I2C_READ	(0)
/*
 * Constant for I2C write
 */
#define I2C_WRITE	(1)

/*
 * Size of I2C buffer
 */
#define I2C_LOCAL_BUF	(16)

/* Private types -------------------------------------------------------------*/

/*
 * Enumeration structure that contains the I2C task states
 */
typedef enum
{
  IDLE,
  RUNNING,
  WAITING
} Task_Command_StateTypeDef;

/*
 * Enumeration structure that contains the I2C FSM states
 */
typedef enum
{
  i2c_fsm_discard,
  i2c_fsm_synch
} Teseo_I2C_FsmTypeDef;

/*
 * I2C private data handler
 */
typedef struct
{
  volatile Task_Command_StateTypeDef cmd;
  volatile Task_Command_StateTypeDef state;
  TESEO_LIV3F_Msg_t *wr_msg;
  volatile Teseo_I2C_FsmTypeDef fsm_state;
  volatile Teseo_I2C_FsmTypeDef fsm_next_state;
  volatile uint32_t timeout;
  volatile int8_t nr_msg;
  volatile int8_t fsm_restart;

  uint8_t buf[I2C_LOCAL_BUF];
  
  TESEO_LIV3F_ctx_t *pCtx;
} Teseo_I2C_DataTypeDef;

/*
 * Instance of I2C private data handler
 */
static Teseo_I2C_DataTypeDef Teseo_I2C_Data;


/* Private functions ---------------------------------------------------------*/
static void i2c_fsm(uint8_t temp);

/*
 * I2C state machine function (param temp is char read on the I2C bus
 */
static void i2c_fsm(uint8_t temp)
{
  TESEO_LIV3F_ctx_t *pCtx = Teseo_I2C_Data.pCtx;
  uint32_t offset = 1;

  Teseo_I2C_Data.fsm_restart = 1;

  while(Teseo_I2C_Data.fsm_restart == 1)
  {
    Teseo_I2C_Data.fsm_state = Teseo_I2C_Data.fsm_next_state;
    
    switch (Teseo_I2C_Data.fsm_state)
    {
      
    case i2c_fsm_discard:
      Teseo_I2C_Data.fsm_restart = 0;
      Teseo_I2C_Data.fsm_next_state = i2c_fsm_discard;
      //PRINT_DBG(".");
      
      if (temp != (uint8_t)'$') {
        break;
      }
      
      if (Teseo_I2C_Data.wr_msg == NULL)
      {
        Teseo_I2C_Data.wr_msg = teseo_queue_claim_wr_buffer(pCtx->pQueue);
      }
      
      if (Teseo_I2C_Data.wr_msg == NULL)
      {
        break;
      }
      
      Teseo_I2C_Data.wr_msg->len = 0;
      Teseo_I2C_Data.wr_msg->buf[Teseo_I2C_Data.wr_msg->len] = temp; /* save '$' */
      Teseo_I2C_Data.wr_msg->len++;

      Teseo_I2C_Data.fsm_next_state = i2c_fsm_synch;
      PRINT_DBG("I2C buffer synch\n\r");
      break;
      
    case i2c_fsm_synch:
      Teseo_I2C_Data.fsm_restart = 0;
      Teseo_I2C_Data.wr_msg->buf[Teseo_I2C_Data.wr_msg->len] = temp;
      Teseo_I2C_Data.wr_msg->len++;
      
      if (Teseo_I2C_Data.wr_msg->buf[Teseo_I2C_Data.wr_msg->len-offset] == (uint8_t)'$')
      {
        --Teseo_I2C_Data.wr_msg->len;
        Teseo_I2C_Data.wr_msg->buf[Teseo_I2C_Data.wr_msg->len] = (uint8_t)'\0';
        teseo_queue_release_wr_buffer(pCtx->pQueue, Teseo_I2C_Data.wr_msg);
        if (Teseo_I2C_Data.nr_msg != -1)
        {
          --Teseo_I2C_Data.nr_msg;
          if (Teseo_I2C_Data.nr_msg == 0)
          {
            Teseo_I2C_Data.cmd = IDLE;
            break;
          }
        }
        PRINT_DBG("I2C buffer released\n\r");
        Teseo_I2C_Data.wr_msg = NULL;
        Teseo_I2C_Data.fsm_next_state = i2c_fsm_discard;

        Teseo_I2C_Data.fsm_restart = 1; // check if we can resynch the new sentence
        break;
      }
#if 0
      //PRINT_DBG("+");
      if (Teseo_I2C_Data.wr_msg->len == (uint32_t)MAX_MSG_BUF)
      {
        PRINT_DBG("MESSAGE TOO LONG\n\r");
        /* it seems we lost some char and the sentence is too much long...
        * reset the msg->len and discand all teh buffer and wait a new '$' sentence
        */
        Teseo_I2C_Data.fsm_next_state = i2c_fsm_discard;
        Teseo_I2C_Data.wr_msg->len = 0;

        Teseo_I2C_Data.fsm_restart = 1;
        break;
      }
#endif
      Teseo_I2C_Data.fsm_next_state = i2c_fsm_synch;
      break;
      
    default:
      Teseo_I2C_Data.fsm_restart = 0;
      break;
    }
  }
}

/* Exported functions --------------------------------------------------------*/
void teseo_i2c_rx_callback(Teseo_I2C_CB_CallerTypedef c)
{
  TESEO_LIV3F_ctx_t *pCtx = Teseo_I2C_Data.pCtx;
  int32_t i;

  if (Teseo_I2C_Data.cmd == IDLE)
  {
    /* turn-off I2C-ISR... */
    Teseo_I2C_Data.state = IDLE;
    return;
  }

  if (c == _i2c_abort)
  {
    PRINT_DBG("Teseo I2C Abort on ISR\n\r");
    /* turn-off I2C-ISR... */
    Teseo_I2C_Data.state = IDLE;
    return;
  }
  
  if (c == _i2c_error)
  {
    PRINT_DBG("Teseo I2C Error on ISR\n\r");
    /* suspend I2C-ISR... and try to resume later...*/
#define TIMEOUT_DELAY	(uint32_t)100
    Teseo_I2C_Data.timeout = pCtx->GetTick(pCtx->Handle) + TIMEOUT_DELAY;
    Teseo_I2C_Data.state = WAITING; /* turn-off */
    return;
  }
  
  PRINT_DBG("I2C ISR\n\r");
  for (i = 0; i < I2C_LOCAL_BUF; ++i)
  {
    if (Teseo_I2C_Data.buf[i] != (uint8_t)0xff)
    {
      i2c_fsm(Teseo_I2C_Data.buf[i]);
    }
  } 
  
  if (Teseo_I2C_Data.buf[I2C_LOCAL_BUF-1] == (uint8_t)0xff)
  {
    Teseo_I2C_Data.timeout = pCtx->GetTick(pCtx->Handle) + TIMEOUT_DELAY;
    Teseo_I2C_Data.state = WAITING;
    return;
  }

  Teseo_I2C_Data.timeout = pCtx->GetTick(pCtx->Handle);
  Teseo_I2C_Data.state = RUNNING;
  pCtx->Receive(pCtx->Handle, (uint8_t *)Teseo_I2C_Data.buf, I2C_LOCAL_BUF);
}

void teseo_i2c_onoff(TESEO_LIV3F_ctx_t *pCtx, uint8_t enable, int8_t nr_msg)
{
  Teseo_I2C_Data.pCtx = pCtx;

  if ((Teseo_I2C_Data.state == RUNNING) && (enable == 1U))
  {
    return;
  }
  if ((Teseo_I2C_Data.state == IDLE) && (enable == 0U))
  {
    return;
  }
  
  if (enable == 1U)
  {
    PRINT_DBG("I2C running\n\r");
    Teseo_I2C_Data.fsm_next_state = i2c_fsm_discard;
    Teseo_I2C_Data.fsm_restart = 1;
    Teseo_I2C_Data.nr_msg = nr_msg;
    Teseo_I2C_Data.timeout = pCtx->GetTick(pCtx->Handle);
    Teseo_I2C_Data.wr_msg = NULL;
    Teseo_I2C_Data.cmd = RUNNING;
    pCtx->Receive(pCtx->Handle, (uint8_t *)Teseo_I2C_Data.buf, I2C_LOCAL_BUF);
    
    return;
  }

  Teseo_I2C_Data.cmd = IDLE;
  while (Teseo_I2C_Data.state == RUNNING) {};
  /*
   * This function returns when the I2C-FiniteStateMachine is no more running...
   */
}

void teseo_i2c_background_process(void)
{
  const TESEO_LIV3F_ctx_t *pCtx = Teseo_I2C_Data.pCtx;

  if (Teseo_I2C_Data.cmd == IDLE)
  {
    return;
  }

  if (pCtx->GetTick(pCtx->Handle) > Teseo_I2C_Data.timeout)
  {
    //PRINT_DBG("I2C Idle running\n\r");
    Teseo_I2C_Data.state = RUNNING;
    pCtx->Receive(pCtx->Handle, (uint8_t *)Teseo_I2C_Data.buf, I2C_LOCAL_BUF);
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
