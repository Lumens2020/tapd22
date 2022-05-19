
//************************************************************
//  Copyright (c):北京碧水源膜科技有限公司
//	文件名称	   : main.c
//	作者		     : 耿伟东
//	模块功能	   : TAPD22电控规格书
//  日期         : 2022-02-17
//  芯片         : GD32E230C8T6, 64K-FLASH,8K-RAM
// 	版本		     : V1.0
//  程序框架     ：多任务并发机制+生产者与消费者模型 = 工厂流水线软件架构
// 	更改记录	   : 
//               1、
//*************************************************************


#include "define.h"

/**************************************************

综合：
一、程序思路
1、能独立模块化的模块化
2、减少全局变量，可以用函数取代或者回调
3、不能的话，全局变量重点使用单字节
4、标志位置1或者清0，都是先状态后动作，先内及外
5、单独模块使用生产者与消费者模型，按照例子写



二、程序定义格式
1、函数名第一个字母大写，全局变量第一个字母大写,除了定义函数和结构体有下划线，其他都没有
2、所有定义都用INT8U,INT16U，INT32U（除了库自带的不做修改）
3、枚举和宏定义都大写
4、结构体定义：_DeviceInfor  volatile xdata DeviceInfor;volatile是用来保护主函数与中断函数共用的全局变量
5、局部变量都小写
6、函数指针定义void (*PFTASK)(void *pMsg)及参数指针定义
7、typedef 参照
typedef struct
{
	INT8U save;
	INT8U get;
	INT8U num;
	INT8U dat[QUEUE_NUM];
	
}QUEUE_S;

三、程序整体框架

互斥锁(原子锁)：
程序中一共有MAIN和TIMER0两个独立进程
DeviceInfor.Run.MutexLock = 1;  //加锁
DeviceInfor.Run.MutexLock = 0;  //解锁
1、只在自己的进程不加
2、只读取操作不加
3、涉及到读改写的地方加
4、两个进程修改一样的数据不加
5、重要的加，不重要的不加
6、紧急任务，执行时间长
7、数据的安全保护主要是针对多字节的变量，比如51单片机int类型(2字节)，long类型(4字节)，单字节char类型不用额外保护
8、设置串口0,1中断设置为最高优先级，优先级数字越小级别越高，其次再考虑中断向量号
9、指针指向的地址
**************************************************/


//QUEUE_S Queue;
//INT8U a;
//INT8U b[4] = {0x11,0x22,0x33,0xaa};
//INT8U c[3] = {0x44,0x55,0x66};
//INT8U d[10];


//__IO FlagStatus g_transfer_complete = RESET;

/*模块初始化*/
void Main_Init(void)
{

	
  Bsp_Init();   //板载设备初始化
	
}

void Main_Exc(void)
{
    #ifdef WDT_ENABLE
	  WDT_SetReload();	             //看门狗计数值清0
	  #endif

	  Api_3S_Task();                 //3S自检任务
	  Usart0_Rec_Task();
	  Task_Exec();
	
	
	 
}


int main(void)
{
    Main_Init();
//	  OS_ENTER_CRITICAL();	
//	  OS_EXIT_CRITICAL();	
    while(1)
    {				
			/*对应的标志位志置1，执行*/
      Main_Exc();
		
    }
    return 0;
}