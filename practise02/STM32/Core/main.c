/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "fatfs.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ssd1306.h"
#include "fonts.h"
#include "usbd_cdc_if.h"
#include "stdio.h"
#include "stdbool.h"
#include "wifiwork.h"
#include "si7021.h"
#include "bmp280.h"
#include "ds1307_for_stm32_hal.h"
extern uint8_t UserRxBufferFS[APP_RX_DATA_SIZE]; //Отправка данных
extern uint8_t UserTxBufferFS[APP_TX_DATA_SIZE]; //Прием данных
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
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
char strRX[21];
char bufferRX[21];
char buffOut[10];
USBD_HandleTypeDef hUsbDeviceFS;
BMP280_HandleTypedef bmp280;
uint8_t date;
uint8_t month;
uint16_t year;
uint8_t dow;
uint8_t hour;
uint8_t minute;
uint8_t second;
int temp2, humidearth, gasq, psi;
int needtemp, needhumearth, maxco2, needhum;
float pressure, temperature, humidity;
uint32_t humidair = 0;
uint32_t temp1 = 0;
uint8_t wifiint = 0;
uint32_t adcval[2];
int nowscreen = 0;
uint16_t timer_val = 0;
uint32_t time_work = 0;
uint16_t pumptime;
uint16_t pumpdelay;
uint8_t dispFlag = 0;
uint8_t autowork = 0;
uint8_t pumpworkflag = 0;
uint8_t ispumpon = 0;
uint8_t iswindowopen = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C2_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM4_Init(void);
/* USER CODE BEGIN PFP */
void usbwork();
int waittx();
void polsens();
int wifiinit();
void displaywork();
void autoWork();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
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
	MX_ADC1_Init();
	MX_I2C1_Init();
	MX_I2C2_Init();
	MX_SPI1_Init();
	MX_FATFS_Init();
	MX_USB_DEVICE_Init();
	MX_TIM2_Init();
	MX_TIM3_Init();
	MX_USART2_UART_Init();
	MX_TIM4_Init();
	/* USER CODE BEGIN 2 */
	SSD1306_Init();
	SSD1306_Clear();
	bmp280_init_default_params(&bmp280.params);
	bmp280.addr = BMP280_I2C_ADDRESS_0;
	bmp280.i2c = &hi2c2;
	bmp280_init(&bmp280, &bmp280.params);
	DS1307_Init(&hi2c2);
	HAL_ADCEx_Calibration_Start(&hadc1);
	HAL_TIM_Base_Start_IT(&htim4);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	htim2.Instance->CCR1 = 90;
	htim3.Instance->CCR1 = 90;
	//htim2.Instance->CCR1=50;
	//DS1307_SetTimeZone(+3, 00);
	//DS1307_SetDate(13);
	//DS1307_SetMonth(7);
	//DS1307_SetYear(2021);
	//DS1307_SetDayOfWeek(4);
	//DS1307_SetHour(4);
	//DS1307_SetMinute(17);
	//DS1307_SetSecond(20);
	needtemp = 29;
	needhumearth = 50;
	maxco2 = 1000;
	needhum = 60;
	autowork = 1;
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		polsens();
		autoWork();
		usbwork();
		displaywork();
		//htim2.Instance->CCR1=90;
		//htim3.Instance->CCR1=90;
		//HAL_Delay(500);
		//htim2.Instance->CCR1=40;
		//htim3.Instance->CCR1=40;
		//HAL_Delay(500);
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC | RCC_PERIPHCLK_USB;
	PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
	PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief ADC1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_ADC1_Init(void) {

	/* USER CODE BEGIN ADC1_Init 0 */

	/* USER CODE END ADC1_Init 0 */

	ADC_ChannelConfTypeDef sConfig = { 0 };

	/* USER CODE BEGIN ADC1_Init 1 */

	/* USER CODE END ADC1_Init 1 */
	/** Common config
	 */
	hadc1.Instance = ADC1;
	hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 2;
	if (HAL_ADC_Init(&hadc1) != HAL_OK) {
		Error_Handler();
	}
	/** Configure Regular Channel
	 */
	sConfig.Channel = ADC_CHANNEL_8;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
		Error_Handler();
	}
	/** Configure Regular Channel
	 */
	sConfig.Channel = ADC_CHANNEL_9;
	sConfig.Rank = ADC_REGULAR_RANK_2;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN ADC1_Init 2 */

	/* USER CODE END ADC1_Init 2 */

}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void) {

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
	if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN I2C1_Init 2 */

	/* USER CODE END I2C1_Init 2 */

}

