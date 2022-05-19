
#include "define.h"


static INT16U NtcVoltage = 0;                              //NTC��Ӧ�ĵ�ѹֵ
static INT16U NtcRes     = 0;                              //NTC����ֵ

static INT16U  ValueMax = 0, ValueMin = 5000;
static INT16U  AdcValue = 0, AdcValueSum = 0;              //ADCת��ֵ��ADCת��ֵ��

volatile INT8U  RealTemp  = 0;				                               //�¶�ֵ


/*100K	 3950K ����������0-109��Ӧ�ĵ���ֵ,���Ŵ�10��*/
INT16U const NtcToRes[NTC_NUM] =			
{
		3270 	,	3107 	,	2954 	,	2809 	,	2672	 	,	2542 	,	2419 	,	2303 	,	2194 	,	2090 ,     //0-9
		1992 	,	1898 	,	1810 	,	1726 	,	1647 	  ,	1572 	,	1500 	,	1433 	,	1368 	,	1307 ,     //10-19
		1249 	,	1194 	,	1142 	,	1092 	,	1045 	  ,	1000 	,	957 	,	916 	,	877 	,	840 ,      //20-29
		805 	,	771 	,	739 	,	709 	,	680 	  ,	652 	,	625 	,	600 	,	576 	,	553 ,      //30-39
		531 	,	510 	,	490 	,	471 	,	453 	  ,	435 	,	418 	,	402 	,	387 	,	372 ,      //40-49
	
		358 	,	345 	,	332 	,	320 	,	308 	  ,	297 	,	286 	,	275 	,	265 	,	256 ,      //50-59
		247 	,	238   , 229 	,	221 	,	214 	  ,	206   ,	199 	,	192 	,	185  	,	179 ,      //60-69
		173 	,	167  	,	161 	,	156 	,	151 	  ,	146 	,	141 	,	136 	,	132 	,	127 ,      //70-79
		123 	,	119 	,	115 	,	112 	,	108 	  ,	105 	,	101 	,	98	 	,	95 	  ,	92  ,      //80-89
		89 	  ,	87 	  ,	84	 	,	81	 	,	79 	    ,	76   	,	74 	  ,	72 	  ,	70   	,	68  , 		 //90-99
    66 	  ,	64 	  ,	62	  ,	60	 	,	58 	    ,	57   	,	55	 	,	53 	  ,	52   	,	50  , 		 //100-109
};


void adc_config(void)
{
    /* ADC contineous function enable */
    adc_special_function_config(ADC_CONTINUOUS_MODE, ENABLE);
    /* ADC trigger config */
    adc_external_trigger_source_config(ADC_REGULAR_CHANNEL, ADC_EXTTRIG_REGULAR_NONE); 
    /* ADC data alignment config */
    adc_data_alignment_config(ADC_DATAALIGN_RIGHT);
    /* ADC channel length config */
    adc_channel_length_config(ADC_REGULAR_CHANNEL, 1);
 
    /* ADC regular channel config */
    adc_regular_channel_config(0, BOARD_ADC_CHANNEL, ADC_SAMPLETIME_55POINT5);
    adc_external_trigger_config(ADC_REGULAR_CHANNEL, ENABLE);

    /* ADC resolusion 6B */

//    adc_resolution_config(ADC_RESOLUTION_6B);
    
    /* enable ADC interface */
    adc_enable();
    delay_1ms(1);
    /* ADC calibration and reset calibration */
    adc_calibration_enable();
}


void Adc_Init(void)
{
	
	 	//ADC�����������
	  rcu_periph_clock_enable(RCU_GPIOA);//PAʱ��
	  rcu_periph_clock_enable(RCU_ADC);//ADCʱ��
	  rcu_adc_clock_config(RCU_ADCCK_APB2_DIV6);//ADCʱ��
	  gpio_mode_set(ADC_GPIO_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, ADC_GPIO_PIN);//PA1����
	
		gpio_mode_set(ADC_GPIO_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, ADC_GPIO_PIN);//PA1����
	  adc_config();//ADC����
 
	 
}




/*
���ʵ��NTC����ֵ
*/
INT16U Get_Ntc_Res(void)
{
	
    INT8U  i = 0;
	
	  AdcValueSum = 0;
        
		for(i=0;i<10;i++)                                     //��ʮ�Σ�ȥ��������ֵ
		{
			adc_software_trigger_enable(ADC_REGULAR_CHANNEL);  //����ʹ�����ת������
			adc_flag_clear(ADC_FLAG_EOC);
			while(SET != adc_flag_get(ADC_FLAG_EOC)){ }
			AdcValue = ADC_RDATA;                              //�õ�ADCת��ֵ
					 
			if(AdcValue >= ValueMax)  ValueMax = AdcValue;
			if(AdcValue <= ValueMin)  ValueMin = AdcValue; 
			AdcValueSum = AdcValueSum + AdcValue;
			
		}
		
		AdcValue = ( AdcValueSum - ValueMax - ValueMin )/8;	   //ת��ֵƽ��ֵ
		
		ValueMin = 5000;   ValueMax = 0;	
		
		
    NtcVoltage = 3297*AdcValue/4095;                       //�õ�ADCת���ĵ�ѹֵ
		
		NtcRes =  47*NtcVoltage/(3297-NtcVoltage);             //���ʵ��NTC����ֵ
		
		
		return NtcRes;
		
}


void Get_Temperature_Task(void *pMsg)
{
	      pMsg = pMsg;
	  	
				INT8U  t1 = 0,t2 = 0, j = 0; 
	      INT16U ntcres = Get_Ntc_Res();
	     
	
	      ntcres = ntcres*10;                                //NTC�Ŵ���10��
	
	      if( ntcres > 3270 || ntcres < 50)
				return;
									
				for(j=0;j<NTC_NUM;j++)
				{
						if( (ntcres >= NtcToRes[j+1]) && ( ntcres < NtcToRes[j] ) )
						{		
							
							t1 = 	ntcres - NtcToRes[j+1];
							t2 = 	NtcToRes[j] - ntcres;		
						
							/*��ֵԽС��˵���¶�Խ����*/			
							if( t1 < t2 )
							RealTemp = j;		          //��ת��ֵ��Ӧ�����±�
							else 
							RealTemp = j+1;		         //��ת��ֵ��Ӧ�����±�	
							
							break;
						}		
					 
					
				}
//				return  realtemp;
				
			/*����ȵ��¶�*/
			Mdu_Data_Auto_Update(ProData5,RealTemp,TRUE);
}


