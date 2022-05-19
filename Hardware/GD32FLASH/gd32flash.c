
#include "define.h"

FLASH_U Flash_U;

/*��ʱ����*/
INT8U FlashParaBuffer[PARA_NUMBER] = {0};   // �洢Ҫд�� FLASH �����Ļ���


/*
/////////////////////////////////////////////////////////////////////////////////////////////////////////
* ���� : Para_UpdateFLASH_Programs
*------------------------------------------------
* ���� : ������д�� FLASH
*------------------------------------------------
* ���� : Para	:Ҫд��Ĳ���
		 Address:������ַ
*------------------------------------------------
* ���� : none.
*------------------------------------------------
* ���� : 
*------------------------------------------------
* ע�� : none.
*------------------------------------------------
*/

void Para_UpdateFLASH_Programs(INT32U F_Add,INT32U Para)
{
		INT16U	i;
	  INT32U   EraseCounter;
	  INT32U   address = 0x00U;
	  /* calculate the number of page to be programmed/erased */
    INT32U PageNum = (FMC_WRITE_END_ADDR - FMC_WRITE_START_ADDR) / FMC_PAGE_SIZE;
		INT32U	*ptrd;
		FLASH_U	_Buff1;

		
		//---	�� FLASH �д洢�����ݶ��뻺�棬�Ա������ҳ����	---
		ptrd = (uint32_t*)FMC_WRITE_START_ADDR;
		for(i = 0; i < FMC_BUFSIZE; i+=4)
		{
				_Buff1.Val = *ptrd;
				FlashParaBuffer[i]		= _Buff1.DATE_S.Buf[0];
				FlashParaBuffer[i+1]	= _Buff1.DATE_S.Buf[1];
				FlashParaBuffer[i+2]	= _Buff1.DATE_S.Buf[2];
				FlashParaBuffer[i+3]	= _Buff1.DATE_S.Buf[3];

				ptrd++;
		}

	////////////////////////////////////////////////////////////////
	//---	��ҳ����	---
    /* unlock the flash program/erase controller */
    fmc_unlock();

    /* clear all pending flags */
    fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_WPERR | FMC_FLAG_PGERR);
    
    /* erase the flash pages */
    for(EraseCounter = 0U; EraseCounter < PageNum; EraseCounter++){
        fmc_page_erase(FMC_WRITE_START_ADDR + (FMC_PAGE_SIZE * EraseCounter));
        fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_WPERR | FMC_FLAG_PGERR);
    }

    /* lock the main FMC after the erase operation */
    fmc_lock();

	////////////////////////////////////////////////////////////////
	//---	��������	---
	_Buff1.Val = Para;
	FlashParaBuffer[F_Add]		= _Buff1.DATE_S.Buf[0];
	FlashParaBuffer[F_Add+1]	= _Buff1.DATE_S.Buf[1];
	FlashParaBuffer[F_Add+2]	= _Buff1.DATE_S.Buf[2];
	FlashParaBuffer[F_Add+3]	= _Buff1.DATE_S.Buf[3];
	
	////////////////////////////////////////////////////////////////
	//---	д�� FLASH	---
    /* unlock the flash program/erase controller */
    fmc_unlock();

    address = FMC_WRITE_START_ADDR;
    //while(Address < FLASH_WRITE_END_ADDR)
	  for(i = 0; i <= FMC_BUFSIZE; i+=4)
    {
			_Buff1.DATE_S.Buf[0] = FlashParaBuffer[i];
			_Buff1.DATE_S.Buf[1] = FlashParaBuffer[i+1];
			_Buff1.DATE_S.Buf[2] = FlashParaBuffer[i+2];
			_Buff1.DATE_S.Buf[3] = FlashParaBuffer[i+3];
			
      fmc_word_program(address, _Buff1.Val);
      address = address + 4; 
      fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_WPERR | FMC_FLAG_PGERR);  
    }
    
    /* lock the main FMC after the program operation */
    fmc_lock();
}




/*ģ��ṹ���ʼ��*/
void Flash_Idev_Struct_Init(void)
{	
   
 	Flash_U.Val = 0;

}

