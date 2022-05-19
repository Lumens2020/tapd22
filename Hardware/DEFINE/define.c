#include "define.h"


 DEVICE_INFOR volatile DeviceInfor;


static INT8U CriticalCnt = 0;
/*进入关键代码段*/
void  Os_Enter_Critical(void)
{
	CriticalCnt++;
	__disable_irq();
}	

/*退出关键代码段*/
void  Os_Exit_Critical(void)
{
	 if( CriticalCnt > 0 )
	 {
		  CriticalCnt--;
	 }
	 
	 if( CriticalCnt == 0 )
	 {
		  __enable_irq();
	 }
	
}	



/*
设备信息初始化
标志位状态使用TURE,FALSE
其他数字值代替
*/
void Device_Infor_Init(void)
{
	
/*********************************全局内部信息***********************************/
	  /*设备运行状态*/
	  DeviceInfor.Run.FirstPowerPara      =  POWER_PARA;
	  DeviceInfor.Run.Self3sStartFlag     =  TRUE;
	  DeviceInfor.Run.Self3sFlag          =  FALSE;
	  DeviceInfor.Run.Self3sEndFlag       =  FALSE;
	  DeviceInfor.Run.EnForce             =  0;     
	  DeviceInfor.Run.Status              =  DeviceStandy;
	
	  /*新膜冲洗*/
	  DeviceInfor.InControl.NewMembraneRinse.Time0      = 0;
		DeviceInfor.InControl.NewMembraneRinse.Time0Flag  = FALSE;
	  DeviceInfor.InControl.NewMembraneRinse.Time1      = 0;
	  DeviceInfor.InControl.NewMembraneRinse.Time1Flag  = FALSE;
	  DeviceInfor.InControl.NewMembraneRinse.Enable     = 0;
	  DeviceInfor.InControl.NewMembraneRinse.Priority   = 0;
	  DeviceInfor.InControl.NewMembraneRinse.Finish     = 0;
	
	  /*取水*/
	  DeviceInfor.InControl.GetWater.StopTimeCount     = 0;
		DeviceInfor.InControl.GetWater.StopFlag          = FALSE;
		DeviceInfor.InControl.GetWater.ColdWaterPriority = 0;
		DeviceInfor.InControl.GetWater.HotWaterPriority  = 0;
		
		/*头杯水回流*/
		DeviceInfor.InControl.HeadWaterReturn.StartFlag      = FALSE;
		DeviceInfor.InControl.HeadWaterReturn.StopFlag       = FALSE;
		DeviceInfor.InControl.HeadWaterReturn.TimeStartCount = 0;
		DeviceInfor.InControl.HeadWaterReturn.TimeStopCount  = 0;
		DeviceInfor.InControl.HeadWaterReturn.Priority       = 0;
		
		/*制水*/
	  DeviceInfor.InControl.MakeWater.OnFlag  = FALSE;
		DeviceInfor.InControl.MakeWater.OffFlag = FALSE;
		DeviceInfor.InControl.MakeWater.Protect = 0;
		
		
	  /*滤芯寿命*/
	  DeviceInfor.InControl.Memory.CpfLife1Year         = CPF_LIFE_FULL_1YEAR;
	  DeviceInfor.InControl.Memory.DfLife5Year          = DF_LIFE_FULL_5YEAR;
		DeviceInfor.InControl.Memory.CpfLifeMotor800Time  = CPF_MOTOR800_WORK_TIME;
	  DeviceInfor.InControl.Memory.DfLifeMotor800Time   = DF_MOTOR800_WORK_TIME;
	  DeviceInfor.InControl.Memory.CpfLifeMotor1000Time = CPF_MOTOR1000_WORK_TIME;
	  DeviceInfor.InControl.Memory.DfLifeMotor1000Time  = DF_MOTOR1000_WORK_TIME;
	  DeviceInfor.InControl.Memory.CpfLife0Flag         = FALSE;
	  DeviceInfor.InControl.Memory.DfLife0Flag          = FALSE;
	  DeviceInfor.InControl.Memory.DfFilterResetPara    = 0;
		DeviceInfor.InControl.Memory.Time5MinCount        = 0;
		DeviceInfor.InControl.Memory.Time1DayCount        = 0;
		DeviceInfor.InControl.Memory.StartFlag            = FALSE;
		
		/*保温*/
		 DeviceInfor.InControl.Keep.Temp95           = 0;
		 DeviceInfor.InControl.Keep.Temp60           = 0;
		 DeviceInfor.InControl.Keep.Temp45           = 0;
		 DeviceInfor.InControl.Keep.SetTemp          = 0;
		 DeviceInfor.InControl.Keep.HighSeaLevelTemp = 0;
		 DeviceInfor.InControl.Keep.AutoTemp         = 0;
		 	
    /*沸点学习*/
		DeviceInfor.InControl.BoilPointStudy.Temp      = BOIL_POINT_STUDY_PARA;
		DeviceInfor.InControl.BoilPointStudy.TimeCount = 0;
		DeviceInfor.InControl.BoilPointStudy.StartFlag = FALSE;
		DeviceInfor.InControl.BoilPointStudy.Finish    = 0;
		DeviceInfor.InControl.BoilPointStudy.Condition = 0;
		DeviceInfor.InControl.BoilPointStudy.Clear     = 0;
		
		/*工厂设置*/
		DeviceInfor.InControl.RecoverFactory.Set = 0;
		
	  /*工程模式*/
		DeviceInfor.InControl.EngineerMode.Set = 0;
		
		
/*********************************全局内部信息***********************************/

/*********************************全局外部信息***********************************/
    /*热胆*/
		DeviceInfor.OutControl.ReDan.StopHeatFlag = TRUE;


/*********************************全局外部信息***********************************/	  
	
	
	  
	
}