/**
 * @brief I2C2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C2_Init(void) {

	/* USER CODE BEGIN I2C2_Init 0 */

	/* USER CODE END I2C2_Init 0 */

	/* USER CODE BEGIN I2C2_Init 1 */

	/* USER CODE END I2C2_Init 1 */
	hi2c2.Instance = I2C2;
	hi2c2.Init.ClockSpeed = 100000;
	hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c2.Init.OwnAddress1 = 0;
	hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c2.Init.OwnAddress2 = 0;
	hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c2) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN I2C2_Init 2 */

	/* USER CODE END I2C2_Init 2 */

}

/**
 * @brief SPI1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI1_Init(void) {

	/* USER CODE BEGIN SPI1_Init 0 */

	/* USER CODE END SPI1_Init 0 */

	/* USER CODE BEGIN SPI1_Init 1 */

	/* USER CODE END SPI1_Init 1 */
	/* SPI1 parameter configuration*/
	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi1.Init.NSS = SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 10;
	if (HAL_SPI_Init(&hspi1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN SPI1_Init 2 */

	/* USER CODE END SPI1_Init 2 */

}

/**
 * @brief TIM2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM2_Init(void) {

	/* USER CODE BEGIN TIM2_Init 0 */

	/* USER CODE END TIM2_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };
	TIM_OC_InitTypeDef sConfigOC = { 0 };

	/* USER CODE BEGIN TIM2_Init 1 */

	/* USER CODE END TIM2_Init 1 */
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 1440 - 1;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 1000;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim2) != HAL_OK) {
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_TIM_PWM_Init(&htim2) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1)
			!= HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM2_Init 2 */

	/* USER CODE END TIM2_Init 2 */
	HAL_TIM_MspPostInit(&htim2);

}

/**
 * @brief TIM3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM3_Init(void) {

	/* USER CODE BEGIN TIM3_Init 0 */

	/* USER CODE END TIM3_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };
	TIM_OC_InitTypeDef sConfigOC = { 0 };

	/* USER CODE BEGIN TIM3_Init 1 */

	/* USER CODE END TIM3_Init 1 */
	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 1440 - 1;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = 1000;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim3) != HAL_OK) {
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_TIM_PWM_Init(&htim3) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1)
			!= HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM3_Init 2 */

	/* USER CODE END TIM3_Init 2 */
	HAL_TIM_MspPostInit(&htim3);

}

/**
 * @brief TIM4 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM4_Init(void) {

	/* USER CODE BEGIN TIM4_Init 0 */

	/* USER CODE END TIM4_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };

	/* USER CODE BEGIN TIM4_Init 1 */

	/* USER CODE END TIM4_Init 1 */
	htim4.Instance = TIM4;
	htim4.Init.Prescaler = (72 * 2 * 10) - 1;
	htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim4.Init.Period = 50000;
	htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim4) != HAL_OK) {
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM4_Init 2 */

	/* USER CODE END TIM4_Init 2 */

}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void) {

	/* USER CODE BEGIN USART2_Init 0 */

	/* USER CODE END USART2_Init 0 */

	/* USER CODE BEGIN USART2_Init 1 */

	/* USER CODE END USART2_Init 1 */
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart2) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART2_Init 2 */

	/* USER CODE END USART2_Init 2 */

}

