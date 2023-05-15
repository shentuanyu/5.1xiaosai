/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled.h"
#include "stdio.h"
#include "motor.h"
#include "niming.h"
#include "pid.h"
#include "cJSON.h"
#include <string.h>
#include "HC_SR04.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
extern float Motor1Speed ;
extern float Motor2Speed ;
int Motor1Pwm;
int Motor2Pwm;

extern tPid pidMotor1Speed;
extern tPid pidMotor2Speed;

extern uint8_t Usart1_ReadBuf[255]; //串口1 缓冲数组
extern uint8_t Usart1_ReadCount; //串口1 接收字节计数

uint8_t g_ucUsart3ReceiveData; //串口3 接受数据
uint8_t Usart3String[20];
float p,i,d,a,b;

extern float Mileage;
uint8_t OledString[35];

float g;

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

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
  MX_USART1_UART_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM4_Init();
  MX_ADC2_Init();
  MX_DMA_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  OLED_Init();			//初始化OLED  
  OLED_Clear()  	; 
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);//开启定时器1 通道1 PWM输出
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);//开启定时器1 通道4 PWM输出
  HAL_TIM_Encoder_Start(&htim2,TIM_CHANNEL_ALL);//开启定时器2
  HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_ALL);//开启定时器4
  HAL_TIM_Base_Start_IT(&htim2);				//开启定时器2 中断
  HAL_TIM_Base_Start_IT(&htim4);                //开启定时器4 中断
  
  HAL_TIM_Base_Start_IT(&htim1);                //开启定时器1 中断
  
  PID_init();//PID初始化
  
	__HAL_UART_ENABLE_IT(&huart1,UART_IT_RXNE); //开启串口1接受中断
	HAL_UART_Receive_IT(&huart3,&g_ucUsart3ReceiveData,1); //开启串口三接受数据
	
//	cJSON *cJsonData ,*cJsonVlaue;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
	
	
	if(READ_KEY2 ==0)
	{
		motorPidSetSpeed(0.2,0.2);
		while(1)
		{
			 sprintf((char *)Usart3String,"Mileage%.2f\r\n",Mileage);
       HAL_UART_Transmit(&huart3,( uint8_t *)Usart3String,strlen(( const char*)Usart3String),50);
	     sprintf((char *)OledString,"Mileage:%.2f ",Mileage);//显示里程数
       OLED_ShowString(0,2,OledString,12);//这个是oled驱动里面的，是显示位置的一个函数
			 if(Mileage>85)
			 {
			 motorPidSetSpeed(0,0);
			 HAL_Delay(1000);
			 motorPidSetSpeed(0.5,-0.5);
		   HAL_Delay(1300); 
			 motorPidSetSpeed(0,0);
			 HAL_Delay(1000);
			 motorPidSetSpeed(0.2,0.2);
			 HAL_Delay(1000);	 
			 break;
			 }
		}
	}
	
		while(1)
	{
	HAL_Delay(1000);
	HAL_Delay(1000);
  HAL_Delay(1000);
  HAL_Delay(1000);	
  HAL_Delay(1000);		
	HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_SET);
	HAL_Delay(500);
	HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_RESET);
	HAL_Delay(1000);
	HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_SET);
	HAL_Delay(500);
	HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_RESET);
	HAL_Delay(1000);
	HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_SET);
	HAL_Delay(500);
	HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_RESET);
	HAL_Delay(1000);
	motorPidSetSpeed(0.35,0.35);
	HAL_Delay(1000);	
	break;

	}
	
	
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//	OLED_ShowCHinese(0,0,0);//中
//	OLED_ShowCHinese(18,0,1);//景
//	OLED_ShowCHinese(36,0,2);//园
//	OLED_ShowCHinese(54,0,3);//电
//	OLED_ShowCHinese(72,0,4);//子
//	OLED_ShowCHinese(90,0,5);//科
//	OLED_ShowCHinese(108,0,6);//技
//	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 0);
//    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 90);
//    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 90);	
//	HAL_GPIO_WritePin(AIN1_GPIO_Port,AIN1_Pin,GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(BIN1_GPIO_Port,BIN1_Pin,GPIO_PIN_RESET);
	
	
//	if(Motor1Speed <2.9) Motor1Pwm++;
//	if(Motor1Speed >3.1) Motor1Pwm--;
//	if(Motor2Speed <2.9) Motor2Pwm++;
//	if(Motor2Speed >3.1) Motor2Pwm--;


	
	
