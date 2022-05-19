#include "define.h"


 DEVICE_INFOR volatile DeviceInfor;


static INT8U CriticalCnt = 0;
/*����ؼ������*/
void  Os_Enter_Critical(void)
{
	CriticalCnt++;
	__disable_irq();
}	

/*�˳��ؼ������*/
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
�豸��Ϣ��ʼ��
��־λ״̬ʹ��TURE,FALSE
��������ֵ����
*/
void Device_Infor_Init(void)
{
	
/*********************************ȫ���ڲ���Ϣ***********************************/
	  /*�豸����״̬*/
	  DeviceInfor.Run.FirstPowerPara      =  POWER_PARA;
	  DeviceInfor.Run.Self3sStartFlag     =  TRUE;
	  DeviceInfor.Run.Self3sFlag          =  FALSE;
	  DeviceInfor.Run.Self3sEndFlag       =  FALSE;
	  DeviceInfor.Run.EnForce             =  0;     
	  DeviceInfor.Run.Status              =  DeviceStandy;
	
	  /*��Ĥ��ϴ*/
	  DeviceInfor.InControl.NewMembraneRinse.Time0      = 0;
		DeviceInfor.InControl.NewMembraneRinse.Time0Flag  = FALSE;
	  DeviceInfor.InControl.NewMembraneRinse.Time1      = 0;
	  DeviceInfor.InControl.NewMembraneRinse.Time1Flag  = FALSE;
	  DeviceInfor.InControl.NewMembraneRinse.Enable     = 0;
	  DeviceInfor.InControl.NewMembraneRinse.Priority   = 0;
	  DeviceInfor.InControl.NewMembraneRinse.Finish     = 0;
	
	  /*ȡˮ*/
	  DeviceInfor.InControl.GetWater.StopTimeCount     = 0;
		DeviceInfor.InControl.GetWater.StopFlag          = FALSE;
		DeviceInfor.InControl.GetWater.ColdWaterPriority = 0;
		DeviceInfor.InControl.GetWater.HotWaterPriority  = 0;
		
		/*ͷ��ˮ����*/
		DeviceInfor.InControl.HeadWaterReturn.StartFlag      = FALSE;
		DeviceInfor.InControl.HeadWaterReturn.StopFlag       = FALSE;
		DeviceInfor.InControl.HeadWaterReturn.TimeStartCount = 0;
		DeviceInfor.InControl.HeadWaterReturn.TimeStopCount  = 0;
		DeviceInfor.InControl.HeadWaterReturn.Priority       = 0;
		
		/*��ˮ*/
	  DeviceInfor.InControl.MakeWater.OnFlag  = FALSE;
		DeviceInfor.InControl.MakeWater.OffFlag = FALSE;
		DeviceInfor.InControl.MakeWater.Protect = 0;
		
		
	  /*��о����*/
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
		
		/*����*/
		 DeviceInfor.InControl.Keep.Temp95           = 0;
		 DeviceInfor.InControl.Keep.Temp60           = 0;
		 DeviceInfor.InControl.Keep.Temp45           = 0;
		 DeviceInfor.InControl.Keep.SetTemp          = 0;
		 DeviceInfor.InControl.Keep.HighSeaLevelTemp = 0;
		 DeviceInfor.InControl.Keep.AutoTemp         = 0;
		 	
    /*�е�ѧϰ*/
		DeviceInfor.InControl.BoilPointStudy.Temp      = BOIL_POINT_STUDY_PARA;
		DeviceInfor.InControl.BoilPointStudy.TimeCount = 0;
		DeviceInfor.InControl.BoilPointStudy.StartFlag = FALSE;
		DeviceInfor.InControl.BoilPointStudy.Finish    = 0;
		DeviceInfor.InControl.BoilPointStudy.Condition = 0;
		DeviceInfor.InControl.BoilPointStudy.Clear     = 0;
		
		/*��������*/
		DeviceInfor.InControl.RecoverFactory.Set = 0;
		
	  /*����ģʽ*/
		DeviceInfor.InControl.EngineerMode.Set = 0;
		
		
/*********************************ȫ���ڲ���Ϣ***********************************/

/*********************************ȫ���ⲿ��Ϣ***********************************/
    /*�ȵ�*/
		DeviceInfor.OutControl.ReDan.StopHeatFlag = TRUE;


/*********************************ȫ���ⲿ��Ϣ***********************************/	  
	
	
	  
	
}