/**
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void) {

	/* DMA controller clock enable */
	__HAL_RCC_DMA1_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA1_Channel1_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13 | GPIO_PIN_15 | GPIO_PIN_8,
			GPIO_PIN_RESET);

	/*Configure GPIO pins : PB13 PB15 PB8 */
	GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_15 | GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void usbwork() {
	if (strcmp(strRX, "") != 0) //Если входящее сообщение не пустое
			{
		strcpy(bufferRX, strRX);
		if (strcmp(strRX, "getpar") == 0) //Возврат параметров
				{
			sprintf((char*) UserTxBufferFS, "paramds"); //Отправка
			while (waittx())
				; //Ожидание свободы USB порта
			CDC_Transmit_FS(UserTxBufferFS, strlen(UserTxBufferFS)); //Отправка данных на порт
			sprintf((char*) UserTxBufferFS, "temp1:%d", temp1); //Заполнение буфера
			while (waittx())
				;
			CDC_Transmit_FS(UserTxBufferFS, strlen(UserTxBufferFS));
			sprintf((char*) UserTxBufferFS, "temp2:%d", temp2); //Отправка
			while (waittx())
				;
			CDC_Transmit_FS(UserTxBufferFS, strlen(UserTxBufferFS));
			sprintf((char*) UserTxBufferFS, "humidair:%d", humidair); //Отправка
			while (waittx())
				;
			CDC_Transmit_FS(UserTxBufferFS, strlen(UserTxBufferFS));
			sprintf((char*) UserTxBufferFS, "humidearth:%d", humidearth); //Отправка
			while (waittx())
				;
			CDC_Transmit_FS(UserTxBufferFS, strlen(UserTxBufferFS));
			sprintf((char*) UserTxBufferFS, "gasq:%d", gasq); //Отправка
			while (waittx())
				;
			CDC_Transmit_FS(UserTxBufferFS, strlen(UserTxBufferFS));
			sprintf((char*) UserTxBufferFS, "psimmhg:%d", psi); //Отправка
			while (waittx())
				;
			CDC_Transmit_FS(UserTxBufferFS, strlen(UserTxBufferFS));
			sprintf((char*) UserTxBufferFS, "needtemp:%d", needtemp); //Отправка
			while (waittx())
				;
			CDC_Transmit_FS(UserTxBufferFS, strlen(UserTxBufferFS));
			sprintf((char*) UserTxBufferFS, "needhum:%d", needhum); //Отправка
			while (waittx())
				;
			CDC_Transmit_FS(UserTxBufferFS, strlen(UserTxBufferFS));
			sprintf((char*) UserTxBufferFS, "needhumearth:%d", needhumearth); //Отправка
			while (waittx())
				;
			CDC_Transmit_FS(UserTxBufferFS, strlen(UserTxBufferFS));
			sprintf((char*) UserTxBufferFS, "maxco2:%d", maxco2); //Отправка
			while (waittx())
				;
			CDC_Transmit_FS(UserTxBufferFS, strlen(UserTxBufferFS));
			sprintf((char*) UserTxBufferFS, "ispumpon:%d", ispumpon); //Отправка
			while (waittx())
				;
			CDC_Transmit_FS(UserTxBufferFS, strlen(UserTxBufferFS));
			sprintf((char*) UserTxBufferFS, "iswindowopen:%d", iswindowopen); //Отправка
			while (waittx())
				;
			CDC_Transmit_FS(UserTxBufferFS, strlen(UserTxBufferFS));
			sprintf((char*) UserTxBufferFS, "autowork:%d", autowork); //Отправка
			while (waittx())
				;
			CDC_Transmit_FS(UserTxBufferFS, strlen(UserTxBufferFS));
			sprintf((char*) UserTxBufferFS, "paramde"); //Отправка
			while (waittx())
				;
			CDC_Transmit_FS(UserTxBufferFS, strlen(UserTxBufferFS));
		}
		if (strcmp(strRX, "checkcon") == 0) //Возврат параметров
				{
			sprintf((char*) UserTxBufferFS, "gut"); //Отправка
			while (waittx())
				;
			CDC_Transmit_FS(UserTxBufferFS, strlen(UserTxBufferFS));
		}
		if (strcmp(strRX, "wifion") == 0) {
			wifiinit();
			wifiint = 1;
		}
		if (strcmp(strRX, "gettime") == 0) //Возврат параметров
				{
			sprintf((char*) UserTxBufferFS, "%02d:%02d:%02d\n", hour, minute,
					second); //Отправка
			while (waittx())
				;
			CDC_Transmit_FS(UserTxBufferFS, strlen(UserTxBufferFS));
		}
		if (strcmp(strRX, "setautowork0") == 0) //Возврат параметров
				{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
			htim2.Instance->CCR1 = 90;
			htim3.Instance->CCR1 = 90;
			iswindowopen = 0;
			ispumpon = 0;
			autowork = 0;
		}
		if (strcmp(strRX, "setautowork1") == 0) //Возврат параметров
				{
			autowork = 1;
			pumpworkflag = 0;
		}
		if (strcmp(strRX, "openwin") == 0) { //Возврат параметров
			if (autowork == 0) {
				htim2.Instance->CCR1 = 40;
				htim3.Instance->CCR1 = 40;
			}
		}
		if (strcmp(strRX, "closewin") == 0) //Возврат параметров
				{
			if (autowork == 0) {
				htim2.Instance->CCR1 = 90;
				htim3.Instance->CCR1 = 90;
			}
		}
		if (strcmp(strRX, "poweronpump") == 0) //Возврат параметров
				{
			if (autowork == 0) {
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
				ispumpon = 1;
			}
		}
		if (strcmp(strRX, "poweroffpump") == 0) //Возврат параметров
			if (autowork == 0) {
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
				ispumpon = 0;
			}
		strcpy(strRX, "");
	}
}
int waittx() { //Ожидание освобождения порта
	USBD_CDC_HandleTypeDef *hcdc =
			(USBD_CDC_HandleTypeDef*) hUsbDeviceFS.pClassData;
	if (hcdc->TxState != 0) { //Если никаких действий по отправке не происходит то всё ок
		return 1;
	} else
		return 0;
}

