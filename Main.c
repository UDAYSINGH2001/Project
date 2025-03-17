#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "usart.h"
#include "i2c.h"
#include "adc.h"
#include "gpio.h"
#include "stdio.h"
#include "string.h"
#include "dht11.h"  // Include DHT11 library
#include "ds1307.h"  // Include DS1307 RTC library
#include "lcd_i2c.h" // Include LCD I2C library

ADC_HandleTypeDef hadc1;
UART_HandleTypeDef huart2;
I2C_HandleTypeDef hi2c1;
DHT11_HandleTypeDef dht11;

void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_ADC1_Init(void);
void MX_USART2_UART_Init(void);
void MX_I2C1_Init(void);

char rxBuffer[10];
uint8_t rxIndex = 0;
uint32_t moisture_value;
char msg[100];

int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_ADC1_Init();
    MX_USART2_UART_Init();
    MX_I2C1_Init();
    
    DHT11_Init(&dht11, GPIOA, GPIO_PIN_8);
    DS1307_Init(&hi2c1);
    LCD_I2C_Init(0x27, &hi2c1);
    
    HAL_UART_Receive_IT(&huart2, (uint8_t *)rxBuffer, 1);

    while (1) {
        // Read Soil Moisture
        HAL_ADC_Start(&hadc1);
        if (HAL_ADC_PollForConversion(&hadc1, 1000) == HAL_OK) {
            moisture_value = HAL_ADC_GetValue(&hadc1);
        }
        HAL_ADC_Stop(&hadc1);

        // Read DHT11
        DHT11_Read(&dht11);
        int temp = dht11.Temperature;
        int humidity = dht11.Humidity;
        
        // Get Current Time
        RTC_TimeTypeDef rtc_time;
        DS1307_GetTime(&rtc_time);
        
        // Send Data via Bluetooth
        sprintf(msg, "Moisture: %lu, Temp: %dC, Humidity: %d%%, Time: %02d:%02d\r\n",
                moisture_value, temp, humidity, rtc_time.Hours, rtc_time.Minutes);
        HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
        
        // Update LCD Display
        LCD_I2C_Clear();
        sprintf(msg, "M:%lu T:%dC H:%d%%", moisture_value, temp, humidity);
        LCD_I2C_SetCursor(0, 0);
        LCD_I2C_WriteString(msg);
        sprintf(msg, "Time: %02d:%02d", rtc_time.Hours, rtc_time.Minutes);
        LCD_I2C_SetCursor(1, 0);
        LCD_I2C_WriteString(msg);
        
        // Automatic Water Pump Control
        if (moisture_value < 2000) {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);  // Turn ON Relay
        } else {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // Turn OFF Relay
        }
        HAL_Delay(2000);
    }
}

// Bluetooth Receive Callback
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART2) {
        if (rxBuffer[0] == 'W') {  // "WATER ON" Command
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
        } else if (rxBuffer[0] == 'O') {  // "WATER OFF" Command
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
        }
        HAL_UART_Receive_IT(&huart2, (uint8_t *)rxBuffer, 1);
    }
}

void MX_ADC1_Init(void) {
    ADC_ChannelConfTypeDef sConfig = {0};
    hadc1.Instance = ADC1;
    hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    hadc1.Init.ScanConvMode = DISABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 1;
    HAL_ADC_Init(&hadc1);
    sConfig.Channel = ADC_CHANNEL_1;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);
}

void MX_USART2_UART_Init(void) {
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 9600;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&huart2);
}

void MX_I2C1_Init(void) {
    hi2c1.Instance = I2C1;
    hi2c1.Init.ClockSpeed = 100000;
    hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    HAL_I2C_Init(&hi2c1);
}

void MX_GPIO_Init(void) {
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
