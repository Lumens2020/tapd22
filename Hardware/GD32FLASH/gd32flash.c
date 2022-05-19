
#include "define.h"

FLASH_U Flash_U;

/*临时缓存*/
INT8U FlashParaBuffer[PARA_NUMBER] = {0};   // 存储要写入 FLASH 参数的缓存


/*
/////////////////////////////////////////////////////////////////////////////////////////////////////////
* 函数 : Para_UpdateFLASH_Programs
*------------------------------------------------
* 描述 : 将参数写入 FLASH
*------------------------------------------------
* 参数 : Para	:要写入的参数
		 Address:参数地址
*------------------------------------------------
* 返回 : none.
*------------------------------------------------
* 调用 : 
*------------------------------------------------
* 注意 : none.
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

		
		//---	将 FLASH 中存储的数据读入缓存，以便后面整页擦除	---
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
	//---	整页擦除	---
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
	//---	参数移入	---
	_Buff1.Val = Para;
	FlashParaBuffer[F_Add]		= _Buff1.DATE_S.Buf[0];
	FlashParaBuffer[F_Add+1]	= _Buff1.DATE_S.Buf[1];
	FlashParaBuffer[F_Add+2]	= _Buff1.DATE_S.Buf[2];
	FlashParaBuffer[F_Add+3]	= _Buff1.DATE_S.Buf[3];
	
	////////////////////////////////////////////////////////////////
	//---	写入 FLASH	---
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




/*模块结构体初始化*/
void Flash_Idev_Struct_Init(void)
{	
   
 	Flash_U.Val = 0;

}