/*新膜冲洗*/
void  New_Filter_Rinse(void)
{
	   static INT8U flag = 1;
	  
			/*首次上电或者DF滤芯复位，新膜冲洗未完成*/
		 if(  flag && ( DeviceInfor.InControl.NewMembraneRinse.Finish == 0 ) && ( ( DeviceInfor.Run.FirstPowerPara == POWER_PARA ) \
			 ||( DeviceInfor.InControl.Memory.DfFilterResetPara == DF_FILTER_RESET_PARA ) ) )
		 {
					/*先清除状态*/
					flag = 0;
					DeviceInfor.Run.EnForce = 1;  //强制执行
					DeviceInfor.InControl.NewMembraneRinse.Time0Flag = TRUE;
					DeviceInfor.InControl.NewMembraneRinse.Priority  = 4;    //优先级4
					DeviceInfor.Run.Status  = DeviceRinse;                   //切换到新膜冲洗状态
			 
					/*后动作，F1、M1、F2、F5打开*/
					F1_VALVE_ON;
//					M1_ON;
					F2_VALVE_ON;
					F5_VALVE_ON;
		 }
		 else
		 {
				 /*不是新膜冲洗切换到正常工作状态*/
				 DeviceInfor.Run.Status = DeviceWork;
		 }
			 
	
}

/*新膜冲洗1S为基本单位*/
void New_Filter_Rinse_Time(void)
{
	  
	   if(  DeviceInfor.InControl.NewMembraneRinse.Time0Flag == TRUE  )
		 {
			     
			    if( ++DeviceInfor.InControl.NewMembraneRinse.Time0 >= TIME_2MIN )
					{
						 DeviceInfor.InControl.NewMembraneRinse.Time0      = 0;
						 DeviceInfor.InControl.NewMembraneRinse.Time0Flag  = FALSE;
						 DeviceInfor.InControl.NewMembraneRinse.Time1Flag  = TRUE;
						
						 F5_VALVE_OFF;
					}
		 }
		 
		 if(  DeviceInfor.InControl.NewMembraneRinse.Time1Flag == TRUE )
		 {
			   
			    if(  ++DeviceInfor.InControl.NewMembraneRinse.Time1 >= TIME_18MIN )
					{
						   DeviceInfor.InControl.NewMembraneRinse.Time1      = 0;
						   DeviceInfor.InControl.NewMembraneRinse.Time1Flag  = FALSE;
						   DeviceInfor.InControl.NewMembraneRinse.Priority   = 0;    //优先级0
							 DeviceInfor.InControl.NewMembraneRinse.Finish     = NEW_MEMBRANE_RINSE_PARA;   //新膜冲洗完成
						   DeviceInfor.Run.Status                            = DeviceWork;                //切换到正常工作状态
						
						   OS_ENTER_CRITICAL();
						   Para_UpdateFLASH_Programs( NEW_MEMBRANE_RINSE_FLASH_ADD,  DeviceInfor.InControl.NewMembraneRinse.Finish);
						   OS_EXIT_CRITICAL();
						
						   F1_VALVE_OFF;
					     M1_OFF;
					     F2_VALVE_OFF;
					}
		 }
	  
	  
}


