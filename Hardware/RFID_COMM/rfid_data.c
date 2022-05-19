
#include "define.h"

RFID_DATA_TX volatile  RfidDataTx;
RFID_DATA_RX volatile  RfidDataRx;


 /*���ݷ�����ʱ������*/
const INT8U RfidTxProBuf[USART1_SND_BUF_SIZE] = {0x00,0x00,0xFF,0x04,0xFC,0xD4,0x4A,0x01,0x00,0xE1,0x00};

static INT8U CardMess[4] = {0};

void Rfid_Data_Struct_Init(void)
{
	 memset((void *)&RfidDataTx,0,sizeof(RfidDataTx));
	 memset((void *)&RfidDataRx,0,sizeof(RfidDataRx));
}

void Rfid_Send_Packet(INT8U *str,INT8U len)
{

	 /*���ô˺���������ײ㺯��*/	
	 Usart1_Tx_Data(str,len);
}




/*����1�������ݸ�RFID*/
void Rfid_Data_Update_Infor(void)
{
	 INT8U i = 0;
	 INT16U sum = 0;
	 /*ʹ��ָ������ָ��һ����ʼ��*/
	 RFID_DATA_TX *p_ctrl;
  
	/*FALSE:æ��TRUE:��æ*/
	if( Usart1TaskData.Snd.SndOk == FALSE )
	{
		return;
	}
	
 	p_ctrl = (RFID_DATA_TX *)&RfidDataTx;   //ָ���ʼ��
	
	 /*��Ӹ��µ�����*/
	 p_ctrl->Head              = RfidTxProBuf[ProData0];
	 p_ctrl->Data2             = RfidTxProBuf[ProData1];
   p_ctrl->Data3             = RfidTxProBuf[ProData2];  
   p_ctrl->Data4             = RfidTxProBuf[ProData3];  
	 p_ctrl->Data5             = RfidTxProBuf[ProData4]; 
	 p_ctrl->Data6             = RfidTxProBuf[ProData5];  
   p_ctrl->Data7             = RfidTxProBuf[ProData6];  
	 p_ctrl->Data8             = RfidTxProBuf[ProData7];  
	 p_ctrl->Data9             = RfidTxProBuf[ProData8];  
   p_ctrl->Data10            = RfidTxProBuf[ProData9]; 
	 p_ctrl->EndData           = RfidTxProBuf[ProData10]; 

	
	/*��������*/
   Rfid_Send_Packet((INT8U *)p_ctrl,ProData10+1);
	
}


/*�������������*/
void Rfid_Data_Rx_Ok(INT8U *buf, INT8U len)
{
		 INT8U i = 0;
	   INT16U sum = 0;
	
		 OS_ENTER_CRITICAL();			 
		 for(i=0; i<len; i++ )
		{
			 RfidDataRx.RecBuf[i] = buf[i];
		}		
		OS_EXIT_CRITICAL();	
		
		/*�޿�*/
		if(len <= RFID_NO_CARD_REC_NUM )
		{
			return;
		}
		/*�п�*/
		else if( len >= RFID_CARD_REC_NUM)
		{
				 len = RFID_CARD_REC_NUM;
				 for(i=11; i<len-1; i++)
				{
						 sum +=RfidDataRx.RecBuf[i]; 
				}
				
				sum = 0x100 - (INT8U)sum;
				sum %= 0x100;
		}

		/*�п�У����ж�*/
		if( sum == RfidDataRx.DATA.CheckNum )
		{
	     /*����������Ϣ*/
			
			 /*��ȡ������Ϣ*/ 
       CardMess[0] = RfidDataRx.DATA.CardData1;
			 CardMess[1] = RfidDataRx.DATA.CardData2;
			 CardMess[2] = RfidDataRx.DATA.CardData3;
			 CardMess[3] = RfidDataRx.DATA.CardData4;
			
			 memset((void *)RfidDataRx.RecBuf,0,RFID_CARD_REC_NUM);      //��֤ÿ��������0
			
//			 gpio_bit_toggle(GPIOC,GPIO_PIN_13);                         //��˸��֤ͨ��
		}
		else
		{

			 return;
		}
		

}