void Flash_Idev_Init(void)
{
	 INT8U i;
	 INT32U *ptrd;
		
	 Flash_Idev_Struct_Init();  //参数初始化
	 	
	 ptrd = (INT32U *)FMC_WRITE_START_ADDR;

	 Flash_U.Val = *ptrd;      //读取地址值
	
	 /*读FLASH参数*/
	 if( ( Flash_U.DATE_S.Buf[0] == ERASE_FLASH )&& ( Flash_U.DATE_S.Buf[1] == 1 ) && ( Flash_U.DATE_S.Buf[2] == 2 ) &&( Flash_U.DATE_S.Buf[3] == 3 ))
	 {
		   /*CPF滤芯寿命时间*/
       ptrd = (INT32U *)( FMC_WRITE_START_ADDR + CPF_LIFE_FLASH_ADD );
		   Flash_U.Val = *ptrd;      //读取地址值
		   DeviceInfor.InControl.Memory.CpfLife1Year =  Flash_U.Val;             //剩下的CPF寿命时间
		 
		   /*DF滤芯寿命时间*/
       ptrd = (INT32U *)( FMC_WRITE_START_ADDR + DF_LIFE_FLASH_ADD );
		   Flash_U.Val = *ptrd;      //读取地址值
		   DeviceInfor.InControl.Memory.DfLife5Year  =  Flash_U.Val;             //剩下的DF寿命时间
		 
		 	 /*CPF_MOTOT800滤芯寿命时间*/
       ptrd = (INT32U *)( FMC_WRITE_START_ADDR + CPF_LIFE_MOTOR800_FLASH_ADD );
		   Flash_U.Val = *ptrd;      //读取地址值
		   DeviceInfor.InControl.Memory.CpfLifeMotor800Time =  Flash_U.Val;     //剩下的CPF_MOTOT800寿命时间
		 
		   /*DF_MOTOT800滤芯寿命时间*/
       ptrd = (INT32U *)( FMC_WRITE_START_ADDR + DF_LIFE_MOTOR800_FLASH_ADD );
		   Flash_U.Val = *ptrd;      //读取地址值
		   DeviceInfor.InControl.Memory.DfLifeMotor800Time =  Flash_U.Val;      //剩下的DF_MOTOT800寿命时间
		 
		 	 /*CPF_MOTOT1000滤芯寿命时间*/
       ptrd = (INT32U *)( FMC_WRITE_START_ADDR + CPF_LIFE_MOTOR1000_FLASH_ADD );
		   Flash_U.Val = *ptrd;      //读取地址值
		   DeviceInfor.InControl.Memory.CpfLifeMotor1000Time =  Flash_U.Val;     //剩下的CPF_MOTOT1000寿命时间
			 
			 /*DF_MOTOT1000滤芯寿命时间*/
       ptrd = (INT32U *)( FMC_WRITE_START_ADDR + DF_LIFE_MOTOR1000_FLASH_ADD );
		   Flash_U.Val = *ptrd;      //读取地址值
		   DeviceInfor.InControl.Memory.DfLifeMotor1000Time =  Flash_U.Val;       //剩下的DF_MOTOT1000寿命时间
		 	 
		   /*首次上电冲洗*/
       ptrd = (INT32U *)( FMC_WRITE_START_ADDR + FIRST_POWER_FLASH_ADD );
		   Flash_U.Val = *ptrd;      //读取地址值
		   DeviceInfor.Run.FirstPowerPara  =  (INT8U)Flash_U.Val;                 //首次上电冲洗
  
		   /*DF滤芯复位标志*/
       ptrd = (INT32U *)( FMC_WRITE_START_ADDR + DF_FILTER_RESET_FLASH_ADD );
		   Flash_U.Val = *ptrd;      //读取地址值
		   DeviceInfor.InControl.Memory.DfFilterResetPara   =  (INT8U)Flash_U.Val; //DF复位标志
		 
		 	 /*RFID锁定状态*/
       ptrd = (INT32U *)( FMC_WRITE_START_ADDR + RFID_LOCK_FLASH_ADD );
		   Flash_U.Val = *ptrd;      //读取地址值
		   DeviceInfor.InControl.Memory.RfidLockState   =  (INT8U)Flash_U.Val; //RFID锁定状态
			 
			 /*沸点温度*/
       ptrd = (INT32U *)( FMC_WRITE_START_ADDR + BOIL_POINT_FLASH_ADD );
		   Flash_U.Val = *ptrd;      //读取地址值
		   DeviceInfor.InControl.BoilPointStudy.Temp  =  (INT8U)Flash_U.Val;       //沸点温度
			 
			 /*新膜冲洗*/
       ptrd = (INT32U *)( FMC_WRITE_START_ADDR + NEW_MEMBRANE_RINSE_FLASH_ADD );
		   Flash_U.Val = *ptrd;      //读取地址值
		   DeviceInfor.InControl.NewMembraneRinse.Finish  =  (INT8U)Flash_U.Val;      //新膜冲洗时间
			 
			 
			 
	 }
	 
	  /*写FLASH参数*/
	 else
	 {
		     OS_ENTER_CRITICAL();  //加锁

		     /*第一次写入CPF滤芯寿命时间*/
//		    DeviceInfor.InControl.Memory.CpfLife12Mon = 0X01234567;
		    Flash_U.Val =  DeviceInfor.InControl.Memory.CpfLife1Year;
		    Para_UpdateFLASH_Programs( CPF_LIFE_FLASH_ADD,   Flash_U.Val);		 
		 
		    /*第一次写入DF滤芯寿命时间*/
		    Flash_U.Val =   DeviceInfor.InControl.Memory.DfLife5Year;
		    Para_UpdateFLASH_Programs( DF_LIFE_FLASH_ADD,  Flash_U.Val);
		 
		 		 /*第一次写入CPF滤芯MOTOR800寿命时间*/
		    Flash_U.Val =  DeviceInfor.InControl.Memory.CpfLifeMotor800Time;
		    Para_UpdateFLASH_Programs( CPF_LIFE_MOTOR800_FLASH_ADD,   Flash_U.Val);	
		 
		 		 /*第一次写入DF滤芯MOTOR800寿命时间*/
		    Flash_U.Val =  DeviceInfor.InControl.Memory.DfLifeMotor800Time;
		    Para_UpdateFLASH_Programs( DF_LIFE_MOTOR800_FLASH_ADD,   Flash_U.Val);	
		 
		  	/*第一次写入CPF滤芯MOTOR1000寿命时间*/
		    Flash_U.Val =  DeviceInfor.InControl.Memory.CpfLifeMotor1000Time;
		    Para_UpdateFLASH_Programs( CPF_LIFE_MOTOR1000_FLASH_ADD,   Flash_U.Val);	
		 
		 		/*第一次写入DF滤芯MOTOR1000寿命时间*/
		    Flash_U.Val =  DeviceInfor.InControl.Memory.DfLifeMotor1000Time;
		    Para_UpdateFLASH_Programs( DF_LIFE_MOTOR1000_FLASH_ADD,   Flash_U.Val);
	    		 
		 		/*第一次写入上电标志*/
		    Flash_U.Val =  DeviceInfor.Run.FirstPowerPara;
		    Para_UpdateFLASH_Programs( FIRST_POWER_FLASH_ADD,  Flash_U.Val);
		 
		 		 /*第一次写入DF滤芯复位标志*/
		    Flash_U.Val =    DeviceInfor.InControl.Memory.DfFilterResetPara;
		    Para_UpdateFLASH_Programs( DF_FILTER_RESET_FLASH_ADD,  Flash_U.Val);
		 
		 		/*RFID锁定状态*/
		     Flash_U.Val =  DeviceInfor.InControl.Memory.RfidLockState;
		    Para_UpdateFLASH_Programs( RFID_LOCK_FLASH_ADD,  Flash_U.Val);
				
				 /*沸点温度*/
		    Flash_U.Val =   DeviceInfor.InControl.BoilPointStudy.Temp;
		    Para_UpdateFLASH_Programs( BOIL_POINT_FLASH_ADD,  Flash_U.Val);
				
				/*新膜冲洗*/
		    Flash_U.Val =  DeviceInfor.InControl.NewMembraneRinse.Finish ;
		    Para_UpdateFLASH_Programs( NEW_MEMBRANE_RINSE_FLASH_ADD,  Flash_U.Val);
				

		 
		    /*第一次写入参数标志*/	
				 Flash_U.DATE_S.Buf[0]= 0;  
				 Flash_U.DATE_S.Buf[1]= 1;  
				 Flash_U.DATE_S.Buf[2]= 2;  
				 Flash_U.DATE_S.Buf[3]= 3; 
				 Para_UpdateFLASH_Programs( MARK0_FLASH_ADD,  Flash_U.Val);
				 

         OS_EXIT_CRITICAL();  //解锁
		
				 
	 }
	
	

}



