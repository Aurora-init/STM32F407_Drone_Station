/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "COMMON/bsp_common.h"	//STD基本宏包
#include "COMMON/bsp_device.h"	//外设模块化宏包
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

//dht11变量
DHT11_Data_TypeDef DHT11_Data;

////TCP透传数据变量
//IPD_RX ipd_rx;

//上传数据的buf
char PUB_BUF[256];
const char *devSubTopic[] = {"subTOPIC"};
const char 	devPubTopic[] = 	"pubTOPIC";
uint8_t ESP8266_INIT_OK = 0;//esp8266初始化完成标志


//串口3接收数据变量
uint8_t rx_buffer3[100];

uint8_t usart_counter = 0;
uint8_t usart_IT_flag = 0;

char lcd_std[100];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	//发送间隔变量
	unsigned short timeCount = 0;	
	//平台返回数据变量
	unsigned char *dataPtr = NULL;
	//LCD屏幕显示变量
	
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FSMC_Init();
  MX_TIM5_Init();
  MX_USART6_UART_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_UART4_Init();
  MX_UART5_Init();
  MX_TIM11_Init();
  /* USER CODE BEGIN 2 */
	
//	MX_GPIO_Init();						//DHT11温湿度模块与BEEP蜂鸣器等
//  MX_FSMC_Init();						//TFTLCD屏幕驱动
//  MX_TIM5_Init();						//定时器延时：	void bsp_delay_us(uint16_t us);
//  MX_USART6_UART_Init();		//INFO串口			
//  MX_USART1_UART_Init();		//WIFI模块专用
//	MX_USART3_UART_Init();		//RFRMUM读卡器
//  MX_UART4_Init();					//K210转接板
//  MX_UART5_Init();					//TM512测温模块

	//1.LCD初始化部分//竖屏显示
	LCD_Init();				LCD_DisplayOn();				LCD_Display_Dir(0);					user_main_info("LCD初始化成功\t[OK]\r\n"); 

	//2.ESP01S连接WIFI等初始化操作
	ESP8266_Init();				user_main_info("ESP01S初始化成功\t[OK]\r\n");		
	
	//3.接入OneNET//接入ONENET成功，蜂鸣器响起提示一下
	while(OneNet_DevLink()){HAL_Delay(500);}				Beep_On;				HAL_Delay(100);				Beep_Off;				user_main_info("连接ONENET成功\t[OK]\r\n");				
//	sprintf((char*)lcd_std," ESP01S LINK ONENET SUCCESS!\r\n");				LCD_ShowString(66,186,500,24,24,(uint8_t*)lcd_std);
			
	//4.订阅话题
	RPRMUM_Init();
	
	OneNet_Subscribe(devSubTopic, 1);
	
	HAL_UART_Receive_IT(&huart3,(uint8_t *)rx_buffer3,1);				user_main_info("串口3接收中断开启\t[OK]\r\n");
	
//	HAL_TIM_Base_Start_IT(&htim11);				HAL_TIM_PWM_Start(&htim11,TIM_CHANNEL_1);				MOTOR_Init();
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {	
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
		
		//检查数据回传
		RPRMUM_DataCheck();
		
		//发送单次读取命令
		RPRMUM_Comm_ReadCard();
		
		HAL_UART_Receive_IT(&huart3,(uint8_t *)rx_buffer3,1);
		//5.将数据流上传到ONENET
		if(++timeCount >= 100)				//2500ms / 25 = 100 发送间隔2500ms
		{
			
			//MQTT通信话题内容封装
			sprintf(PUB_BUF,"{\"Hum\":%d.%d,\"Temp\":%d.%d}",DHT11_Data.humi_int,DHT11_Data.humi_deci,DHT11_Data.temp_int,DHT11_Data.temp_deci);
			
			//将话题内容发布到"pubTOPIC"话题中
			OneNet_Publish(devPubTopic, PUB_BUF);
			
			//打包数据包发送到ONENET云平台，一次只能打包一个数据流发送，若要发得更快就调整发送时间间隔
			OneNet_SendData();
			
			//时间间隔计时请0
			timeCount = 0;
			
			//数据包清空
			ESP8266_Clear();

		}
		
		//检测平台是否有响应
		dataPtr = ESP8266_GetIPD(3);
		if(dataPtr != NULL)
		OneNet_RevPro(dataPtr);
		HAL_Delay(10);
  }
	
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
//这里就拿来放一些外设扫描吧

//串口接收中断回调函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	
	//串口3接收回调函数
	if(huart->Instance == USART3)
	{
		usart_counter++;
		//
		if(usart_counter >= 100&&usart_IT_flag == 1)
		{
			HAL_GPIO_WritePin(GPIOF, LED0_Pin, GPIO_PIN_SET);
			usart_counter = 0;
			usart_IT_flag = 0;
		}
		else if(usart_counter >= 100&&usart_IT_flag == 0)
		{
			HAL_GPIO_WritePin(GPIOF, LED0_Pin, GPIO_PIN_RESET);
			usart_counter = 0;
			usart_IT_flag = 1;
		}
	}
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
