/**
  ******************************************************************************
  * @file    UART/UART_WakeUpFromStopUsingFIFO/CM7/Src/main.c
  * @author  MCD Application Team
  * @brief   This example shows how to use UART HAL API to wake up the MCU from
  *          STOP mode using FIFO filling level.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include "CTRMode.h"
#include "ARIA.h"
#include "CAMELLIA.h"
#include "NOEKEON.h"
#include "SEED.h"
#include "SIMON.h"
#include "SPECK.h"
#include "IDEA.h"
#include "PRESENT.h"
#include "HIGHT.h"
#include "GOST.h"
#include "constants.h"
#include "config.h"

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup UART_WakeUpFromStopUsingFIFO
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define HAL_TIMEOUT_VALUE 0xFFFFFFFF
#define countof(a) (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/
/* UART handler declaration */
UART_HandleTypeDef UartHandle;


/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void CPU_CACHE_Enable(void);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/
/* DWT (Data Watchpoint and Trace) registers, only exists on ARM Cortex with a DWT unit */
#define KIN1_DWT_CONTROL             (*((volatile uint32_t*)0xE0001000))
/*!< DWT Control register */
#define KIN1_DWT_CYCCNTENA_BIT       (1UL<<0)
/*!< CYCCNTENA bit in DWT_CONTROL register */
#define KIN1_DWT_CYCCNT              (*((volatile uint32_t*)0xE0001004))
/*!< DWT Cycle Counter register */
#define KIN1_DEMCR                   (*((volatile uint32_t*)0xE000EDFC))
/*!< DEMCR: Debug Exception and Monitor Control Register */
#define KIN1_TRCENA_BIT              (1UL<<24)
/*!< Trace enable bit in DEMCR register */

#define KIN1_InitCycleCounter() \
KIN1_DEMCR |= KIN1_TRCENA_BIT
/*!< TRCENA: Enable trace and debug block DEMCR (Debug Exception and Monitor Control Register */

#define KIN1_ResetCycleCounter() \
KIN1_DWT_CYCCNT = 0
/*!< Reset cycle counter */

#define KIN1_EnableCycleCounter() \
KIN1_DWT_CONTROL |= KIN1_DWT_CYCCNTENA_BIT
/*!< Enable cycle counter */

#define KIN1_DisableCycleCounter() \
KIN1_DWT_CONTROL &= ~KIN1_DWT_CYCCNTENA_BIT
/*!< Disable cycle counter */

#define KIN1_GetCycleCounter() \
KIN1_DWT_CYCCNT
/*!< Read cycle counter register */

uint32_t cycles; /* number of cycles */

int Call_CTR(enum Algorithm algorithm, int SIZE){
	CTRCounter ctrCounter;

	int contText = 0;
	int contNonce = 0;	

	for(int i = 0; i < 8;i++)	
		ctrCounter.Key[i] = KEY[i];

	do{ 
		for (int i = 0; i < SIZE; i++)
		{			
			ctrCounter.text[i] = TEXT_LIST[contText];
			contText++;
		}
		
		for (int i = 0; i < SIZE; i++)
		{			
			ctrCounter.ctrNonce[i] = NONCE_LIST[contNonce]; 
			contNonce++;
		}

		if (CTRMode_main(ctrCounter, algorithm, SIZE, contText) == 1){
			 return 1;
		}		

	}while (contText < 202);	
  return 0;

}



/**
  * @brief  Main program
  * @param  None
  * @retval None
  */



