#include "COMMON/bsp_device.h"
#include "COMMON/bsp_common.h"	
#include "COMMON/bsp_headfile.h"	
#include "main.h"

//电机速度控制系数，1000是指在200000us(200ms)内转1圈
#define Motor_speed_control_coefficient 1000 
//长丝杆电机转动圈数，2600是指转13圈
#define Number_of_revolutions_of_motor1 2600
//短丝杆电机转动圈数，2600是指转13圈
#define Number_of_revolutions_of_motor2 800

//1.长丝杆电机(完成)
void MOTOR_RUN1(void)
{
	int x;
	HAL_GPIO_WritePin(DIR1_GPIO_Port, DIR1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DIR2_GPIO_Port, DIR2_Pin, GPIO_PIN_RESET);
	for(x=0; x<1900; x=x+1)//电机正转	//200一圈 
    {
      HAL_GPIO_WritePin(STEP1_GPIO_Port, STEP1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(STEP2_GPIO_Port, STEP2_Pin, GPIO_PIN_SET);
      bsp_delay_us(Motor_speed_control_coefficient); //延时控制速度
      HAL_GPIO_WritePin(STEP1_GPIO_Port, STEP1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(STEP2_GPIO_Port, STEP2_Pin, GPIO_PIN_RESET);
      bsp_delay_us(Motor_speed_control_coefficient);
    }
    HAL_Delay(1000);//hal库延时1s
}

//2.短丝杆电机(完成)
void MOTOR_RUN2(void)
{
	int x;
	HAL_GPIO_WritePin(DIR3_GPIO_Port, DIR3_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(DIR4_GPIO_Port, DIR4_Pin, GPIO_PIN_RESET);
	for(x=0; x<1200; x=x+1)//电机正转 //200一圈 
    {
      HAL_GPIO_WritePin(STEP3_GPIO_Port, STEP3_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(STEP4_GPIO_Port, STEP4_Pin, GPIO_PIN_SET);
      bsp_delay_us(Motor_speed_control_coefficient);
      HAL_GPIO_WritePin(STEP3_GPIO_Port, STEP3_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(STEP4_GPIO_Port, STEP4_Pin, GPIO_PIN_RESET);
      bsp_delay_us(Motor_speed_control_coefficient);
    }
    HAL_Delay(1000);//hal库延时1s
}

//3.电磁铁电机(完成)
void MOTOR_RUN3(void)
{
	int x;
	HAL_GPIO_WritePin(DIR5_GPIO_Port, DIR5_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(DIR7_GPIO_Port, DIR7_Pin, GPIO_PIN_RESET);
	for(x=0; x<1150; x=x+1)//电机正转 //200一圈 
    {
      HAL_GPIO_WritePin(STEP5_GPIO_Port, STEP5_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(STEP7_GPIO_Port, STEP7_Pin, GPIO_PIN_SET);
      bsp_delay_us(Motor_speed_control_coefficient);
      HAL_GPIO_WritePin(STEP5_GPIO_Port, STEP5_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(STEP7_GPIO_Port, STEP7_Pin, GPIO_PIN_RESET);
      bsp_delay_us(Motor_speed_control_coefficient);
    }
    HAL_Delay(1000);//hal库延时1s
}

//4.顶部电磁铁电机(完成)(拿电池的电机)
void MOTOR_RUN4(void)
{
	int x;
	HAL_GPIO_WritePin(DIR6_GPIO_Port, DIR6_Pin, GPIO_PIN_RESET);
	for(x=0; x<650; x=x+1)//电机正转 //200一圈 
    {
      HAL_GPIO_WritePin(STEP6_GPIO_Port, STEP6_Pin, GPIO_PIN_SET);
      bsp_delay_us(Motor_speed_control_coefficient);
      HAL_GPIO_WritePin(STEP6_GPIO_Port, STEP6_Pin, GPIO_PIN_RESET);
      bsp_delay_us(Motor_speed_control_coefficient);
    }
    HAL_Delay(1000);//hal库延时1s
}

//5.顶部电磁铁电机(完成)(放电池的电机)
void MOTOR_RUN5(void)
{
	int x;
	HAL_GPIO_WritePin(DIR6_GPIO_Port, DIR6_Pin, GPIO_PIN_RESET);
	for(x=0; x<550; x=x+1)//电机正转 //200一圈 
    {
      HAL_GPIO_WritePin(STEP6_GPIO_Port, STEP6_Pin, GPIO_PIN_SET);
      bsp_delay_us(Motor_speed_control_coefficient);
      HAL_GPIO_WritePin(STEP6_GPIO_Port, STEP6_Pin, GPIO_PIN_RESET);
      bsp_delay_us(Motor_speed_control_coefficient);
    }
    HAL_Delay(1000);//hal库延时1s
}

//5.顶部电磁铁电机(完成)(往前走一点点防止撞到无人机)
void MOTOR_RUNx(void)
{
	int x;
	HAL_GPIO_WritePin(DIR6_GPIO_Port, DIR6_Pin, GPIO_PIN_RESET);
	for(x=0; x<700; x=x+1)//电机正转 //200一圈 
    {
      HAL_GPIO_WritePin(STEP6_GPIO_Port, STEP6_Pin, GPIO_PIN_SET);
      bsp_delay_us(Motor_speed_control_coefficient);
      HAL_GPIO_WritePin(STEP6_GPIO_Port, STEP6_Pin, GPIO_PIN_RESET);
      bsp_delay_us(Motor_speed_control_coefficient);
    }
    HAL_Delay(1000);//hal库延时1s
}

//6.电磁铁电机复位(完成)
void MOTOR_RUN6(void)
{
	int x;
	HAL_GPIO_WritePin(DIR5_GPIO_Port, DIR5_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DIR7_GPIO_Port, DIR7_Pin, GPIO_PIN_SET);
	for(x=0; x<1150; x=x+1)//电机正转 //200一圈 
    {
      HAL_GPIO_WritePin(STEP5_GPIO_Port, STEP5_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(STEP7_GPIO_Port, STEP7_Pin, GPIO_PIN_SET);
      bsp_delay_us(Motor_speed_control_coefficient);
      HAL_GPIO_WritePin(STEP5_GPIO_Port, STEP5_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(STEP7_GPIO_Port, STEP7_Pin, GPIO_PIN_RESET);
      bsp_delay_us(Motor_speed_control_coefficient);
    }
    HAL_Delay(1000);//hal库延时1s
}

//7.短丝杆电机复位(完成)
void MOTOR_RUN7(void)
{
	int x;
	HAL_GPIO_WritePin(DIR3_GPIO_Port, DIR3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DIR4_GPIO_Port, DIR4_Pin, GPIO_PIN_SET);
	for(x=0; x<1200; x=x+1)//电机正转 //200一圈 
    {
      HAL_GPIO_WritePin(STEP3_GPIO_Port, STEP3_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(STEP4_GPIO_Port, STEP4_Pin, GPIO_PIN_SET);
      bsp_delay_us(Motor_speed_control_coefficient);
      HAL_GPIO_WritePin(STEP3_GPIO_Port, STEP3_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(STEP4_GPIO_Port, STEP4_Pin, GPIO_PIN_RESET);
      bsp_delay_us(Motor_speed_control_coefficient);
    }
    HAL_Delay(1000);//hal库延时1s
}

//8.长丝杆电机复位(完成)
void MOTOR_RUN8(void)
{
	int x;
	HAL_GPIO_WritePin(DIR1_GPIO_Port, DIR1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(DIR2_GPIO_Port, DIR2_Pin, GPIO_PIN_SET);
	for(x=0; x<1900; x=x+1)//电机正转	//200一圈 
    {
      HAL_GPIO_WritePin(STEP1_GPIO_Port, STEP1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(STEP2_GPIO_Port, STEP2_Pin, GPIO_PIN_SET);
      bsp_delay_us(Motor_speed_control_coefficient); //延时控制速度
      HAL_GPIO_WritePin(STEP1_GPIO_Port, STEP1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(STEP2_GPIO_Port, STEP2_Pin, GPIO_PIN_RESET);
      bsp_delay_us(Motor_speed_control_coefficient);
    }
    HAL_Delay(1000);//hal库延时1s
}

//9.顶部电磁铁电机复位(完成)
void MOTOR_RUN9(void)
{
	int x;
	HAL_GPIO_WritePin(DIR6_GPIO_Port, DIR6_Pin, GPIO_PIN_SET);
	for(x=0; x<1900; x=x+1)//电机正转 //200一圈 
    {
      HAL_GPIO_WritePin(STEP6_GPIO_Port, STEP6_Pin, GPIO_PIN_SET);
      bsp_delay_us(Motor_speed_control_coefficient);
      HAL_GPIO_WritePin(STEP6_GPIO_Port, STEP6_Pin, GPIO_PIN_RESET);
      bsp_delay_us(Motor_speed_control_coefficient);
    }
    HAL_Delay(1000);//hal库延时1s
}

////状态机
//void MOTOR(void)
//{
//	
//}