void Flash_Idev_Init(void)
{
	 INT8U i;
	 INT32U *ptrd;
		
	 Flash_Idev_Struct_Init();  //������ʼ��
	 	
	 ptrd = (INT32U *)FMC_WRITE_START_ADDR;

	 Flash_U.Val = *ptrd;      //��ȡ��ֵַ
	
	 /*��FLASH����*/
	 if( ( Flash_U.DATE_S.Buf[0] == ERASE_FLASH )&& ( Flash_U.DATE_S.Buf[1] == 1 ) && ( Flash_U.DATE_S.Buf[2] == 2 ) &&( Flash_U.DATE_S.Buf[3] == 3 ))
	 {
		   /*CPF��о����ʱ��*/
       ptrd = (INT32U *)( FMC_WRITE_START_ADDR + CPF_LIFE_FLASH_ADD );
		   Flash_U.Val = *ptrd;      //��ȡ��ֵַ
		   DeviceInfor.InControl.Memory.CpfLife1Year =  Flash_U.Val;             //ʣ�µ�CPF����ʱ��
		 
		   /*DF��о����ʱ��*/
       ptrd = (INT32U *)( FMC_WRITE_START_ADDR + DF_LIFE_FLASH_ADD );
		   Flash_U.Val = *ptrd;      //��ȡ��ֵַ
		   DeviceInfor.InControl.Memory.DfLife5Year  =  Flash_U.Val;             //ʣ�µ�DF����ʱ��
		 
		 	 /*CPF_MOTOT800��о����ʱ��*/
       ptrd = (INT32U *)( FMC_WRITE_START_ADDR + CPF_LIFE_MOTOR800_FLASH_ADD );
		   Flash_U.Val = *ptrd;      //��ȡ��ֵַ
		   DeviceInfor.InControl.Memory.CpfLifeMotor800Time =  Flash_U.Val;     //ʣ�µ�CPF_MOTOT800����ʱ��
		 
		   /*DF_MOTOT800��о����ʱ��*/
       ptrd = (INT32U *)( FMC_WRITE_START_ADDR + DF_LIFE_MOTOR800_FLASH_ADD );
		   Flash_U.Val = *ptrd;      //��ȡ��ֵַ
		   DeviceInfor.InControl.Memory.DfLifeMotor800Time =  Flash_U.Val;      //ʣ�µ�DF_MOTOT800����ʱ��
		 
		 	 /*CPF_MOTOT1000��о����ʱ��*/
       ptrd = (INT32U *)( FMC_WRITE_START_ADDR + CPF_LIFE_MOTOR1000_FLASH_ADD );
		   Flash_U.Val = *ptrd;      //��ȡ��ֵַ
		   DeviceInfor.InControl.Memory.CpfLifeMotor1000Time =  Flash_U.Val;     //ʣ�µ�CPF_MOTOT1000����ʱ��
			 
			 /*DF_MOTOT1000��о����ʱ��*/
       ptrd = (INT32U *)( FMC_WRITE_START_ADDR + DF_LIFE_MOTOR1000_FLASH_ADD );
		   Flash_U.Val = *ptrd;      //��ȡ��ֵַ
		   DeviceInfor.InControl.Memory.DfLifeMotor1000Time =  Flash_U.Val;       //ʣ�µ�DF_MOTOT1000����ʱ��
		 	 
		   /*�״��ϵ��ϴ*/
       ptrd = (INT32U *)( FMC_WRITE_START_ADDR + FIRST_POWER_FLASH_ADD );
		   Flash_U.Val = *ptrd;      //��ȡ��ֵַ
		   DeviceInfor.Run.FirstPowerPara  =  (INT8U)Flash_U.Val;                 //�״��ϵ��ϴ
  
		   /*DF��о��λ��־*/
       ptrd = (INT32U *)( FMC_WRITE_START_ADDR + DF_FILTER_RESET_FLASH_ADD );
		   Flash_U.Val = *ptrd;      //��ȡ��ֵַ
		   DeviceInfor.InControl.Memory.DfFilterResetPara   =  (INT8U)Flash_U.Val; //DF��λ��־
		 
		 	 /*RFID����״̬*/
       ptrd = (INT32U *)( FMC_WRITE_START_ADDR + RFID_LOCK_FLASH_ADD );
		   Flash_U.Val = *ptrd;      //��ȡ��ֵַ
		   DeviceInfor.InControl.Memory.RfidLockState   =  (INT8U)Flash_U.Val; //RFID����״̬
			 
			 /*�е��¶�*/
       ptrd = (INT32U *)( FMC_WRITE_START_ADDR + BOIL_POINT_FLASH_ADD );
		   Flash_U.Val = *ptrd;      //��ȡ��ֵַ
		   DeviceInfor.InControl.BoilPointStudy.Temp  =  (INT8U)Flash_U.Val;       //�е��¶�
			 
			 /*��Ĥ��ϴ*/
       ptrd = (INT32U *)( FMC_WRITE_START_ADDR + NEW_MEMBRANE_RINSE_FLASH_ADD );
		   Flash_U.Val = *ptrd;      //��ȡ��ֵַ
		   DeviceInfor.InControl.NewMembraneRinse.Finish  =  (INT8U)Flash_U.Val;      //��Ĥ��ϴʱ��
			 
			 
			 
	 }
	 
	  /*дFLASH����*/
	 else
	 {
		     OS_ENTER_CRITICAL();  //����

		     /*��һ��д��CPF��о����ʱ��*/
//		    DeviceInfor.InControl.Memory.CpfLife12Mon = 0X01234567;
		    Flash_U.Val =  DeviceInfor.InControl.Memory.CpfLife1Year;
		    Para_UpdateFLASH_Programs( CPF_LIFE_FLASH_ADD,   Flash_U.Val);		 
		 
		    /*��һ��д��DF��о����ʱ��*/
		    Flash_U.Val =   DeviceInfor.InControl.Memory.DfLife5Year;
		    Para_UpdateFLASH_Programs( DF_LIFE_FLASH_ADD,  Flash_U.Val);
		 
		 		 /*��һ��д��CPF��оMOTOR800����ʱ��*/
		    Flash_U.Val =  DeviceInfor.InControl.Memory.CpfLifeMotor800Time;
		    Para_UpdateFLASH_Programs( CPF_LIFE_MOTOR800_FLASH_ADD,   Flash_U.Val);	
		 
		 		 /*��һ��д��DF��оMOTOR800����ʱ��*/
		    Flash_U.Val =  DeviceInfor.InControl.Memory.DfLifeMotor800Time;
		    Para_UpdateFLASH_Programs( DF_LIFE_MOTOR800_FLASH_ADD,   Flash_U.Val);	
		 
		  	/*��һ��д��CPF��оMOTOR1000����ʱ��*/
		    Flash_U.Val =  DeviceInfor.InControl.Memory.CpfLifeMotor1000Time;
		    Para_UpdateFLASH_Programs( CPF_LIFE_MOTOR1000_FLASH_ADD,   Flash_U.Val);	
		 
		 		/*��һ��д��DF��оMOTOR1000����ʱ��*/
		    Flash_U.Val =  DeviceInfor.InControl.Memory.DfLifeMotor1000Time;
		    Para_UpdateFLASH_Programs( DF_LIFE_MOTOR1000_FLASH_ADD,   Flash_U.Val);
	    		 
		 		/*��һ��д���ϵ��־*/
		    Flash_U.Val =  DeviceInfor.Run.FirstPowerPara;
		    Para_UpdateFLASH_Programs( FIRST_POWER_FLASH_ADD,  Flash_U.Val);
		 
		 		 /*��һ��д��DF��о��λ��־*/
		    Flash_U.Val =    DeviceInfor.InControl.Memory.DfFilterResetPara;
		    Para_UpdateFLASH_Programs( DF_FILTER_RESET_FLASH_ADD,  Flash_U.Val);
		 
		 		/*RFID����״̬*/
		     Flash_U.Val =  DeviceInfor.InControl.Memory.RfidLockState;
		    Para_UpdateFLASH_Programs( RFID_LOCK_FLASH_ADD,  Flash_U.Val);
				
				 /*�е��¶�*/
		    Flash_U.Val =   DeviceInfor.InControl.BoilPointStudy.Temp;
		    Para_UpdateFLASH_Programs( BOIL_POINT_FLASH_ADD,  Flash_U.Val);
				
				/*��Ĥ��ϴ*/
		    Flash_U.Val =  DeviceInfor.InControl.NewMembraneRinse.Finish ;
		    Para_UpdateFLASH_Programs( NEW_MEMBRANE_RINSE_FLASH_ADD,  Flash_U.Val);
				

		 
		    /*��һ��д�������־*/	
				 Flash_U.DATE_S.Buf[0]= 0;  
				 Flash_U.DATE_S.Buf[1]= 1;  
				 Flash_U.DATE_S.Buf[2]= 2;  
				 Flash_U.DATE_S.Buf[3]= 3; 
				 Para_UpdateFLASH_Programs( MARK0_FLASH_ADD,  Flash_U.Val);
				 

         OS_EXIT_CRITICAL();  //����
		
				 
	 }
	
	

}



