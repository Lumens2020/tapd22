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

/*ȫ�ֶ���*/
//#define PILIANG_PRODUCTION              //����������Ҫ����������ÿ��Ź�
                          
#define KAIFA_DEBUG                   //����BUG

//#define KAIFA_TEST                    //��������

//#define WDT_ENABLE                    //ʹ�ܿ��Ź���LVR����2.9V

#define ERASE_FLASH  0                  //�������������ⲻͬ�������洢����Ҫ�����û�0


/***********************************��������*****************************/
#ifdef  PILIANG_PRODUCTION

#define  POWER_PARA                0XAA     //=0XAA:�ϵ�ǰ����  =0: �ϵ�����
#define  DF_FILTER_RESET_PARA      0XBB     //=0�� ��о��λǰ   =0XBB: ��о��λ��
#define  NEW_MEMBRANE_RINSE_PARA   0XCC     //=0�� ��ϴǰ       =0XCC: ��ϴ��
#define  BOIL_POINT_STUDY_PARA     0        //=0: �е�ѧϰǰ =ʵ��ֵ���е�ѧϰ��

/*����洢*/
#define CPF_LIFE_FULL_1YEAR       365      //CPF����
#define DF_LIFE_FULL_5YEAR        1825     //DF����
/*�����Ӵ洢*/
#define CPF_MOTOR800_WORK_TIME    2750      //���� 
#define DF_MOTOR800_WORK_TIME     13750     //���� 
/*�����Ӵ洢*/
#define CPF_MOTOR1000_WORK_TIME    2200     //���� 
#define DF_MOTOR1000_WORK_TIME     11000    //���� 


#define TIME_70S                   70        //��λS
#define TIME_2MIN                  2*60      //��λS
#define TIME_18MIN                 18*60     //��λS,
#define TIME_24HOUR                24*3600   //��λS,

#endif
/***********************************��������*****************************/

/***********************************��������*****************************/
#ifdef KAIFA_DEBUG 

#define  POWER_PARA                0XAA     //=0XAA:�ϵ�ǰ����  =0: �ϵ�����
#define  DF_FILTER_RESET_PARA      0XBB     //=0�� ��о��λǰ   =0XBB: ��о��λ��
#define  NEW_MEMBRANE_RINSE_PARA   0XCC     //=0�� ��ϴǰ       =0XCC: ��ϴ��
#define  BOIL_POINT_STUDY_PARA     0X00     //=0: �е�ѧϰǰ    =ʵ��ֵ���е�ѧϰ��

/*����洢*/
#define CPF_LIFE_FULL_1YEAR       365      //CPF����
#define DF_LIFE_FULL_5YEAR        1825     //DF����
/*�����Ӵ洢*/
#define CPF_MOTOR800_WORK_TIME    2750      //���� 
#define DF_MOTOR800_WORK_TIME     13750     //���� 
/*�����Ӵ洢*/
#define CPF_MOTOR1000_WORK_TIME    2200     //���� 
#define DF_MOTOR1000_WORK_TIME     11000    //���� 


#define TIME_2S                    100       //��λMS,20MSһ��
#define TIME_5S                    250       //��λMS,20MSһ��
#define TIME_10S                   500       //��λMS,20MSһ��
#define TIME_12S                   12        //��λS
#define TIME_30S                   30        //��λS
#define TIME_70S                   70        //��λS
#define TIME_2MIN                  0         //��λS������1S
#define TIME_5MIN                  5*60      //��λS
#define TIME_18MIN                 0         //��λS,����1S
#define TIME_250MIN                250*60    //��λS,����1����
#define TIME_24HOUR                24*60*60  //��λS,����1����
#define TIME_1DAY                  24*60*60  //��λS,����1����
#define TIME_1Month                30*24     //��λ��,����1����

#define BOIL_POINT_97C             97        //97��
#define KEEP_TEMP_95C              95        //95��
#define KEEP_TEMP_60C              60        //60��
#define KEEP_TEMP_45C              45        //45��



#endif
/***********************************��������*****************************/





/*�������ж�*/
#define  OS_ENTER_CRITICAL()		Os_Enter_Critical()
#define  OS_EXIT_CRITICAL()			Os_Exit_Critical()

/*
C������NULL���궨��NULL
*/
#ifndef NULL
   #define NULL (void*)0
#endif


/*
��ӡ�Ǹ�.c�ļ��Ͷ�����,������ó������LOG��ӡ������#define __DEBUG__ 
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




/*������ȫ������*/

typedef struct
{
	   /*�豸״̬*/	
	  struct
		{
			
		   INT8U FirstPowerPara;
			 INT8U Self3sStartFlag;
			 INT8U Self3sFlag;
			 INT8U Self3sEndFlag;
			 INT8U EnForce;       //ǿ��
			 enum
			 {

				 DeviceStandy = 0,
				 DeviceRinse,
				 DeviceWork,
				 DeviceLock,

			 }Status;
			 
	
		 }Run;
	 
	 /*�ڲ��豸*/
	 struct
	 {
		  struct
			{
				INT8U ReDanState;         // ��ˮλ1����ˮλ2
				INT8U Temp;               //�¶�
			}SelfTest;
			
			struct
			{
			
				INT16U Time0;      //ʱ��
				INT8U  Time0Flag;  //ʱ��
				INT16U Time1;      //ʱ��
				INT8U  Time1Flag;  //ʱ��
				INT8U  Enable;      //���й��ܽ���0,ʹ��1
			  INT8U  Priority;    //���ȼ�����Ĥ��ϴ����4������ˮȡˮ����3=��ˮȡˮ����3��ͷ��ˮ��������2���ȵ���ˮ����1
				INT8U  Finish;
			}NewMembraneRinse;
			
		
			
			struct
			{
				INT8U StopTimeCount;
				INT8U StopFlag;
				INT8U HotWaterPriority;    //��ˮ���ȼ�Ϊ3��Ĭ��0
        INT8U ColdWaterPriority;   //��ˮ���ȼ�Ϊ3��Ĭ��0 		
				
				
			}GetWater;
			
			
			
			struct
			{
				INT8U  StartFlag;
        INT8U  StopFlag;
				INT8U  TimeStartCount;
				INT32U TimeStopCount;
				INT8U  Priority;            //�������ȼ�Ϊ2��Ĭ��0 
			}HeadWaterReturn;             //ͷ��ˮ����
			
			struct
			{
				INT8U OnFlag;
				INT8U OffFlag;
				INT8U Protect;    //����
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
				 INT8U  DfFilterResetPara;         // =0����λǰ��=0XBB ��λ��
				 INT8U  FilterResetMenuState;		
				 INT8U  FilterResetMenu10sEndFlag;	
				 INT16U Time5MinCount;
				 INT32U Time1DayCount;
				 INT8U  StartFlag; 
				 /*RFID����*/				
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
				
				INT8U SetTemp;           //�趨�¶�
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
	 
	 
	 	/*�ⲿ�豸*/
	 struct
	 {

		
		  struct
			{
				INT8U RefreshTime;
				INT8U RealTemp;    //ʵ���¶�
				
			}TempDisplay;
     			
			struct
			{
				INT8U Temp95;
				INT8U Temp60;
				INT8U Temp45; 
				
				INT8U Protect;
				INT8U BuShuiPriority;   //��ˮ���ȼ�Ϊ1��Ĭ��0 
				
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
				INT8U Enable;  //ʹ��λ
				
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