/*��Ĥ��ϴ*/
void  New_Filter_Rinse(void)
{
	   static INT8U flag = 1;
	  
			/*�״��ϵ����DF��о��λ����Ĥ��ϴδ���*/
		 if(  flag && ( DeviceInfor.InControl.NewMembraneRinse.Finish == 0 ) && ( ( DeviceInfor.Run.FirstPowerPara == POWER_PARA ) \
			 ||( DeviceInfor.InControl.Memory.DfFilterResetPara == DF_FILTER_RESET_PARA ) ) )
		 {
					/*�����״̬*/
					flag = 0;
					DeviceInfor.Run.EnForce = 1;  //ǿ��ִ��
					DeviceInfor.InControl.NewMembraneRinse.Time0Flag = TRUE;
					DeviceInfor.InControl.NewMembraneRinse.Priority  = 4;    //���ȼ�4
					DeviceInfor.Run.Status  = DeviceRinse;                   //�л�����Ĥ��ϴ״̬
			 
					/*������F1��M1��F2��F5��*/
					F1_VALVE_ON;
//					M1_ON;
					F2_VALVE_ON;
					F5_VALVE_ON;
		 }
		 else
		 {
				 /*������Ĥ��ϴ�л�����������״̬*/
				 DeviceInfor.Run.Status = DeviceWork;
		 }
			 
	
}

/*��Ĥ��ϴ1SΪ������λ*/
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
						   DeviceInfor.InControl.NewMembraneRinse.Priority   = 0;    //���ȼ�0
							 DeviceInfor.InControl.NewMembraneRinse.Finish     = NEW_MEMBRANE_RINSE_PARA;   //��Ĥ��ϴ���
						   DeviceInfor.Run.Status                            = DeviceWork;                //�л�����������״̬
						
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
3Sɨ��
ɨ�����������ʱ��Ƭ����
��ͷ��������ʹ�ܻ��߳�ʼ��
*/
void Api_3S_Task(void)
{
			/*
			 3S��ʱ���Լ죺
			 ��ȡҺλ�źţ��ж���ˮ״̬
			 ��ȡ��о����		
			 �¶�
		 */
			if( DeviceInfor.Run.Self3sStartFlag == TRUE )
			{
						/*��ȡҺλ�ź�*/ 
             
						/*CPF��о����*/
						if(  ( DeviceInfor.InControl.Memory.CpfLife1Year == 0 )|| ( DeviceInfor.InControl.Memory.CpfLifeMotor800Time == 0 ) 
								 || (  DeviceInfor.InControl.Memory.DfLifeMotor800Time == 0 ))
						{
							 /*ָʾ�Ʊ��*/
							 
						}
            else
						{
							 /*ָʾ�ư�ɫ*/
						}
						
						/*DF��о����*/
						if(  ( DeviceInfor.InControl.Memory.DfLife5Year == 0 )|| ( DeviceInfor.InControl.Memory.CpfLifeMotor1000Time == 0 ) 
								 || (  DeviceInfor.InControl.Memory.DfLifeMotor1000Time == 0 ))
						{
							 /*ָʾ�Ʊ��*/
							 
						}
             else
						 {
							  /*ָʾ�ư�ɫ*/
						 }							 
						
						/*�¶�*/
						Get_Temperature_Task(0);  //�������0
		}
			
		 /*3S�Լ����*/
		 if( DeviceInfor.Run.Self3sFlag == TRUE )
		 {
			  DeviceInfor.Run.Self3sFlag     = FALSE;
			  DeviceInfor.Run.Self3sEndFlag  = TRUE;
			 
			  /*��Ĥ��ϴ*/
			  New_Filter_Rinse();
		 }
		 
	
}



/*����ˮȡˮ����*/
static void Cold_Water_Get_Water_Task(void)
{
	 	   /*����ˮȡˮֹͣ*/
	   if(  DeviceInfor.InControl.GetWater.StopFlag == TRUE )
		 {
			  /*ֹͣ2����*/
			 if( 	++DeviceInfor.InControl.GetWater.StopTimeCount >= TIME_2MIN )
			 {
				  	DeviceInfor.InControl.GetWater.StopTimeCount    = 0;
				    DeviceInfor.InControl.GetWater.StopFlag         = FALSE;
				 
				    /*��������*/
				    DeviceInfor.InControl.HeadWaterReturn.StartFlag = TRUE;
				    DeviceInfor.InControl.HeadWaterReturn.Priority  = 2;
				 
           	/*����,F1��M1��F4��*/
            F1_VALVE_ON;
//				    M1_ON;
            F4_VALVE_ON;				 
				    
			 }
			 
	   }
}

/*ͷ��ˮ��������*/
static void Head_Water_Return_Task(void)
{
			/*����������ʱ*/
		 if(  DeviceInfor.InControl.HeadWaterReturn.StartFlag == TRUE )
		 {
			   if(  ++DeviceInfor.InControl.HeadWaterReturn.TimeStartCount >= TIME_70S )
				 {
					  /*״̬*/
					  DeviceInfor.InControl.HeadWaterReturn.TimeStartCount = 0;
					  DeviceInfor.InControl.HeadWaterReturn.StartFlag      = FALSE;	
					  DeviceInfor.InControl.HeadWaterReturn.Priority       = 0;
					  DeviceInfor.InControl.HeadWaterReturn.StopFlag       = TRUE;	
					 
					  /*F1��M1��F4�ر�*/
					  F1_VALVE_OFF;
            M1_OFF;
            F4_VALVE_OFF;		
					  
				 }
		 }
		 
		 	/*����������ʱ*/
		  if(  DeviceInfor.InControl.HeadWaterReturn.StopFlag == TRUE )
			{
				  if(  ++DeviceInfor.InControl.HeadWaterReturn.TimeStopCount >= TIME_24HOUR )
					{
						 DeviceInfor.InControl.HeadWaterReturn.TimeStopCount = 0;
						 DeviceInfor.InControl.HeadWaterReturn.StopFlag      = FALSE;
						 /*��������*/
				     DeviceInfor.InControl.HeadWaterReturn.StartFlag     = TRUE;
				     DeviceInfor.InControl.HeadWaterReturn.Priority      = 2;
				 
            	/*����,F1��M1��F4��*/
							F1_VALVE_ON;
//							M1_ON;
							F4_VALVE_ON;	
						 
					}
			}
		 
		 
		 
}

