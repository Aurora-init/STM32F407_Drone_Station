#include "COMMON/bsp_device.h"
#include "COMMON/bsp_common.h"	
#include "COMMON/bsp_headfile.h"
//#include "include.h"
//#include "bsp_uart.h"

//接收缓存
u8 *RecBuff = NULL;
u16 RecBuff_rx = 0;
u16 RecBuff_tx = 0;

u16 counter_rec = 0;
/************************************************************
Function:函数声明
Input:
Output:
*************************************************************/
uint8_t RPRMUM_ExecAnswer(uint8_t *p,uint16_t DataLen);
uint8_t RPRMUM_ExecInform(uint8_t *p,uint16_t DataLen);
/************************************************************
Function:初始化通信环境
Input:
Output:
*************************************************************/
void RPRMUM_Init(void)
{
	//串口初始化，打开接收中断
	RecBuff = RPRMUM_malloc(USC_BUFF_MAX);
	memset(RecBuff,0x00,USC_BUFF_MAX);
	RecBuff_tx = 0;
	RecBuff_rx = 0;
}

/************************************************************
Function：十六进制方式输出这个数组内容到输出log
Input:
Output:
*************************************************************/
void printf_x(u8 *buff,u32 len)
{

	u32 buff_i = 0;
	for(buff_i = 0;buff_i < len; buff_i++)
	{
		RPRMUM_pr(" %02X ",buff[buff_i]);
		//如果读卡器读到0x33
		if(buff[buff_i] == 0x33)
		{
			counter_rec++;
			if(counter_rec >= 10)
			{
				counter_rec = 0;
				//用户操作代码
				HAL_GPIO_WritePin(GPIOF, LED0_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOE, STATE1_Pin, GPIO_PIN_SET);//放电池的继电器先打开
				MOTOR_RUN1();//长丝杆电机正向走动
				MOTOR_RUN2();//短丝杆电机正向走动
				MOTOR_RUN3();//电磁铁电机正向走动
				MOTOR_RUN4();//顶部电磁铁电机正向走动
				HAL_GPIO_WritePin(GPIOG, STATE2_Pin, GPIO_PIN_SET);//打开继电器拿电池
				HAL_Delay(2500);
				MOTOR_RUN5();//顶部电磁铁电机继续正向走动
				HAL_GPIO_WritePin(GPIOE, STATE1_Pin, GPIO_PIN_RESET);//放电池
				HAL_Delay(2500);
				MOTOR_RUN7();//短丝杆电机反向走动复位
				MOTOR_RUNx();
				MOTOR_RUN6();//电磁铁电机反向走动复位
				MOTOR_RUN8();//长丝杆电机反向走动复位
				MOTOR_RUN9();//顶部电磁铁电机反向走动复位
			}
		}
	}
	RPRMUM_pr("\r\n********************************\r\n");
}

/************************************************************
Function:数据校验和
Input:p为数据，len为数据长度
Output:返回校验结果
*************************************************************/
static uint8_t RPRMUM_SumCheck(uint8_t *p,uint16_t len)
{
    uint8_t check = 0; 
    while(len--)
    {
     check += *p;
     p++;        
    }
    return check;
}
/************************************************************
Function:数据解析，检测数据校验，执行对应功能调度，可由消息邮箱发送
Input:data为数据，len为数据长度
Output:执行成功返回true，失败返回false
*************************************************************/
u8 RPRMUM_UsartDataAnaly(u8 *data,u16 len)
{	
	
	//长度不对，退出
	if(len <= 7){user_main_info("Usc data length err.\r\n");return FALSE;}
    //检查头是否正确
    if(data[0] != COMM_HEAD_1){user_main_info("Usc head err.\r\n");return FALSE;}
    //检查尾是否正确
	if(data[len-1] != COMM_END){user_main_info("Usc data err.\r\n");return FALSE;}
    //检查校验和
    if(data[len-2] !=RPRMUM_SumCheck(&data[1],len-3)){user_main_info("Usc checksum err.\r\n");return FALSE;}
	//数据正常，可以进行执行	
    if(data[1]==0x01){RPRMUM_ExecAnswer(data,len); return TRUE;}//响应帧
    else if(data[1]==0x02){RPRMUM_ExecInform(data,len); return TRUE;}//通知帧
    else {user_main_info("data comm tyep err.\r\n");return FALSE;}

}
/************************************************************
Function:检测循环BUFF的有效数据内容
Input:
Output:返回有效数据的个数
*************************************************************/
u16 dataBuffLength(u16 rx,u16 tx)
{
	if(rx > tx ){return rx-tx;}
	else if(rx < tx){return USC_BUFF_MAX-tx+rx;}
	else return 0;
}
/************************************************************
Function:数据段检查，检查数据段内是否有数据。主程序循环调用该函数
Input:data为数据
Output:TRUE 成功执行，false 未执行,NO_data:2
*************************************************************/
#define NO_DATA 2
u8 UStime_out = 0;
u8 RPRMUM_DataCheck(void)
{		
	unsigned char *data 	= NULL;
	u8 head 	= FALSE;
	u8 end 		= FALSE;
	u8 err 		= FALSE;
	u16 lenHAdd 		= 0;
  u16 lenLAdd 		= 0;
	u16 length 		= 0;

	if(dataBuffLength(RecBuff_rx,RecBuff_tx) <= 7 ) {return NO_DATA;} 

	while(RecBuff_rx != RecBuff_tx)
		{
		 err = FALSE;
		 if(RecBuff[RecBuff_tx] == COMM_HEAD_1)
		 	{
                head = TRUE;
		 	}
		 else {	err = TRUE;}
		 if(err == TRUE)
		 	{	
		 		RecBuff_tx++ ;
				if(RecBuff_tx >= USC_BUFF_MAX)RecBuff_tx=0;
				continue;
		 	}
		 if(head == TRUE){break;}
		}
        if(head == FALSE)return FALSE;

       
        if(RecBuff_tx+3>=USC_BUFF_MAX)
            {lenHAdd = RecBuff_tx+3 - USC_BUFF_MAX;}
        else {lenHAdd = RecBuff_tx+3;}

        if(RecBuff_tx+4>=USC_BUFF_MAX)
            {lenLAdd = RecBuff_tx+4 - USC_BUFF_MAX;}
        else {lenLAdd = RecBuff_tx+4;}
        length = RecBuff[lenHAdd]<<8 | RecBuff[lenLAdd]+7;
        if(dataBuffLength(RecBuff_rx,RecBuff_tx) >=length)
            {	
                u16 i = length;

                if(RecBuff_tx + i >= USC_BUFF_MAX)
                    {
						if(RecBuff_tx + i == USC_BUFF_MAX)
							{
								if(RecBuff[USC_BUFF_MAX-1] == COMM_END)
								{
								 end = TRUE;	
								}
							}
                        else if(RecBuff[(((RecBuff_tx+i)-USC_BUFF_MAX)-1)] == COMM_END)
                            {
                               	end = TRUE;				
                            }
                        else {			
                                RecBuff[RecBuff_tx] = 0xFF;
                                RecBuff_tx ++;
                            }
                    }
                else if(RecBuff[RecBuff_tx+i-1] == COMM_END )
                    {
                        end = TRUE;
                    }
                else {
                        RecBuff[RecBuff_tx] = 0xFF;
                        RecBuff_tx ++;
                }
            }
	else {
		UStime_out ++ ;
		if(UStime_out > 250)
			{
			UStime_out = 0;
			RecBuff[RecBuff_tx] = 0xFF;
			RecBuff_tx ++;
			user_main_info("Usc UStime_out.\r\n");
			}	
		}	
		
		//找到数据
		if( end == TRUE )
			{
			u8 i = 0;
			UStime_out = 0;
			data = RPRMUM_malloc(length);
			memset(data,0x00,length);
			//从指定位置循环读取多个数据,同时处理循环问题
			while(i < length)
				{
				data[i++]=RecBuff[RecBuff_tx];
				RecBuff[RecBuff_tx] = 0x00;
				RecBuff_tx++;
				if(RecBuff_tx >= USC_BUFF_MAX){RecBuff_tx = 0x00;}
				}
			RPRMUM_UsartDataAnaly(data,length);
			RPRMUM_free(data);
			}
	return 1;
}

