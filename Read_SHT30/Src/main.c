/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SHT30 0x44
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
DMA_HandleTypeDef hdma_i2c1_rx;
DMA_HandleTypeDef hdma_i2c1_tx;

TIM_HandleTypeDef htim7;

UART_HandleTypeDef huart5;
DMA_HandleTypeDef hdma_uart5_rx;
DMA_HandleTypeDef hdma_uart5_tx;

/* USER CODE BEGIN PV */
volatile uint8_t flag=1;
uint8_t soft_reset[2] = {0x30,0x2A};
uint8_t break_cmd[2] = {0x30,0x93};
uint8_t single_shot[2]= {0x2C, 0x06};
uint8_t periodic[2]= {0x22, 0x36};
uint8_t fetch[2] = {0xE0, 0x00};
uint8_t receive[6]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t test[2] = {0x21, 0x25};
//uint8_t send_data_uart[6]={48,49,50,51,52,53}, receive_data_uart[4]={0,0,0,0};
uint8_t Send_UART[11];
uint8_t Received_UART[8];
uint32_t count;
double temperature;
double temperature_pre;
double humidity;
double humidity_pre;
double temperature_set;
uint8_t run;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM7_Init(void);
static void MX_UART5_Init(void);
/* USER CODE BEGIN PFP */
void I2C_ReInit(void)
{
    MX_I2C1_Init();
}
static uint8_t crc8(const uint8_t *data, int len) {
	const uint8_t POLYNOMIAL = 0x31;
  uint8_t crc = 0xFF;
	for (int j = len; j; --j) {
    crc ^= *data++;

    for (int i = 8; i; --i) {
      crc = (crc & 0x80) ? (crc << 1) ^ POLYNOMIAL : (crc << 1);
    }
  }
	return crc;
}

static double calculate_temperature(uint8_t MSB, uint8_t LSB, uint8_t crc)	
{	
	uint8_t data[2] ={MSB, LSB};
	uint8_t crc_calculated = crc8(data,2);
	if (crc == crc_calculated)
	{temperature_pre = temperature;
	return( 175*((double)MSB*256 + (double)LSB)/65535 -45);
	}
	else return(temperature_pre);
}

static double calculate_humidity(uint8_t MSB, uint8_t LSB, uint8_t crc)
{	uint8_t data[2] ={MSB, LSB};
	uint8_t crc_calculated = crc8(data,2);
	if (crc == crc_calculated)
	{humidity_pre = humidity;
	return( 100*((double)MSB*256 + (double)LSB)/65535);
	}
	else return(humidity_pre);
}

void SHT30_initialize()
{
	HAL_Delay(100);
	HAL_I2C_Master_Transmit(&hi2c1, SHT30<<1, break_cmd, 2, 100);
	//HAL_I2C_Master_Transmit_DMA(&hi2c1, SHT30<<1, break_cmd, 2);
	HAL_Delay(100);
	HAL_I2C_Master_Transmit(&hi2c1, SHT30<<1, soft_reset, 2, 100);
	//HAL_I2C_Master_Transmit_DMA(&hi2c1, SHT30<<1, soft_reset, 2);
	HAL_Delay(100);
	HAL_I2C_Master_Transmit(&hi2c1, SHT30<<1, periodic, 2, 100);
	//HAL_I2C_Master_Transmit_DMA(&hi2c1, SHT30<<1, periodic, 2);
	HAL_Delay(100);
}

void SHT30_check_connection(double temperature, double temperature_pre, double humidity, double humidity_pre)
{
	static int times;
	if (temperature == temperature_pre & humidity == humidity_pre)
	{
		times ++;
	}
	else
	{
		times =0;
	}
	if( times > 4)
	{
		I2C_ReInit();
		SHT30_initialize();
	}
}

/*void i2c_deinit(i2c_t *obj)
{
  HAL_NVIC_DisableIRQ(obj->irq);
#if !defined(STM32F0xx) && !defined(STM32L0xx)
  HAL_NVIC_DisableIRQ(obj->irqER);
#endif // !defined(STM32F0xx) && !defined(STM32L0xx)
  HAL_I2C_DeInit(&(obj->handle));
}*/