/*
3S扫描
扫描结束后，启动时间片任务；
龙头接收数据使能或者初始化
*/
void Api_3S_Task(void)
{
			/*
			 3S的时间自检：
			 读取液位信号，判定制水状态
			 读取滤芯寿命		
			 温度
		 */
			if( DeviceInfor.Run.Self3sStartFlag == TRUE )
			{
						/*读取液位信号*/ 
             
						/*CPF滤芯寿命*/
						if(  ( DeviceInfor.InControl.Memory.CpfLife1Year == 0 )|| ( DeviceInfor.InControl.Memory.CpfLifeMotor800Time == 0 ) 
								 || (  DeviceInfor.InControl.Memory.DfLifeMotor800Time == 0 ))
						{
							 /*指示灯变红*/
							 
						}
            else
						{
							 /*指示灯白色*/
						}
						
						/*DF滤芯寿命*/
						if(  ( DeviceInfor.InControl.Memory.DfLife5Year == 0 )|| ( DeviceInfor.InControl.Memory.CpfLifeMotor1000Time == 0 ) 
								 || (  DeviceInfor.InControl.Memory.DfLifeMotor1000Time == 0 ))
						{
							 /*指示灯变红*/
							 
						}
             else
						 {
							  /*指示灯白色*/
						 }							 
						
						/*温度*/
						Get_Temperature_Task(0);  //补充参数0
		}
			
		 /*3S自检完成*/
		 if( DeviceInfor.Run.Self3sFlag == TRUE )
		 {
			  DeviceInfor.Run.Self3sFlag     = FALSE;
			  DeviceInfor.Run.Self3sEndFlag  = TRUE;
			 
			  /*新膜冲洗*/
			  New_Filter_Rinse();
		 }
		 
	
}



/*常温水取水任务*/
static void Cold_Water_Get_Water_Task(void)
{
	 	   /*常温水取水停止*/
	   if(  DeviceInfor.InControl.GetWater.StopFlag == TRUE )
		 {
			  /*停止2分钟*/
			 if( 	++DeviceInfor.InControl.GetWater.StopTimeCount >= TIME_2MIN )
			 {
				  	DeviceInfor.InControl.GetWater.StopTimeCount    = 0;
				    DeviceInfor.InControl.GetWater.StopFlag         = FALSE;
				 
				    /*启动回流*/
				    DeviceInfor.InControl.HeadWaterReturn.StartFlag = TRUE;
				    DeviceInfor.InControl.HeadWaterReturn.Priority  = 2;
				 
           	/*动作,F1、M1、F4打开*/
            F1_VALVE_ON;
//				    M1_ON;
            F4_VALVE_ON;				 
				    
			 }
			 
	   }
}

/*头杯水回流任务*/
static void Head_Water_Return_Task(void)
{
			/*回流启动计时*/
		 if(  DeviceInfor.InControl.HeadWaterReturn.StartFlag == TRUE )
		 {
			   if(  ++DeviceInfor.InControl.HeadWaterReturn.TimeStartCount >= TIME_70S )
				 {
					  /*状态*/
					  DeviceInfor.InControl.HeadWaterReturn.TimeStartCount = 0;
					  DeviceInfor.InControl.HeadWaterReturn.StartFlag      = FALSE;	
					  DeviceInfor.InControl.HeadWaterReturn.Priority       = 0;
					  DeviceInfor.InControl.HeadWaterReturn.StopFlag       = TRUE;	
					 
					  /*F1、M1、F4关闭*/
					  F1_VALVE_OFF;
            M1_OFF;
            F4_VALVE_OFF;		
					  
				 }
		 }
		 
		 	/*回流结束计时*/
		  if(  DeviceInfor.InControl.HeadWaterReturn.StopFlag == TRUE )
			{
				  if(  ++DeviceInfor.InControl.HeadWaterReturn.TimeStopCount >= TIME_24HOUR )
					{
						 DeviceInfor.InControl.HeadWaterReturn.TimeStopCount = 0;
						 DeviceInfor.InControl.HeadWaterReturn.StopFlag      = FALSE;
						 /*启动回流*/
				     DeviceInfor.InControl.HeadWaterReturn.StartFlag     = TRUE;
				     DeviceInfor.InControl.HeadWaterReturn.Priority      = 2;
				 
            	/*动作,F1、M1、F4打开*/
							F1_VALVE_ON;
//							M1_ON;
							F4_VALVE_ON;	
						 
					}
			}
		 
		 
		 
}

