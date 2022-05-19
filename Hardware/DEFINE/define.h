#ifndef _DEFINE_H
#define _DEFINE_H


#include "gd32e23x.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include <string.h>

#include "bsp.h"
#include "multitask.h"
#include "vector.h"
#include "led.h"
#include "queue.h"
#include "pcmodel.h"
#include "usart0.h"
#include "mdu_data.h"
#include "uart_timer_recv.h"
#include "comm_snd.h"
#include "timer2.h"
#include "motor.h"
#include "adc.h"
#include "exti.h"
#include "timer14.h"
#include "waterlevel.h"
#include "tm1650.h"
#include "key.h"
#include "msgqueue.h"
#include "gd32flash.h"
#include "usart1.h"
#include "rfid_data.h"
#include "timer15.h"
#include "valve.h"

/*全局定义*/
//#define PILIANG_PRODUCTION              //批量生产，要再软件上设置看门狗
                          
#define KAIFA_DEBUG                   //开发BUG

//#define KAIFA_TEST                    //开发测试

//#define WDT_ENABLE                    //使能看门狗，LVR设置2.9V

#define ERASE_FLASH  0                  //当设置两次任意不同数擦除存储，还要再设置回0


/***********************************批量生产*****************************/
#ifdef  PILIANG_PRODUCTION

#define  POWER_PARA                0XAA     //=0XAA:上电前参数  =0: 上电后参数
#define  DF_FILTER_RESET_PARA      0XBB     //=0： 滤芯复位前   =0XBB: 滤芯复位后
#define  NEW_MEMBRANE_RINSE_PARA   0XCC     //=0： 冲洗前       =0XCC: 冲洗后
#define  BOIL_POINT_STUDY_PARA     0        //=0: 沸点学习前 =实际值：沸点学习后

/*按天存储*/
#define CPF_LIFE_FULL_1YEAR       365      //CPF寿命
#define DF_LIFE_FULL_5YEAR        1825     //DF寿命
/*按分钟存储*/
#define CPF_MOTOR800_WORK_TIME    2750      //分钟 
#define DF_MOTOR800_WORK_TIME     13750     //分钟 
/*按分钟存储*/
#define CPF_MOTOR1000_WORK_TIME    2200     //分钟 
#define DF_MOTOR1000_WORK_TIME     11000    //分钟 


#define TIME_70S                   70        //单位S
#define TIME_2MIN                  2*60      //单位S
#define TIME_18MIN                 18*60     //单位S,
#define TIME_24HOUR                24*3600   //单位S,

#endif
/***********************************批量生产*****************************/

/***********************************开发调试*****************************/
#ifdef KAIFA_DEBUG 

#define  POWER_PARA                0XAA     //=0XAA:上电前参数  =0: 上电后参数
#define  DF_FILTER_RESET_PARA      0XBB     //=0： 滤芯复位前   =0XBB: 滤芯复位后
#define  NEW_MEMBRANE_RINSE_PARA   0XCC     //=0： 冲洗前       =0XCC: 冲洗后
#define  BOIL_POINT_STUDY_PARA     0X00     //=0: 沸点学习前    =实际值：沸点学习后

/*按天存储*/
#define CPF_LIFE_FULL_1YEAR       365      //CPF寿命
#define DF_LIFE_FULL_5YEAR        1825     //DF寿命
/*按分钟存储*/
#define CPF_MOTOR800_WORK_TIME    2750      //分钟 
#define DF_MOTOR800_WORK_TIME     13750     //分钟 
/*按分钟存储*/
#define CPF_MOTOR1000_WORK_TIME    2200     //分钟 
#define DF_MOTOR1000_WORK_TIME     11000    //分钟 


#define TIME_2S                    100       //单位MS,20MS一次
#define TIME_5S                    250       //单位MS,20MS一次
#define TIME_10S                   500       //单位MS,20MS一次
#define TIME_12S                   12        //单位S
#define TIME_30S                   30        //单位S
#define TIME_70S                   70        //单位S
#define TIME_2MIN                  0         //单位S，测试1S
#define TIME_5MIN                  5*60      //单位S
#define TIME_18MIN                 0         //单位S,测试1S
#define TIME_250MIN                250*60    //单位S,测试1分钟
#define TIME_24HOUR                24*60*60  //单位S,测试1分钟
#define TIME_1DAY                  24*60*60  //单位S,测试1分钟
#define TIME_1Month                30*24     //单位天,测试1分钟

#define BOIL_POINT_97C             97        //97度
#define KEEP_TEMP_95C              95        //95度
#define KEEP_TEMP_60C              60        //60度
#define KEEP_TEMP_45C              45        //45度



#endif
/***********************************开发调试*****************************/





/*开关总中断*/
#define  OS_ENTER_CRITICAL()		Os_Enter_Critical()
#define  OS_EXIT_CRITICAL()			Os_Exit_Critical()

/*
C语言无NULL，宏定义NULL
*/
#ifndef NULL
   #define NULL (void*)0
#endif


/*
打印那个.c文件和多少行,如果不让程序出现LOG打印，屏蔽#define __DEBUG__ 
*/
#define __DEBUG__  
#ifdef __DEBUG__  

#define DEBUG_Printf(format,...) printf("File: "__FILE__", Line: %05d: "format"", __LINE__, ##__VA_ARGS__)  

#else  
#define DEBUG_Printf(format,...)  
#endif  