/************************************************************
Function:接收数据到缓冲区
Input:data为数据
Output:无
*************************************************************/
void RPRMUM_DataReceive(u8 data)
{
	RecBuff[RecBuff_rx] = data;
	
	RecBuff_rx += 1;
	
	if( RecBuff_rx >= USC_BUFF_MAX ){ RecBuff_rx = 0; }
}
/************************************************************
Function:发送命令帧给RPRMUM
Input:
Output:
*************************************************************/
uint8_t RPRMUM_SendCommand(uint8_t command,uint16_t DataLen,uint8_t *p)
{
    uint8_t *data = NULL;
    uint16_t len  = 0;
    data = RPRMUM_malloc(DataLen+10);
    if(data == NULL){ return 0;}
    memset(data,0x00,DataLen+10);

    data[len++] = 0xAA;
    data[len++] = 0x00;
    data[len++] = command;
    data[len++] = (uint8_t)DataLen >> 8;
    data[len++] = (uint8_t)DataLen & 0x00ff;
    if(DataLen != 0)
    {
        memcpy(&data[len],p,DataLen);
        len += DataLen;
    }
    data[len++] = RPRMUM_SumCheck(&data[1],DataLen+4); 
    data[len++] = 0XDD;  
	
    HAL_UART_Transmit(&huart3,data,len,500);
    RPRMUM_free(data);
	return 1;
}

/************************************************************
Function:处理模块响应帧0x01,一般表示命令接收或处理成功，无数据处理，
该处数据已经完成校验和，只需要处理即可
Input:
Output:
*************************************************************/
uint8_t RPRMUM_ExecAnswer(uint8_t *p,uint16_t DataLen)
{

    if((p == NULL)|(DataLen < 8)){RPRMUM_pr("RPRMUM answer err.\r\n");  return TRUE;}
    //指令代码
    uint8_t comm = p[2];

	switch (comm)
	{
	//硬件版本号
	case 0x03:
		RPRMUM_pr("RPRMUM hard type%d.\r\n",p[5]);
		break;
	default:

		break;
	}
	return TRUE;
}

/************************************************************
Function:处理模块通知帧0x02
Input:
Output:
*************************************************************/
uint8_t RPRMUM_ExecInform(uint8_t *p,uint16_t DataLen)
{
	if((p == NULL)|(DataLen < 8)){RPRMUM_pr("RPRMUM Inform err.\r\n");  return TRUE;}
    //指令代码
    uint8_t comm = p[2];

	switch (comm)
	{
	//单次读取返回数据,返回EPC数据
	case 0x22:
		RPRMUM_pr("RSSI:-%d dBM\r\n",p[5]);
		RPRMUM_pr("PC:%02X %02X\r\n",p[6],p[7]);
		RPRMUM_pr("EPC:");
		printf_x(&p[8],RPRMUM_EPC_NUM);
		break;
	default:

		break;
	}
	return TRUE;
}
/************************************************************
Function:控制模块进行单次读取
Input:
Output:
*************************************************************/
void RPRMUM_Comm_ReadCard(void )
{
	RPRMUM_SendCommand(0x22,0x00,NULL);
}
