
#ifndef _GD32FLASH_H
#define _GD32FLASH_H

#include "stdint.h"



/*
����
0x0801FFFF����Ӧ��127K
0x08010000: ��Ӧ��64K
*/
#define FMC_PAGE_SIZE            ((uint16_t)0x400U)
#define FMC_WRITE_START_ADDR     ((uint32_t)0x0800FC00)
#define FMC_WRITE_END_ADDR       ((uint32_t)0x08010000)     
#define FMC_BUFSIZE              FMC_WRITE_END_ADDR - FMC_WRITE_START_ADDR

#define  PARA_NUMBER					  50					// ��������

/*����������FMC_Para_Buffer�д洢λ��*/
#define  MARK0_FLASH_ADD  0      //��һ��д������־λ0
#define  MARK1_FLASH_ADD  1      //�ڶ���д������־λ1
#define  MARK2_FLASH_ADD  2      //������д������־λ2
#define  MARK3_FLASH_ADD  3      //���Ĵ�д������־λ3

/*���ݴ洢λ��*/
#define  CPF_LIFE_FLASH_ADD            4    //CPF��о����
#define  DF_LIFE_FLASH_ADD             8    //DF��о����
#define  CPF_LIFE_MOTOR800_FLASH_ADD   12   //CPF��о800����
#define  DF_LIFE_MOTOR800_FLASH_ADD    16   //DF��о800����
#define  CPF_LIFE_MOTOR1000_FLASH_ADD  20   //CPF��о1000����
#define  DF_LIFE_MOTOR1000_FLASH_ADD   24   //DF��о1000����
#define  FIRST_POWER_FLASH_ADD         28   //�״��ϵ�
#define  DF_FILTER_RESET_FLASH_ADD     32   //DF��о��λ��־
#define  RFID_LOCK_FLASH_ADD           36   //RFID����״̬
#define  BOIL_POINT_FLASH_ADD          40   //�е��¶�
#define  NEW_MEMBRANE_RINSE_FLASH_ADD  44   //��Ĥ��ϴ







/*�ڴ湲��*/
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