typedef enum{
	
	ProData0 = 0,
	ProData1,
	ProData2,
	ProData3,
	ProData4,
	ProData5,
	ProData6,
	ProData7,
	ProData8,
	ProData9,
	ProData10,
	ProData11,
	ProData12,
	ProData13,
	ProData14,
	ProData15,	
	ProDataNum

}MAIN_BOARD_PRO_DATA;




/*程序框架全局声明*/

typedef struct
{
	   /*设备状态*/	
	  struct
		{
			
		   INT8U FirstPowerPara;
			 INT8U Self3sStartFlag;
			 INT8U Self3sFlag;
			 INT8U Self3sEndFlag;
			 INT8U EnForce;       //强制
			 enum
			 {

				 DeviceStandy = 0,
				 DeviceRinse,
				 DeviceWork,
				 DeviceLock,

			 }Status;
			 
	
		 }Run;
	 
	 /*内部设备*/
	 struct
	 {
		  struct
			{
				INT8U ReDanState;         // 低水位1，高水位2
				INT8U Temp;               //温度
			}SelfTest;
			
			struct
			{
			
				INT16U Time0;      //时间
				INT8U  Time0Flag;  //时间
				INT16U Time1;      //时间
				INT8U  Time1Flag;  //时间
				INT8U  Enable;      //所有功能禁用0,使能1
			  INT8U  Priority;    //优先级：新膜冲洗功能4＞常温水取水功能3=热水取水功能3＞头杯水回流功能2＞热胆补水功能1
				INT8U  Finish;
			}NewMembraneRinse;
			
		
			
			struct
			{
				INT8U StopTimeCount;
				INT8U StopFlag;
				INT8U HotWaterPriority;    //热水优先级为3，默认0
        INT8U ColdWaterPriority;   //冷水优先级为3，默认0 		
				
				
			}GetWater;
			
			
			
			struct
			{
				INT8U  StartFlag;
        INT8U  StopFlag;
				INT8U  TimeStartCount;
				INT32U TimeStopCount;
				INT8U  Priority;            //回流优先级为2，默认0 
			}HeadWaterReturn;             //头杯水回流
			
			struct
			{
				INT8U OnFlag;
				INT8U OffFlag;
				INT8U Protect;    //保护
			}MakeWater;
			
			struct 
			{
				INT8U Low;
				INT8U LowHigh;
				INT16U Time;       
			}SupplyWater;
		 

			
				struct
			 {
				 INT32U CpfLife1Year;
				 INT32U DfLife5Year;
				 INT32U CpfLifeMotor800Time;
				 INT32U DfLifeMotor800Time;
				 INT32U CpfLifeMotor1000Time;
				 INT32U DfLifeMotor1000Time;
				 INT8U  CpfLife0Flag;
				 INT8U  DfLife0Flag;
				 INT8U  DfFilterResetPara;         // =0：复位前，=0XBB 复位后
				 INT8U  FilterResetMenuState;		
				 INT8U  FilterResetMenu10sEndFlag;	
				 INT16U Time5MinCount;
				 INT32U Time1DayCount;
				 INT8U  StartFlag; 
				 /*RFID锁定*/				
				 INT8U   RfidLockState;						 
					 enum
					{
						Null = 0,
						Cpf = 1,
						Df,
						Normal
					}FilterResetMenu;
				 
			 }Memory;
			
		  struct
      {
				INT8U Temp95;
				INT8U Temp60;
				INT8U Temp45;
				
				INT8U SetTemp;           //设定温度
				INT8U HighSeaLevelTemp;  
				INT8U AutoTemp;
							
			}Keep;
			
			struct
			{
				 INT8U Temp;
				 INT8U TimeCount;
				 INT8U StartFlag;
				 INT8U Finish;
				 INT8U Condition;
				 INT8U Clear;
				
			}BoilPointStudy;
			
			struct
			{
				INT8U Set;
				
			}RecoverFactory;
			
			struct
			{
				INT8U Set;
				
			}EngineerMode;
					
			
	 }InControl;
	 
	 
	 	/*外部设备*/
	 struct
	 {

		
		  struct
			{
				INT8U RefreshTime;
				INT8U RealTemp;    //实测温度
				
			}TempDisplay;
     			
			struct
			{
				INT8U Temp95;
				INT8U Temp60;
				INT8U Temp45; 
				
				INT8U Protect;
				INT8U BuShuiPriority;   //补水优先级为1，默认0 
				
				INT8U WaterLevelLow;
				INT8U WaterLevelLowFlag;
				INT8U WaterLevelLowHighFlag;
				INT8U WaterLevelHigh;
				INT8U WaterLevelHighFlag;		
				
				INT8U StopHeatFlag;
				
			}ReDan;
			
			struct
			{
				INT8U Heat;
				INT8U Cpf;
				INT8U Df;
			}Led;
			
			struct
			{
				INT8U Enable;  //使能位
				
			}TapCom;
			
			 struct
			{
				INT8U Time;
				
			}RfidCom;
			
			struct
			{
				INT8U Cpf;
				INT8U Df;
				INT8U Heat;
			}Key;
		 
	 }OutControl;
	 
}DEVICE_INFOR;

extern	DEVICE_INFOR  volatile  DeviceInfor;


extern void  Os_Enter_Critical(void);
extern void  Os_Exit_Critical(void);

extern void Device_Infor_Init(void);

extern void New_Filter_Rinse(void);

extern void New_Filter_Rinse_Time(void);

extern void Api_3S_Task(void);

extern void Time_Base_1S_Task(void *pMsg);




#endif



