/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#include "tim.h"
#include	<string.h>	//用到函数strlen()
uint8_t	proBuffer[10]="#S45;\n";		//用于处理数据, #H12; #M23; #S43;
uint8_t rxBuffer[10]="#H12;\n";		//接收缓存数据, #H12; #M23; #S43;
uint8_t	rxCompleted=RESET;	//HAL_UART_Receive_IT()接收是否完成
uint8_t	isUploadTime=1;	//是否上传时间数据
extern prescalerWidth;
extern periodWidth;
extern pulseWidth;
/* USER CODE END 0 */

UART_HandleTypeDef huart1;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART1)
	{
		//接收到固定长度数据后使能UART_IT_IDLE中断，在UART_IT_IDLE中断里再次接收
		rxCompleted=SET;	//接收完成
		for(uint16_t i=0;i<RX_CMD_LEN;i++)
			proBuffer[i]=rxBuffer[i];
		__HAL_UART_ENABLE_IT(huart, UART_IT_IDLE); //接收到数据后才开启 IDLE中断
	}
}

void	on_UART_IDLE(UART_HandleTypeDef *huart)		//在串口IDLE时处理
{
	if (__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) == RESET)	//IDLE中断挂起标志位是否置位
		return;

	__HAL_UART_CLEAR_IDLEFLAG(huart); 	//清除IDLE挂起标志
	__HAL_UART_DISABLE_IT(huart, UART_IT_IDLE); 	//禁止IDLE事件中断
	if (rxCompleted)	//接收到1条指令
	{
		HAL_UART_Transmit(huart,proBuffer, strlen(proBuffer), 200); //返回接收到的指令字符�?
		HAL_Delay(10);  //需要适当延时，否则updateRTCTime()函数处理可能出错
		updateRTCTime();  //更新RTC时间
//		LCD_ShowStr(30,170, (uint8_t *)proBuffer);  //显示接收到的指令字符串

		rxCompleted=RESET;
		HAL_UART_Receive_IT(huart, rxBuffer, RX_CMD_LEN);	//再次启动串口接收

	}

}
void updateRTCTime()	//根据串口接收的指令字符串进行处理
{
	if (proBuffer[0] != '#')	//非有效指�???
		return;

	uint16_t	timeSection=proBuffer[1];	//类型字符
	uint16_t	tmp1000=proBuffer[2]-0x30;	//十位�???
	uint16_t	tmp100=proBuffer[3]-0x30;	//十位�???
	uint16_t	tmp10=proBuffer[4]-0x30;	//十位�???
	uint16_t	tmp1 =proBuffer[5]-0x30;	//个位�???
	uint16_t	val=1000*tmp1000+100*tmp100+10*tmp10+tmp1;
	if (timeSection=='U')	//是否上传RTC时间
	{
		isUploadTime=val;
		return;
	}

//	RTC_TimeTypeDef sTime;
//	RTC_DateTypeDef sDate;


		if (timeSection=='Q'){
			prescalerWidth=val;
			__HAL_TIM_SET_PRESCALER(&htim11,prescalerWidth);
		}
		else if (timeSection=='W'){
			periodWidth=val;
			__HAL_TIM_SET_AUTORELOAD(&htim11,periodWidth);
		}
		else if (timeSection=='E'){
			pulseWidth=val;
		__HAL_TIM_SET_COMPARE(&htim11,TIM_CHANNEL_1,pulseWidth);
		}

}
/* USER CODE END 1 */