/*任务优先级*/
static void Priority_Task(void)
{
		 /*优先级判断,常温水取水功能3=热水取水功能3＞回流功能2*/
		 if( DeviceInfor.InControl.GetWater.ColdWaterPriority >  DeviceInfor.InControl.HeadWaterReturn.Priority ) 
		 {
				 DeviceInfor.InControl.GetWater.ColdWaterPriority = 0; 
			 
				if(   DeviceInfor.InControl.HeadWaterReturn.Priority == 0  )
				{
						DeviceInfor.InControl.GetWater.StopTimeCount     = 0;
						DeviceInfor.InControl.GetWater.StopFlag          = FALSE;
				}
				else if( DeviceInfor.InControl.HeadWaterReturn.Priority == 2 )
				{
						DeviceInfor.InControl.HeadWaterReturn.TimeStartCount = 0;
						DeviceInfor.InControl.HeadWaterReturn.StartFlag      = FALSE;	
						DeviceInfor.InControl.HeadWaterReturn.Priority       = 0;
						/*F1、M1、F4关闭*/
						F4_VALVE_OFF;		
				}
		}
			 
}

/*沸点学习任务*/
static void Boil_Point_Study_Task(void)
{   
	  static INT8U temp[2] = {0};
		INT8U  difftemp = 0;
		
		temp[1] = temp[0];
		temp[0] = RealTemp; 
		
		difftemp = temp[0] - temp[1];
		
	
	      /*没有沸点学习+热胆高水位条件*/
				if( DeviceInfor.InControl.BoilPointStudy.Temp == 0 )
				{
					
						if(  DeviceInfor.InControl.BoilPointStudy.StartFlag == FALSE )
						{
							if( ( ++DeviceInfor.InControl.BoilPointStudy.TimeCount >= TIME_30S ) && ( RealTemp >= 80 ) )
							{
									DeviceInfor.InControl.BoilPointStudy.TimeCount = 0;
									DeviceInfor.InControl.BoilPointStudy.StartFlag = TRUE;
							}
							
						}
						else
						{
								/*沸点学习条件1*/
								if( difftemp == 0 ) 	
								{
										
										if( ++DeviceInfor.InControl.BoilPointStudy.TimeCount >= TIME_12S )
										{
											 DeviceInfor.InControl.BoilPointStudy.TimeCount = 0;
											 DeviceInfor.InControl.BoilPointStudy.StartFlag = FALSE;
											 DeviceInfor.InControl.BoilPointStudy.Finish    = 1;       //沸点学习完成
											
											 /*停止加热*/
											 
											 /*存储此时的温度*/
											 
										}
								}	
								else
								{
										  DeviceInfor.InControl.BoilPointStudy.TimeCount = 0;
								}	
								
								/*沸点学习条件2*/
								if( RealTemp >= BOIL_POINT_97C )
								{
										 DeviceInfor.InControl.BoilPointStudy.TimeCount = 0;
										 DeviceInfor.InControl.BoilPointStudy.StartFlag = FALSE;
										 DeviceInfor.InControl.BoilPointStudy.Finish    = 1;       //沸点学习完成
										
										 /*停止加热*/
										 
										 /*存储此时的温度*/
								}

														
						}
						
				}

	 
}