int main(void)
{
  int32_t timeout;
  /* Enable the CPU Cache */
  CPU_CACHE_Enable();
  timeout = 0xFFFF;
  while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) != RESET) && (timeout-- > 0));
  if ( timeout < 0 )
  {
    Error_Handler();
  }
  /* STM32H7xx HAL library initialization:
       - Systick timer is configured by default as source of time base, but user
         can eventually implement his proper time base source (a general purpose
         timer for example or other time source), keeping in mind that Time base
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();



  /* Configure the system clock to 400 MHz */
  SystemClock_Config();

  /* Configure leds */
  BSP_LED_Init(LED2);

  /*##########################################################################*/
  /*##-1- Configure the UART peripheral ######################################*/
  /*##########################################################################*/

  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  /* UART configured as follows:
      - Word Length = 8 Bits (7 data bit + 1 parity bit) :
                      BE CAREFUL : Program 7 data bits + 1 parity bit in PC HyperTerminal
      - Stop Bit    = One Stop bit
      - Parity      = Odd
      - BaudRate    = 9600 baud
      - Hardware flow control disabled (RTS and CTS signals) */
  UartHandle.Instance             = USARTx;
  UartHandle.Init.BaudRate        = 115200;
  UartHandle.Init.WordLength      = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits        = UART_STOPBITS_1;
  UartHandle.Init.Parity          = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl       = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode            = UART_MODE_TX_RX;
  UartHandle.Init.OverSampling    = UART_OVERSAMPLING_16;

  if(HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Set the RXFIFO threshold */
  HAL_UARTEx_SetRxFifoThreshold(&UartHandle, UART_RXFIFO_THRESHOLD_1_4);

  /* Enable the FIFO mode */
  HAL_UARTEx_EnableFifoMode(&UartHandle);

  /*##-2- Configure UART peripheral for reception process ##########*/  

  /* Enable MCU wakeup by UART */
  HAL_UARTEx_EnableStopMode(&UartHandle);

  /* Enable the UART RX FIFO full interrupt */
  __HAL_UART_ENABLE_IT(&UartHandle, UART_IT_RXFF);

  /* Enable the UART wakeup from stop mode interrupt */
  __HAL_UART_ENABLE_IT(&UartHandle, UART_IT_WUF);

  /*##-3- Start the transmission process #############*/

  


  KIN1_InitCycleCounter(); /* enable DWT hardware */

	KIN1_ResetCycleCounter(); /* reset cycle counter */
  KIN1_EnableCycleCounter(); /* start counting */
  /* Infinite loop */
  while (1)
  {
    
     int ret;
    uint32_t tick,tock,spent,acc;
    uint8_t ret_string[32];
    
    for(int i = 0; i < 20; i++)
    {
    	tick = KIN1_GetCycleCounter();
      ret = Call_CTR(KEYSIZE, TEXT_SIZE);
      tock = KIN1_GetCycleCounter();
      if(ret == 1)
        Error_Handler();
      spent = tock - tick;
      acc += spent;
    }
    sprintf(ret_string,"%lu clock cycles\n\r",acc/20);
    acc=0;
    HAL_UART_Transmit(&UartHandle, (uint8_t*)ret_string, strlen(ret_string), 1000);
	  HAL_Delay(1000);

  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE BYPASS)
  *            SYSCLK(Hz)                     = 400000000 (CPU Clock)
  *            HCLK(Hz)                       = 200000000 (AXI and AHBs Clock)
  *            AHB Prescaler                  = 2
  *            D1 APB3 Prescaler              = 2 (APB3 Clock  100MHz)
  *            D2 APB1 Prescaler              = 2 (APB1 Clock  100MHz)
  *            D2 APB2 Prescaler              = 2 (APB2 Clock  100MHz)
  *            D3 APB4 Prescaler              = 2 (APB4 Clock  100MHz)
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 4
  *            PLL_N                          = 400
  *            PLL_P                          = 2
  *            PLL_Q                          = 4
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /*!< Supply configuration update enable */
  HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);
  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 400;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;

  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_1;
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    Error_Handler();
  }

/* Select PLL as system clock source and configure  bus clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_D1PCLK1 | RCC_CLOCKTYPE_PCLK1 | \
                                 RCC_CLOCKTYPE_PCLK2  | RCC_CLOCKTYPE_D3PCLK1);

  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);
  if(ret != HAL_OK)
  {
    Error_Handler();
  }

}

/**
* @brief  CPU L1-Cache enable.
* @param  None
* @retval None
*/
static void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED3 on */
  BSP_LED_On(LED3);
  while(1) { ; }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n\r", file, line) */

  /* Infinite loop */
  while(1) { ; }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/