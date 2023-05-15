#include "motor.h"
#include "tim.h"
#include "pid.h"


#define MAX_SPEED_up 3

extern float Motor1Speed;
extern float Motor2Speed;	
extern tPid pidMotor1Speed;
extern tPid pidMotor2Speed;
float motorSpeedUpCut;

void Motor_Set(int Motor1,int Motor2)
{
	//取反更适合控制习惯，大于0小车向前运动
	Motor1 =-Motor1;
	Motor2 =-Motor2;
	
	//1.先根据正负设置方向GPIO 高低电平
	if(Motor1 <0) BIN1_SET;
	else  BIN1_RESET;
	
	if(Motor2 <0) AIN1_SET;
	else AIN1_RESET;
	
	//2.然后设置占空比  
	if(Motor1 <0)
	{
		if(Motor1 <-99) Motor1 =-99;
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (100+Motor1));
	}
	else 
	{
		if(Motor1 >99) Motor1 = 99;
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,Motor1);
	}

	if(Motor2<0)
	{
		if(Motor2 <-99) Motor2=-99;
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, (100+Motor2));
	}
	else
	{
		if(Motor2 >99) Motor2 =99;
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, Motor2);
	}


}

//	motorPidSetSpeed(1,2);//向右
//	motorPidSetSpeed(2,1);//向左
//	motorPidSetSpeed(1,1);//向前
//	motorPidSetSpeed(-1,-1);//向后
//  motorPidSetSpeed(-1,1);//原地右旋
//  motorPidSetSpeed(1,-1);//原地左旋
//电机1（右边）目标速度和电机2（左边）目标速度

void motorPidSetSpeed(float Motor1SetSpeed,float Motor2SetSpeed)
{
	//设置PID目标转速
   	pidMotor1Speed.target_val=Motor1SetSpeed;
		pidMotor2Speed.target_val=Motor2SetSpeed;
    //PID计算控制电机
	  Motor_Set(PID_realize(&pidMotor1Speed,Motor1Speed),PID_realize(&pidMotor2Speed,Motor2Speed));	
	
	
}
//加速
void motorPidSpeedUp(void)
{

	if(motorSpeedUpCut <= MAX_SPEED_up) motorSpeedUpCut +=0.5;
   motorPidSetSpeed(motorSpeedUpCut,motorSpeedUpCut);
}
//减速
void motorPidSpeedCut(void)
{

	if(motorSpeedUpCut >=0.5) motorSpeedUpCut -=0.5;
	   motorPidSetSpeed(motorSpeedUpCut,motorSpeedUpCut);
}