/*滤芯寿命*/
static void Memory_Task(void)
{
	   INT16U cpfmin = 0,dfmin = 0;
	   INT16U cpfday = 0,dfday = 0;
	
      if( ( DeviceInfor.Run.FirstPowerPara == 0xAA ) && ( DeviceInfor.InControl.MakeWater.OnFlag == TRUE ) )
		 {
				 /*250min后开始记忆*/
				if(  ++DeviceInfor.InControl.Memory.Time5MinCount >= TIME_250MIN )
				{
						DeviceInfor.InControl.Memory.Time5MinCount = 0;
						DeviceInfor.InControl.Memory.StartFlag     = TRUE;	
					  DeviceInfor.Run.FirstPowerPara             = 0;
					
						OS_ENTER_CRITICAL();
						Para_UpdateFLASH_Programs( FIRST_POWER_FLASH_ADD,  DeviceInfor.Run.FirstPowerPara );
						OS_EXIT_CRITICAL();				
					
				}
		 }
		 
		  if( ( DeviceInfor.InControl.Memory.StartFlag == TRUE ) && ( DeviceInfor.InControl.MakeWater.OnFlag == TRUE ))
		 { 
			  /*5分钟存储*/
				if(  ++DeviceInfor.InControl.Memory.Time5MinCount >= TIME_5MIN )
				{
						DeviceInfor.InControl.Memory.Time5MinCount = 0;
					
					  /*800加仑泵*/
					  if(  DeviceInfor.InControl.Memory.CpfLife0Flag == FALSE )
						{
								cpfmin = 5;					
								cpfmin = DeviceInfor.InControl.Memory.CpfLifeMotor800Time - cpfmin;
						  	DeviceInfor.InControl.Memory.CpfLifeMotor800Time = cpfmin;
								OS_ENTER_CRITICAL();
								Para_UpdateFLASH_Programs( CPF_LIFE_MOTOR800_FLASH_ADD,  cpfmin );
								OS_EXIT_CRITICAL();	
						}	
            /*仿照D2101*/						

						if(  DeviceInfor.InControl.Memory.DfLife0Flag == FALSE )
						{
								dfmin = 5;					
								dfmin = DeviceInfor.InControl.Memory.DfLifeMotor800Time - dfmin;
							  DeviceInfor.InControl.Memory.DfLifeMotor800Time = dfmin;
								OS_ENTER_CRITICAL();
								Para_UpdateFLASH_Programs( DF_LIFE_MOTOR800_FLASH_ADD, dfmin );
								OS_EXIT_CRITICAL();
						}
						 /*仿照D2101*/		
        
//					  /*1000加仑泵*/
//					  cpfmin = 5;					
//					  cpfmin = DeviceInfor.InControl.Memory.CpfLifeMotor1000Time - cpfmin;
//						OS_ENTER_CRITICAL();
//						Para_UpdateFLASH_Programs( CPF_LIFE_MOTOR1000_FLASH_ADD,  cpfmin );
//						OS_EXIT_CRITICAL();						
//					  dfmin = 5;					
//					  dfmin = DeviceInfor.InControl.Memory.CpfLifeMotor1000Time - dfmin;
//						OS_ENTER_CRITICAL();
//						Para_UpdateFLASH_Programs( DF_LIFE_MOTOR1000_FLASH_ADD, dfmin );
//						OS_EXIT_CRITICAL();
            					
					
					
				}
				
				/*1天存储*/
				if(  ++DeviceInfor.InControl.Memory.Time1DayCount >= TIME_1DAY )
				{
					  DeviceInfor.InControl.Memory.Time1DayCount = 0;
					 
					  /*时间都是一样*/
					  cpfday = 1;					
					  cpfday =   DeviceInfor.InControl.Memory.CpfLife1Year - cpfday;
						OS_ENTER_CRITICAL();
						Para_UpdateFLASH_Programs( CPF_LIFE_FLASH_ADD,  cpfday );
						OS_EXIT_CRITICAL();						
					  dfday = 1;					
					  dfday = DeviceInfor.InControl.Memory.DfLife5Year - dfday;
						OS_ENTER_CRITICAL();
						Para_UpdateFLASH_Programs( DF_LIFE_FLASH_ADD, dfday );
						OS_EXIT_CRITICAL();
									
				}
			  
		 }
			
			

}

/*1s时间基准*/
void Time_Base_1S_Task(void *pMsg)
{
	  pMsg = pMsg;
	  
  
    Cold_Water_Get_Water_Task();
		Head_Water_Return_Task();
	  Priority_Task();
	  Boil_Point_Study_Task();
	
	
}