//	printf("Motor1Speed:%.2f Motor1Pwm:%d\r\n",Motor1Speed,Motor1Pwm);
//	printf("Motor2Speed:%.2f Motor2Pwm:%d\r\n",Motor2Speed,Motor2Pwm);
	HAL_Delay(10);
	
//	motorPidSetSpeed(1,2);//向右
//	motorPidSetSpeed(2,1);//向左
//	motorPidSetSpeed(1,1);//向前
//	motorPidSetSpeed(-1,-1);//向后
//   motorPidSpeedUp();
	 
	 //oled显示
	 

	 
	
  
//	 sprintf((char *)OledString,"V1:%.2f V2:%.2f", Motor1Speed,Motor2Speed);//显示两个电机的速度
//   OLED_ShowString(0,0,OledString,12);//这个是oled驱动里面的，是显示位置的一个函数
//	 
   sprintf((char *)Usart3String,"Mileage%.2f\r\n",Mileage);
   HAL_UART_Transmit(&huart3,( uint8_t *)Usart3String,strlen(( const char*)Usart3String),50);
   sprintf((char *)OledString,"Mileage:%.2f ",Mileage);//显示里程数
   OLED_ShowString(0,2,OledString,12);//这个是oled驱动里面的，是显示位置的一个函数
//	 
//	 sprintf((char *)OledString,"U:%.2fV ",adcGetBatteryVoltage());//显示里程数
//   OLED_ShowString(0,4,OledString,12);//这个是oled驱动里面的，是显示位置的一个函数
//   
	  
   
	 if(READ_HW_OUT_1 == 1&&READ_HW_OUT_2 == 1&&READ_HW_OUT_3 == 1&&READ_HW_OUT_4== 1 )
	 {
		 motorPidSetSpeed(0.2,0.2);
		 HAL_Delay(2000);
//			motorPidSetSpeed(-0.45,0.45);
//		    HAL_Delay(100);
			motorPidSetSpeed(0,0);
		    break;
	 }
	 else if(READ_HW_OUT_1 == 1&&READ_HW_OUT_2 == 1&&READ_HW_OUT_3 == 1&&READ_HW_OUT_4== 0 )
	 {
			
		 motorPidSetSpeed(0.2,0.2);
		 HAL_Delay(2000);
//			motorPidSetSpeed(-0.45,0.45);
//		 HAL_Delay(100);
			motorPidSetSpeed(0,0);
		    break;
	 }
	 else if(READ_HW_OUT_1 == 0&&READ_HW_OUT_2 == 1&&READ_HW_OUT_3 == 1&&READ_HW_OUT_4== 1 )
	 {
			
		 motorPidSetSpeed(0.2,0.2);
		 HAL_Delay(2000);
//			motorPidSetSpeed(-0.45,0.45);
//	   HAL_Delay(100);
			motorPidSetSpeed(0,0);
		    break;
	 }
	else if(READ_HW_OUT_1 == 0&&READ_HW_OUT_2 == 1&&READ_HW_OUT_3 == 1&&READ_HW_OUT_4== 0 )
	 {
			
		 motorPidSetSpeed(0.2,0.2);
		 HAL_Delay(2000);
//			motorPidSetSpeed(-0.45,0.45);
//		 HAL_Delay(100);
			motorPidSetSpeed(0,0);
		    break;
	 }
	 else if(READ_HW_OUT_1 == 0&&READ_HW_OUT_2 == 0&&READ_HW_OUT_3 == 1&&READ_HW_OUT_4== 1 )
	 {
		 motorPidSetSpeed(0.2,0.2);
		 HAL_Delay(2000);
//			motorPidSetSpeed(-0.45,0.45);
//	   HAL_Delay(100);
			motorPidSetSpeed(0,0);
		    break;
	 }
	 else if(READ_HW_OUT_1 == 1&&READ_HW_OUT_2 == 1&&READ_HW_OUT_3 == 0&&READ_HW_OUT_4== 0 )
	 {
			motorPidSetSpeed(0,0);
		    break;
	 }
	 
	 if(READ_HW_OUT_1 == 0&&READ_HW_OUT_2 == 0&&READ_HW_OUT_3 == 0&&READ_HW_OUT_4== 0 )
	 {

			motorPidSetSpeed(0.12,0.12);//前运动
	 }
	 
    if(READ_HW_OUT_1 == 0&&READ_HW_OUT_2 == 0&&READ_HW_OUT_3 == 1&&READ_HW_OUT_4== 0 )
		{

			motorPidSetSpeed(0.15,0.35);//右边运动
		}
		
    if(READ_HW_OUT_1 == 0&&READ_HW_OUT_2 == 0&&READ_HW_OUT_3 == 0&&READ_HW_OUT_4== 1 )
		{

			motorPidSetSpeed(0.15,0.7);//快速右转
		}
		
    if(READ_HW_OUT_1 == 0&&READ_HW_OUT_2 == 1&&READ_HW_OUT_3 == 0&&READ_HW_OUT_4== 0 )
		{

			motorPidSetSpeed(0.35,0.15);//左边运动
		}
		
    if(READ_HW_OUT_1 == 1&&READ_HW_OUT_2 == 0&&READ_HW_OUT_3 == 0&&READ_HW_OUT_4== 0 )
		{

			motorPidSetSpeed(0.7,0.15);//快速左转
		}
  }
	
			while(1)//在暂停点1蜂鸣加超声波测距
	{
  sprintf((char *)Usart3String,"Mileage%.2f\r\n",Mileage);
  HAL_UART_Transmit(&huart3,( uint8_t *)Usart3String,strlen(( const char*)Usart3String),50);
	HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_SET);
	HAL_Delay(500);
	HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_RESET);
	HAL_Delay(1000);
	HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_SET);
	HAL_Delay(500);
	HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_RESET);
	HAL_Delay(1000);
	HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_SET);
	HAL_Delay(500);
	HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_RESET);
	HAL_Delay(1000);
	float g = HC_SR04_Read();	

	sprintf((char *)OledString,"g:%.2fcm",g+15);//显示超声波数据
  OLED_ShowString(0,0,OledString,12);//这个是oled驱动里面的，是显示位置的一个函数	
	sprintf((char *)Usart3String,"g:%.2f\r\n",g+15);
  HAL_UART_Transmit(&huart3,( uint8_t *)Usart3String,strlen(( const char*)Usart3String),50);
	HAL_Delay(5);
	HAL_Delay(1000);
  HAL_Delay(1000);
  HAL_Delay(1000);
	HAL_Delay(1000);
	HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_SET);
	HAL_Delay(500);
	HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_RESET);
	HAL_Delay(1000);
	HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_SET);
	HAL_Delay(500);
	HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_RESET);
	HAL_Delay(1000);
	HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_SET);
	HAL_Delay(500);
	HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_RESET);	
	break;
   }
	
   while(1)//避障前的循线
   {
	   sprintf((char *)Usart3String,"Mileage%.2f\r\n",Mileage);
     HAL_UART_Transmit(&huart3,( uint8_t *)Usart3String,strlen(( const char*)Usart3String),50);
		 sprintf((char *)OledString,"Mileage:%.2f ",Mileage);//显示里程数
     OLED_ShowString(0,2,OledString,12);//这个是oled驱动里面的，是显示位置的一个函数
		 sprintf((char *)OledString,"Mileage:%.2f ",Mileage);//显示里程数
   OLED_ShowString(0,2,OledString,12);//这个是oled驱动里面的，是显示位置的一个函数
			 if(READ_HW_OUT_1 == 0&&READ_HW_OUT_2 == 0&&READ_HW_OUT_3 == 0&&READ_HW_OUT_4== 0 )
	 {

			motorPidSetSpeed(0.4,0.4);//前运动
	 }
	 
    if(READ_HW_OUT_1 == 0&&READ_HW_OUT_2 == 0&&READ_HW_OUT_3 == 1&&READ_HW_OUT_4== 0 )
		{

			motorPidSetSpeed(0.2,0.3);//右边运动，0.2 0.4
		}
		
    if(READ_HW_OUT_1 == 0&&READ_HW_OUT_2 == 0&&READ_HW_OUT_3 == 0&&READ_HW_OUT_4== 1 )
		{

			motorPidSetSpeed(0.2,0.6);//快速右转
		}
		
    if(READ_HW_OUT_1 == 0&&READ_HW_OUT_2 == 1&&READ_HW_OUT_3 == 0&&READ_HW_OUT_4== 0 )
		{

			motorPidSetSpeed(0.3,0.2);//左边运动
		}
		
    if(READ_HW_OUT_1 == 1&&READ_HW_OUT_2 == 0&&READ_HW_OUT_3 == 0&&READ_HW_OUT_4== 0 )
		{

			motorPidSetSpeed(0.6,0.2);//快速左转
		}
		
	   float g = HC_SR04_Read();	
		 if(g<=22&&g>1)  break;
	  }
	 

	//避障代码
		motorPidSetSpeed(1,-1);
		HAL_Delay(350);
	  motorPidSetSpeed(0,0);
	  HAL_Delay(500);
		motorPidSetSpeed(1,1);
		HAL_Delay(1800);
		motorPidSetSpeed(0,0);
	  HAL_Delay(500);
	  motorPidSetSpeed(-1,1);
		HAL_Delay(600);
	  motorPidSetSpeed(0,0);
	  HAL_Delay(500);
	

		
		
		while(1)//避障到达暂停点2
   {
		sprintf((char *)Usart3String,"Mileage%.2f\r\n",Mileage);
     HAL_UART_Transmit(&huart3,( uint8_t *)Usart3String,strlen(( const char*)Usart3String),50); 
	  sprintf((char *)OledString,"Mileage:%.2f ",Mileage);//显示里程数
    OLED_ShowString(0,2,OledString,12);//这个是oled驱动里面的，是显示位置的一个函数
		 
		 
		if(READ_HW_OUT_1 == 1&&READ_HW_OUT_2 == 1&&READ_HW_OUT_3 == 1&&READ_HW_OUT_4== 1 )
	 {
      motorPidSetSpeed(0.3,0.3);
		  HAL_Delay(500);
			motorPidSetSpeed(0,0);
		  HAL_Delay(500);
		    break;
	 }
	 else if(READ_HW_OUT_1 == 1&&READ_HW_OUT_2 == 1&&READ_HW_OUT_3 == 1&&READ_HW_OUT_4== 0 )
	 {
      motorPidSetSpeed(0.3,0.3);
		  HAL_Delay(500);
			motorPidSetSpeed(0,0);
		  HAL_Delay(500);
		    break;
	 }
	 else if(READ_HW_OUT_1 == 0&&READ_HW_OUT_2 == 1&&READ_HW_OUT_3 == 1&&READ_HW_OUT_4== 1 )
	 {
      motorPidSetSpeed(0.3,0.3);
		  HAL_Delay(500);
			motorPidSetSpeed(0,0);
		  HAL_Delay(500);
		    break;
	 }
	else if(READ_HW_OUT_1 == 0&&READ_HW_OUT_2 == 1&&READ_HW_OUT_3 == 1&&READ_HW_OUT_4== 0 )
	 {
      motorPidSetSpeed(0.3,0.3);
		  HAL_Delay(500);
			motorPidSetSpeed(0,0);
		  HAL_Delay(500);
		    break;
	 }
	 else if(READ_HW_OUT_1 == 0&&READ_HW_OUT_2 == 0&&READ_HW_OUT_3 == 1&&READ_HW_OUT_4== 1 )
	 {
      motorPidSetSpeed(0.3,0.3);
		  HAL_Delay(500);
			motorPidSetSpeed(0,0);
		  HAL_Delay(500);
		    break;
	 }
	 else if(READ_HW_OUT_1 == 1&&READ_HW_OUT_2 == 1&&READ_HW_OUT_3 == 0&&READ_HW_OUT_4== 0 )
	 {
		  motorPidSetSpeed(0.3,0.3);
		  HAL_Delay(500);
			motorPidSetSpeed(0,0);
		  HAL_Delay(500);
		    break;
	 }
		 
		if(READ_HW_OUT_1 == 0&&READ_HW_OUT_2 == 0&&READ_HW_OUT_3 == 0&&READ_HW_OUT_4== 0 )
	 {
			printf("应该前进\r\n");
			motorPidSetSpeed(0.4,0.4);//前运动
	 }
	 
    if(READ_HW_OUT_1 == 0&&READ_HW_OUT_2 == 0&&READ_HW_OUT_3 == 1&&READ_HW_OUT_4== 0 )
		{
			printf("应该右转\r\n");
			motorPidSetSpeed(0.1,0.3);//右边运动
		}
		
    if(READ_HW_OUT_1 == 0&&READ_HW_OUT_2 == 0&&READ_HW_OUT_3 == 0&&READ_HW_OUT_4== 1 )
		{
			printf("快速右转\r\n");
			motorPidSetSpeed(0.1,0.7);//快速右转
		}
		
    if(READ_HW_OUT_1 == 0&&READ_HW_OUT_2 == 1&&READ_HW_OUT_3 == 0&&READ_HW_OUT_4== 0 )
		{
			printf("应该左转\r\n");
			motorPidSetSpeed(0.5,0.1);//左边运动
		}
		
    if(READ_HW_OUT_1 == 1&&READ_HW_OUT_2 == 0&&READ_HW_OUT_3 == 0&&READ_HW_OUT_4== 0 )
		{
			printf("快速左转\r\n");
			motorPidSetSpeed(0.7,0.1);//快速左转
		}
	 
	 }
	 //	 motorPidSetSpeed(-1,1);
	 //  HAL_Delay(600);//90度角k
	 
	 

	 
	 
	 
	 while(1)//在暂停点2蜂鸣
	 {
//			sprintf((char *)OledString,"HC_SR04:%.2fcm",g);//显示超声波数据
//			OLED_ShowString(0,1,OledString,12);//这个是oled驱动里面的，是显示位置的一个函数	
			HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_SET);
			HAL_Delay(500);
			HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_RESET);
			HAL_Delay(1000);
			HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_SET);
			HAL_Delay(500);
			HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_RESET);
			HAL_Delay(1000);
			HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_SET);
			HAL_Delay(500);
			HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_RESET);
			HAL_Delay(5000);
		  HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_SET);
			HAL_Delay(500);
			HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_RESET);
			HAL_Delay(1000);
			HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_SET);
			HAL_Delay(500);
			HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_RESET);
			HAL_Delay(1000);
			HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_SET);
			HAL_Delay(500);
			HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_RESET);
			break;
	 }
	 
	 
	 
	 
	 motorPidSetSpeed(0.5,0.5);
	 HAL_Delay(4300);
	 motorPidSetSpeed(0,0);
	 HAL_Delay(1500);
	 motorPidSetSpeed(-0.5,0.5);
	 HAL_Delay(1300);
	 
	 while(1)//到暂停点三的循线
	 {
	  sprintf((char *)Usart3String,"Mileage%.2f\r\n",Mileage);
     HAL_UART_Transmit(&huart3,( uint8_t *)Usart3String,strlen(( const char*)Usart3String),50);
	  sprintf((char *)OledString,"Mileage:%.2f ",Mileage);//显示里程数
    OLED_ShowString(0,2,OledString,12);//这个是oled驱动里面的，是显示位置的一个函数
	 	if(READ_HW_OUT_1 == 1&&READ_HW_OUT_2 == 1&&READ_HW_OUT_3 == 1&&READ_HW_OUT_4== 1 )
	 {
      motorPidSetSpeed(0.8,0.8);
		  HAL_Delay(500);
			motorPidSetSpeed(0,0);
		  HAL_Delay(500);
		    break;
	 }
	 else if(READ_HW_OUT_1 == 1&&READ_HW_OUT_2 == 1&&READ_HW_OUT_3 == 1&&READ_HW_OUT_4== 0 )
	 {
      motorPidSetSpeed(0.8,0.8);
		  HAL_Delay(500);
			motorPidSetSpeed(0,0);
		  HAL_Delay(500);
		    break;
	 }
	 else if(READ_HW_OUT_1 == 0&&READ_HW_OUT_2 == 1&&READ_HW_OUT_3 == 1&&READ_HW_OUT_4== 1 )
	 {
      motorPidSetSpeed(0.8,0.8);
		  HAL_Delay(500);
			motorPidSetSpeed(0,0);
		  HAL_Delay(500);
		    break;
	 }
	else if(READ_HW_OUT_1 == 0&&READ_HW_OUT_2 == 1&&READ_HW_OUT_3 == 1&&READ_HW_OUT_4== 0 )
	 {
      motorPidSetSpeed(0.8,0.8);
		  HAL_Delay(500);
			motorPidSetSpeed(0,0);
		  HAL_Delay(500);
		    break;
	 }
	 else if(READ_HW_OUT_1 == 0&&READ_HW_OUT_2 == 0&&READ_HW_OUT_3 == 1&&READ_HW_OUT_4== 1 )
	 {
      motorPidSetSpeed(0.8,0.8);
		  HAL_Delay(500);
			motorPidSetSpeed(0,0);
		  HAL_Delay(500);
		    break;
	 }
	 else if(READ_HW_OUT_1 == 1&&READ_HW_OUT_2 == 1&&READ_HW_OUT_3 == 0&&READ_HW_OUT_4== 0 )
	 {
		  motorPidSetSpeed(0.8,0.8);
		  HAL_Delay(500);
			motorPidSetSpeed(0,0);
		  HAL_Delay(500);
		    break;
	 }
		 
		if(READ_HW_OUT_1 == 0&&READ_HW_OUT_2 == 0&&READ_HW_OUT_3 == 0&&READ_HW_OUT_4== 0 )
	 {
			printf("应该前进\r\n");
			motorPidSetSpeed(0.8,0.8);//前运动
	 }
	 
    if(READ_HW_OUT_1 == 0&&READ_HW_OUT_2 == 0&&READ_HW_OUT_3 == 1&&READ_HW_OUT_4== 0 )
		{
			printf("应该右转\r\n");
			motorPidSetSpeed(0.1,0.3);//右边运动
		}
		
    if(READ_HW_OUT_1 == 0&&READ_HW_OUT_2 == 0&&READ_HW_OUT_3 == 0&&READ_HW_OUT_4== 1 )
		{
			printf("快速右转\r\n");
			motorPidSetSpeed(0.1,0.6);//快速右转
		}
		
    if(READ_HW_OUT_1 == 0&&READ_HW_OUT_2 == 1&&READ_HW_OUT_3 == 0&&READ_HW_OUT_4== 0 )
		{
			printf("应该左转\r\n");
			motorPidSetSpeed(0.3,0.1);//左边运动
		}
		
    if(READ_HW_OUT_1 == 1&&READ_HW_OUT_2 == 0&&READ_HW_OUT_3 == 0&&READ_HW_OUT_4== 0 )
		{
			printf("快速左转\r\n");
			motorPidSetSpeed(0.6,0.1);//快速左转
		}
		 
	 }
	 
	 
	    HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_SET);
			HAL_Delay(500);
			HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_RESET);
			HAL_Delay(1000);
			HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_SET);
			HAL_Delay(500);
			HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_RESET);
			HAL_Delay(1000);
			HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_SET);
			HAL_Delay(500);
			HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_RESET);	 
	    HAL_Delay(5000);
		  HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_SET);
			HAL_Delay(500);
			HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_RESET);
			HAL_Delay(1000);
			HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_SET);
			HAL_Delay(500);
			HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_RESET);
			HAL_Delay(1000);
			HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_SET);
			HAL_Delay(500);
			HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_RESET);
	 
	     motorPidSetSpeed(-1,1);
	     HAL_Delay(300);//90度角k
	     motorPidSetSpeed(0,0);
	     HAL_Delay(600);
	     motorPidSetSpeed(1,1);
	     HAL_Delay(7200);
			 motorPidSetSpeed(0,0);
	     HAL_Delay(600);
			 motorPidSetSpeed(0,0);
	     motorPidSetSpeed(-1,1);
			 HAL_Delay(720);
			 motorPidSetSpeed(0,0);
			 HAL_Delay(600);
	 
	 while(1)
	 {
	  sprintf((char *)Usart3String,"Mileage%.2f\r\n",Mileage);
     HAL_UART_Transmit(&huart3,( uint8_t *)Usart3String,strlen(( const char*)Usart3String),50);
	  sprintf((char *)OledString,"Mileage:%.2f ",Mileage);//显示里程数
    OLED_ShowString(0,2,OledString,12);//这个是oled驱动里面的，是显示位置的一个函数
	 	if(READ_HW_OUT_1 == 1&&READ_HW_OUT_2 == 1&&READ_HW_OUT_3 == 1&&READ_HW_OUT_4== 1 )
	 {
      motorPidSetSpeed(0.8,0.8);
		  HAL_Delay(500);
			motorPidSetSpeed(0,0);
		  HAL_Delay(500);
		    break;
	 }
	 else if(READ_HW_OUT_1 == 1&&READ_HW_OUT_2 == 1&&READ_HW_OUT_3 == 1&&READ_HW_OUT_4== 0 )
	 {
      motorPidSetSpeed(0.8,0.8);
		  HAL_Delay(500);
			motorPidSetSpeed(0,0);
		  HAL_Delay(500);
		    break;
	 }
	 else if(READ_HW_OUT_1 == 0&&READ_HW_OUT_2 == 1&&READ_HW_OUT_3 == 1&&READ_HW_OUT_4== 1 )
	 {
      motorPidSetSpeed(0.8,0.8);
		  HAL_Delay(500);
			motorPidSetSpeed(0,0);
		  HAL_Delay(500);
		    break;
	 }
	else if(READ_HW_OUT_1 == 0&&READ_HW_OUT_2 == 1&&READ_HW_OUT_3 == 1&&READ_HW_OUT_4== 0 )
	 {
      motorPidSetSpeed(0.8,0.8);
		  HAL_Delay(500);
			motorPidSetSpeed(0,0);
		  HAL_Delay(500);
		    break;
	 }
	 else if(READ_HW_OUT_1 == 0&&READ_HW_OUT_2 == 0&&READ_HW_OUT_3 == 1&&READ_HW_OUT_4== 1 )
	 {
      motorPidSetSpeed(0.8,0.8);
		  HAL_Delay(500);
			motorPidSetSpeed(0,0);
		  HAL_Delay(500);
		    break;
	 }
	 else if(READ_HW_OUT_1 == 1&&READ_HW_OUT_2 == 1&&READ_HW_OUT_3 == 0&&READ_HW_OUT_4== 0 )
	 {
		  motorPidSetSpeed(0.8,0.8);
		  HAL_Delay(500);
			motorPidSetSpeed(0,0);
		  HAL_Delay(500);
		    break;
	 }
		 
		if(READ_HW_OUT_1 == 0&&READ_HW_OUT_2 == 0&&READ_HW_OUT_3 == 0&&READ_HW_OUT_4== 0 )
	 {
			printf("应该前进\r\n");
			motorPidSetSpeed(0.8,0.8);//前运动
	 }
	 
    if(READ_HW_OUT_1 == 0&&READ_HW_OUT_2 == 0&&READ_HW_OUT_3 == 1&&READ_HW_OUT_4== 0 )
		{
			printf("应该右转\r\n");
			motorPidSetSpeed(0.1,0.3);//右边运动
		}
		
    if(READ_HW_OUT_1 == 0&&READ_HW_OUT_2 == 0&&READ_HW_OUT_3 == 0&&READ_HW_OUT_4== 1 )
		{
			printf("快速右转\r\n");
			motorPidSetSpeed(0.1,0.6);//快速右转
		}
		
    if(READ_HW_OUT_1 == 0&&READ_HW_OUT_2 == 1&&READ_HW_OUT_3 == 0&&READ_HW_OUT_4== 0 )
		{
			printf("应该左转\r\n");
			motorPidSetSpeed(0.3,0.1);//左边运动
		}
		
    if(READ_HW_OUT_1 == 1&&READ_HW_OUT_2 == 0&&READ_HW_OUT_3 == 0&&READ_HW_OUT_4== 0 )
		{
			printf("快速左转\r\n");
			motorPidSetSpeed(0.6,0.1);//快速左转
		}
			 
	 }
	 		HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_SET);
			HAL_Delay(500);
			HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_RESET);
			HAL_Delay(1000);
			HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_SET);
			HAL_Delay(500);
			HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_RESET);
			HAL_Delay(1000);
			HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_SET);
			HAL_Delay(500);
			HAL_GPIO_WritePin(FWQ_GPIO_Port,FWQ_Pin,GPIO_PIN_RESET);
	 
	 
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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
