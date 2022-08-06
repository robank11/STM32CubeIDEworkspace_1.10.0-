/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    rtc.c
  * @brief   This file provides code for the configuration
  *          of the RTC instances.
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
#include "rtc.h"

/* USER CODE BEGIN 0 */
#include	<stdio.h>
#include "oled.h"

uint16_t	triggerCntA=0;	//闹钟A触发次数
uint16_t	triggerCntB=0;	//闹钟B触发次数
/* USER CODE END 0 */

RTC_HandleTypeDef hrtc;

/* RTC init function */
void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
  RTC_AlarmTypeDef sAlarm = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_WAKEUP;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_LOW;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x7;
  sTime.Minutes = 0x15;
  sTime.Seconds = 0x10;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_SATURDAY;
  sDate.Month = RTC_MONTH_MARCH;
  sDate.Date = 0x16;
  sDate.Year = 0x19;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable the Alarm A
  */
  sAlarm.AlarmTime.Hours = 0x8;
  sAlarm.AlarmTime.Minutes = 0x16;
  sAlarm.AlarmTime.Seconds = 0x5;
  sAlarm.AlarmTime.SubSeconds = 0x0;
  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  sAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY|RTC_ALARMMASK_HOURS;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmDateWeekDay = 0x3;
  sAlarm.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable the Alarm B
  */
  sAlarm.AlarmTime.Hours = 0x10;
  sAlarm.AlarmTime.Minutes = 0x20;
  sAlarm.AlarmTime.Seconds = 0x30;
  sAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY|RTC_ALARMMASK_HOURS
                              |RTC_ALARMMASK_MINUTES;
  sAlarm.AlarmDateWeekDay = 0x1;
  sAlarm.Alarm = RTC_ALARM_B;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable the WakeUp
  */
  if (HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 0, RTC_WAKEUPCLOCK_CK_SPRE_16BITS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

void HAL_RTC_MspInit(RTC_HandleTypeDef* rtcHandle)
{

  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspInit 0 */

  /* USER CODE END RTC_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* RTC clock enable */
    __HAL_RCC_RTC_ENABLE();

    /* RTC interrupt Init */
    HAL_NVIC_SetPriority(RTC_WKUP_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);
    HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 1, 1);
    HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
  /* USER CODE BEGIN RTC_MspInit 1 */

  /* USER CODE END RTC_MspInit 1 */
  }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspDeInit 0 */

  /* USER CODE END RTC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_RTC_DISABLE();

    /* RTC interrupt Deinit */
    HAL_NVIC_DisableIRQ(RTC_WKUP_IRQn);
    HAL_NVIC_DisableIRQ(RTC_Alarm_IRQn);
  /* USER CODE BEGIN RTC_MspDeInit 1 */

  /* USER CODE END RTC_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;

	if (HAL_RTC_GetTime(hrtc, &sTime,  RTC_FORMAT_BIN) == HAL_OK)
	{
		HAL_RTC_GetDate(hrtc, &sDate,  RTC_FORMAT_BIN);
		//调用HAL_RTC_GetTime()之后必须调用HAL_RTC_GetDate()以解锁数据，才能连续更新日期和时间
		//调用HAL_RTC_GetTime()时会将日历影子寄存器的当前值锁定，直到日期数据被读出。
		//		所以，即使不使用日期数据，也需要调用HAL_RTC_GetDate()读取
		//		  * @note You must call HAL_RTC_GetDate() after HAL_RTC_GetTime() to unlock the values
		//		  *        in the higher-order calendar shadow registers to ensure consistency between the time and date values.
		//		  *        Reading RTC current time locks the values in calendar shadow registers until current date is read.
		//uint16_t	yPos=50, xPos=20;
		//显示日期  年-月-日
		char str[40];
		sprintf(str,"RTC Date= %4d-%2d-%2d",2000+sDate.Year,sDate.Month,sDate.Date);
		//LCD_ShowStr(xPos,yPos, (uint8_t*)str);
		OLED_ShowNum(0,0,2000+sDate.Year,5,12);
		OLED_ShowNum(0,1,sDate.Month,5,12);
		OLED_ShowNum(0,2,sDate.Date,5,12);
//		LCD_ShowUint(xPos, yPos, 2000+sDate.Year);
//
//		LCD_ShowChar(LCD_CurX, yPos,	'-',0);
//
//		LCD_ShowUintX0(LCD_CurX, yPos,	sDate.Month, 2);
//
//		LCD_ShowChar(LCD_CurX, yPos,	'-',0);
//
//		LCD_ShowUintX0(LCD_CurX, yPos,	sDate.Date, 2);

		//显示 时间  hh:mm:ss
		//yPos=yPos+LCD_SP15;
		sprintf(str,"RTC Time= %2d:%2d:%2d",sTime.Hours,sTime.Minutes,sTime.Seconds);
		//LCD_ShowStr(xPos,yPos, (uint8_t*)str);
		OLED_ShowNum(0,3,sTime.Hours,5,12);
		OLED_ShowNum(0,4,sTime.Minutes,5,12);
		OLED_ShowNum(0,5,sTime.Seconds,5,12);
//		LCD_ShowUintX0(xPos,yPos,sTime.Hours,2);
//
//		LCD_ShowChar(LCD_CurX, yPos, ':', 0);
//
//		LCD_ShowUintX0(LCD_CurX, yPos, sTime.Minutes,2);
//
//		LCD_ShowChar(LCD_CurX, yPos, ':', 0);
//
//		LCD_ShowUintX0(LCD_CurX, yPos, sTime.Seconds,2);
	}

}


///*  闹钟A中断的回调函�?  */
//void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
//{
//	uint16_t	yPos=120;
//
//	char infoA[]="Alarm A(xx:16:05) trigger: ";
//	LCD_ShowStr(0,yPos, (uint8_t *)infoA);
//
//	triggerCntA++;	//闹钟A触发次数加
//	//LCD_ShowUint(LCD_CurX,yPos,triggerCntA);
//}
//
///*   闹钟B中断的回调函数  */
//void HAL_RTCEx_AlarmBEventCallback(RTC_HandleTypeDef *hrtc)
//{
//	uint16_t	yPos=150;
//
//	char infoB[]="Alarm B(xx:xx:30) trigger: ";
//	LCD_ShowStr(0,yPos,(uint8_t *)infoB);
//
//	triggerCntB++;		//闹钟B触发次数加1
//	LCD_ShowUint(LCD_CurX, yPos, triggerCntB);
//}
/* USER CODE END 1 */