void Pack_data_to_send_UART(uint8_t *receive, uint8_t ACK_send)
{
	Send_UART[0]=0x53;// STX[1] (character S)
	Send_UART[1]=0x65;// STX[2] (character e)
	if (ACK_send ==1)
	{
		Send_UART[8]=0x2B;//ACK (character +)
	}
	else
	{
	Send_UART[8]=0x2D; //Not ACK (character -)
	}
	Send_UART[9]=0x45;// ETX[1] (character E)
	Send_UART[10]=0x6e;// ETX[2] (character n)
	for (int i=0; i<6; i++)
		{
				Send_UART[i+2] = receive[i];
		}
}

void Process_UART_received(uint8_t Receive_UART[8])
{	
	if(Receive_UART[0]=0x53, Receive_UART[1]=0x65, Receive_UART[6]=0x45, Receive_UART[7]=0x6e)
	{
		if (Receive_UART[2] == 0x54)
		{
				uint8_t data[2] ={Receive_UART[3], Receive_UART[4]};
				if (Receive_UART[5] == crc8(data,2))
				{
					temperature_set = calculate_temperature(Receive_UART[3],Receive_UART[4], Receive_UART[5]);
				}
				else
				{
					return;
				}
		}
		if (Receive_UART[2] == 0x52)
		{
			run =1;
		}
		if (Receive_UART[2] == 0x50)
		{
			run =0;
		}
	}
}

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
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_TIM7_Init();
  MX_UART5_Init();
  /* USER CODE BEGIN 2 */
	//HAL_NVIC_SetPriority(TIM7_IRQn,0 ,1);
	//HAL_NVIC_EnableIRQ(TIM7_IRQn);
	
	SHT30_initialize();
	HAL_TIM_Base_Start_IT(&htim7);
	HAL_UART_Receive_DMA (&huart5, Received_UART, 8);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		Process_UART_received(Received_UART);
		//HAL_I2C_Master_Transmit(&hi2c1, SHT30<<1, fetch, 2, 100);
		HAL_I2C_Master_Transmit_DMA(&hi2c1, SHT30<<1, fetch, 2);
		HAL_Delay(10);
		//HAL_I2C_Master_Receive(&hi2c1, SHT30<<1, receive, 6, 100);
		HAL_I2C_Master_Receive_DMA(&hi2c1, SHT30<<1, receive, 6);
		HAL_Delay(500);
		//HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
		temperature = calculate_temperature(receive[0], receive[1], receive[2]);
		humidity = calculate_humidity(receive[3], receive[4], receive[5]);
		SHT30_check_connection(temperature, temperature_pre, humidity, humidity_pre);
		Pack_data_to_send_UART(receive, 0); //Save in Send_UART[10]
		HAL_UART_Transmit_DMA(&huart5, Send_UART, 11);
		//HAL_UART_Receive_DMA (&huart5, Received_UART, 8);
		while(flag==1);
		flag=1;
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */
	HAL_NVIC_SetPriority(I2C1_EV_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief TIM7 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM7_Init(void)
{

  /* USER CODE BEGIN TIM7_Init 0 */

  /* USER CODE END TIM7_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM7_Init 1 */

  /* USER CODE END TIM7_Init 1 */
  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 8399;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 9999;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM7_Init 2 */
	__HAL_TIM_CLEAR_FLAG(&htim7, TIM_SR_UIF); // Clear first Interrupt
  /* USER CODE END TIM7_Init 2 */

}

/**
  * @brief UART5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART5_Init(void)
{

  /* USER CODE BEGIN UART5_Init 0 */

  /* USER CODE END UART5_Init 0 */

  /* USER CODE BEGIN UART5_Init 1 */

  /* USER CODE END UART5_Init 1 */
  huart5.Instance = UART5;
  huart5.Init.BaudRate = 9600;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART5_Init 2 */

  /* USER CODE END UART5_Init 2 */

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
  /* DMA1_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);
  /* DMA1_Stream7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream7_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream7_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PD12 */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{if (hi2c->Instance==hi2c1.Instance)
	{	
		HAL_DMA_Abort_IT(hi2c->hdmatx);
	}
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{if (hi2c->Instance==hi2c1.Instance)
	{
		HAL_DMA_Abort_IT(hi2c->hdmarx);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==htim7.Instance)
	{
		flag=0;
	__HAL_TIM_CLEAR_FLAG(&htim7, TIM_FLAG_UPDATE);
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
		count++;
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
