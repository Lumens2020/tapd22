
#ifndef _GD32FLASH_H
#define _GD32FLASH_H

#include "stdint.h"



/*
常量
0x0801FFFF：对应的127K
0x08010000: 对应的64K
*/
#define FMC_PAGE_SIZE            ((uint16_t)0x400U)
#define FMC_WRITE_START_ADDR     ((uint32_t)0x0800FC00)
#define FMC_WRITE_END_ADDR       ((uint32_t)0x08010000)     
#define FMC_BUFSIZE              FMC_WRITE_END_ADDR - FMC_WRITE_START_ADDR

#define  PARA_NUMBER					  50					// 参数总数

/*仪器参数在FMC_Para_Buffer中存储位置*/
#define  MARK0_FLASH_ADD  0      //第一次写参数标志位0
#define  MARK1_FLASH_ADD  1      //第二次写参数标志位1
#define  MARK2_FLASH_ADD  2      //第三次写参数标志位2
#define  MARK3_FLASH_ADD  3      //第四次写参数标志位3

/*数据存储位置*/
#define  CPF_LIFE_FLASH_ADD            4    //CPF滤芯寿命
#define  DF_LIFE_FLASH_ADD             8    //DF滤芯寿命
#define  CPF_LIFE_MOTOR800_FLASH_ADD   12   //CPF滤芯800寿命
#define  DF_LIFE_MOTOR800_FLASH_ADD    16   //DF滤芯800寿命
#define  CPF_LIFE_MOTOR1000_FLASH_ADD  20   //CPF滤芯1000寿命
#define  DF_LIFE_MOTOR1000_FLASH_ADD   24   //DF滤芯1000寿命
#define  FIRST_POWER_FLASH_ADD         28   //首次上电
#define  DF_FILTER_RESET_FLASH_ADD     32   //DF滤芯复位标志
#define  RFID_LOCK_FLASH_ADD           36   //RFID锁定状态
#define  BOIL_POINT_FLASH_ADD          40   //沸点温度
#define  NEW_MEMBRANE_RINSE_FLASH_ADD  44   //新膜冲洗







/*内存共享*/
typedef union{

	  struct{
			
		    INT8U Buf[4];
		}DATE_S;
		
		INT32U Val;
}FLASH_U;

extern FLASH_U Flash_U;


extern void Flash_Idev_Init(void);

extern void Para_UpdateFLASH_Programs(INT32U F_Add,INT32U Para);

#endif