void polsens() { //Функция чтения и преобразования данных с датчиков

	humidair = si7021_measure_humidity(&hi2c2); //Читаем уровень влажности
	if (humidair == SI7021_MEASURE_FAILED) {
	}
	temp1 = si7021_measure_temperature(&hi2c2); //Читаем температуру 1
	if (humidair == SI7021_MEASURE_FAILED) {
	}
	while (!bmp280_read_float(&bmp280, &temperature, &pressure, &humidity)) {
	} //Читаем температуру 2, и атмосферное давление, читаем в пустую переменную влажности (особенности регистров констроллера)
	temp2 = (int) temperature;
	HAL_ADC_Start_DMA(&hadc1, adcval, 2); //Запускаем АЦП на DMA
	int hum;
	hum = adcval[1]; //Читаем сырое значение влажности
	humidearth = ((4096 - hum)) * 100 / 2100; //Переводим в проценты
	gasq = adcval[0]; //Читаем показатели датчика газа
	psi = (int) pressure / 133.322; //Переводим атмосферное давление из паскалей в мм рт ст (нам так привычние)
	//Извлечем время из часиков
	date = DS1307_GetDate();
	month = DS1307_GetMonth();
	year = DS1307_GetYear();
	dow = DS1307_GetDayOfWeek();
	hour = DS1307_GetHour();
	minute = DS1307_GetMinute();
	second = DS1307_GetSecond();

}
int wifiinit() {
	ESP_Init("Ermos", "ermil1999");
	return 0;
}
void displaywork() {
	if (timer_val > 3) {
		nowscreen++;
		if (dispFlag == 1) {
			nowscreen = 0;
			dispFlag = 0;
		}
		timer_val = 0;
	};
	//nowscreen=5;
	switch (nowscreen) {
	case 0:
		SSD1306_Clear();
		SSD1306_GotoXY(0, 0);
		SSD1306_Puts("temp", &Font_11x18, 1);
		SSD1306_GotoXY(10, 20);
		sprintf(buffOut, "%d", temp2);
		SSD1306_Puts(buffOut, &Font_11x18, 1);
		SSD1306_GotoXY(10, 40);
		strcpy(buffOut, "");
		sprintf(buffOut, "need:%d", needtemp);
		SSD1306_Puts(buffOut, &Font_11x18, 1);
		SSD1306_UpdateScreen();
		break;
	case 1:
		SSD1306_Clear();
		SSD1306_GotoXY(0, 0);
		SSD1306_Puts("humid", &Font_11x18, 1);
		SSD1306_GotoXY(10, 20);
		sprintf(buffOut, "%d", humidair);
		SSD1306_Puts(buffOut, &Font_11x18, 1);
		SSD1306_GotoXY(10, 40);
		strcpy(buffOut, "");
		sprintf(buffOut, "need:%d", needhum);
		SSD1306_Puts(buffOut, &Font_11x18, 1);
		SSD1306_UpdateScreen();
		strcpy(buffOut, "");
		break;
	case 2:
		SSD1306_Clear();
		SSD1306_GotoXY(0, 0);
		SSD1306_Puts("hum earth %", &Font_11x18, 1);
		SSD1306_GotoXY(10, 20);
		sprintf(buffOut, "%d", humidearth);
		SSD1306_Puts(buffOut, &Font_11x18, 1);
		SSD1306_GotoXY(10, 40);
		strcpy(buffOut, "");
		sprintf(buffOut, "need:%d", needhumearth);
		SSD1306_Puts(buffOut, &Font_11x18, 1);
		SSD1306_UpdateScreen();
		strcpy(buffOut, "");
		break;
	case 3:
		SSD1306_Clear();
		SSD1306_GotoXY(0, 0);
		SSD1306_Puts("CO2 ppn", &Font_11x18, 1);
		SSD1306_GotoXY(10, 20);
		sprintf(buffOut, "%d", gasq);
		SSD1306_Puts(buffOut, &Font_11x18, 1);
		SSD1306_GotoXY(10, 40);
		strcpy(buffOut, "");
		sprintf(buffOut, "max:%d", maxco2);
		SSD1306_Puts(buffOut, &Font_11x18, 1);
		SSD1306_UpdateScreen();
		strcpy(buffOut, "");
		break;
	case 4:
		SSD1306_Clear();
		SSD1306_GotoXY(0, 0);
		SSD1306_Puts("psi", &Font_11x18, 1);
		SSD1306_GotoXY(10, 30);
		sprintf(buffOut, "%d", psi);
		SSD1306_Puts(buffOut, &Font_11x18, 1);
		SSD1306_UpdateScreen();
		strcpy(buffOut, "");
		break;
	case 5:
		SSD1306_Clear();
		SSD1306_GotoXY(0, 0);
		SSD1306_Puts("dtm", &Font_11x18, 1);
		SSD1306_GotoXY(10, 20);
		char timeBuffer[100] = "";
		sprintf(timeBuffer, "%02d:%02d:%02d", hour, minute, second);
		SSD1306_Puts(timeBuffer, &Font_11x18, 1);
		strcpy(timeBuffer, "");
		SSD1306_GotoXY(10, 40);
		sprintf(timeBuffer, "%04d-%02d-%02d", year, month, date);
		SSD1306_Puts(timeBuffer, &Font_11x18, 1);
		SSD1306_UpdateScreen();
		break;
	case 6:
		SSD1306_Clear();
		SSD1306_GotoXY(0, 0);
		SSD1306_Puts("recive", &Font_11x18, 1);
		SSD1306_GotoXY(10, 30);
		SSD1306_Puts(bufferRX, &Font_11x18, 1);
		SSD1306_UpdateScreen();
		dispFlag = 1;
		break;
	}
}
void autoWork() {
	if (autowork == 1) { //Если режим автоработы
		if (humidearth < needhumearth) { //Если влажность почвы меньше требуемой
			if (pumpworkflag == 0) {
				pumptime = 0, pumpworkflag = 1;
			}
			if (pumptime < 5) { //Включаем насос на 5 секунд
				if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15) == GPIO_PIN_RESET) {
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
					ispumpon = 1;
				}
				pumpdelay = 0;

			} else { //Выключаем насос на 10 секунд
				if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15) == GPIO_PIN_SET) {
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
					ispumpon = 0;
				}
				if (pumpdelay > 10) {
					pumpworkflag = 0; //Обнуляем флаг
					pumptime = 0; //Обнуляем время работы
				}
			}
		} else {
			pumpworkflag = 0;
			if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15) == GPIO_PIN_SET) {
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
				ispumpon = 0;
			}
		}
		if (gasq > maxco2) {
			htim2.Instance->CCR1 = 40;
			htim3.Instance->CCR1 = 40;
			iswindowopen = 1;
		} else {
			htim2.Instance->CCR1 = 90;
			htim3.Instance->CCR1 = 90;
			iswindowopen = 0;
		}
	}
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
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