/*�������ȼ�*/
static void Priority_Task(void)
{
		 /*���ȼ��ж�,����ˮȡˮ����3=��ˮȡˮ����3����������2*/
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
						/*F1��M1��F4�ر�*/
						F4_VALVE_OFF;		
				}
		}
			 
}

/*�е�ѧϰ����*/
static void Boil_Point_Study_Task(void)
{   
	  static INT8U temp[2] = {0};
		INT8U  difftemp = 0;
		
		temp[1] = temp[0];
		temp[0] = RealTemp; 
		
		difftemp = temp[0] - temp[1];
		
	
	      /*û�зе�ѧϰ+�ȵ���ˮλ����*/
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
								/*�е�ѧϰ����1*/
								if( difftemp == 0 ) 	
								{
										
										if( ++DeviceInfor.InControl.BoilPointStudy.TimeCount >= TIME_12S )
										{
											 DeviceInfor.InControl.BoilPointStudy.TimeCount = 0;
											 DeviceInfor.InControl.BoilPointStudy.StartFlag = FALSE;
											 DeviceInfor.InControl.BoilPointStudy.Finish    = 1;       //�е�ѧϰ���
											
											 /*ֹͣ����*/
											 
											 /*�洢��ʱ���¶�*/
											 
										}
								}	
								else
								{
										  DeviceInfor.InControl.BoilPointStudy.TimeCount = 0;
								}	
								
								/*�е�ѧϰ����2*/
								if( RealTemp >= BOIL_POINT_97C )
								{
										 DeviceInfor.InControl.BoilPointStudy.TimeCount = 0;
										 DeviceInfor.InControl.BoilPointStudy.StartFlag = FALSE;
										 DeviceInfor.InControl.BoilPointStudy.Finish    = 1;       //�е�ѧϰ���
										
										 /*ֹͣ����*/
										 
										 /*�洢��ʱ���¶�*/
								}

														
						}
						
				}

	 
}

/*��о����*/
static void Memory_Task(void)
{
	   INT16U cpfmin = 0,dfmin = 0;
	   INT16U cpfday = 0,dfday = 0;
	
      if( ( DeviceInfor.Run.FirstPowerPara == 0xAA ) && ( DeviceInfor.InControl.MakeWater.OnFlag == TRUE ) )
		 {
				 /*250min��ʼ����*/
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
			  /*5���Ӵ洢*/
				if(  ++DeviceInfor.InControl.Memory.Time5MinCount >= TIME_5MIN )
				{
						DeviceInfor.InControl.Memory.Time5MinCount = 0;
					
					  /*800���ر�*/
					  if(  DeviceInfor.InControl.Memory.CpfLife0Flag == FALSE )
						{
								cpfmin = 5;					
								cpfmin = DeviceInfor.InControl.Memory.CpfLifeMotor800Time - cpfmin;
						  	DeviceInfor.InControl.Memory.CpfLifeMotor800Time = cpfmin;
								OS_ENTER_CRITICAL();
								Para_UpdateFLASH_Programs( CPF_LIFE_MOTOR800_FLASH_ADD,  cpfmin );
								OS_EXIT_CRITICAL();	
						}	
            /*����D2101*/						

						if(  DeviceInfor.InControl.Memory.DfLife0Flag == FALSE )
						{
								dfmin = 5;					
								dfmin = DeviceInfor.InControl.Memory.DfLifeMotor800Time - dfmin;
							  DeviceInfor.InControl.Memory.DfLifeMotor800Time = dfmin;
								OS_ENTER_CRITICAL();
								Para_UpdateFLASH_Programs( DF_LIFE_MOTOR800_FLASH_ADD, dfmin );
								OS_EXIT_CRITICAL();
						}
						 /*����D2101*/		
        
//					  /*1000���ر�*/
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
				
				/*1��洢*/
				if(  ++DeviceInfor.InControl.Memory.Time1DayCount >= TIME_1DAY )
				{
					  DeviceInfor.InControl.Memory.Time1DayCount = 0;
					 
					  /*ʱ�䶼��һ��*/
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

/*1sʱ���׼*/
void Time_Base_1S_Task(void *pMsg)
{
	  pMsg = pMsg;
	  
  
    Cold_Water_Get_Water_Task();
		Head_Water_Return_Task();
	  Priority_Task();
	  Boil_Point_Study_Task();
	
	
}