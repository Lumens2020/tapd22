
#ifndef _RFID_DATA_H
#define _RFID_DATA_H


#include "stdint.h"



#define RFID_CARD_REC_NUM     24    //�п�24���޿�6
#define RFID_NO_CARD_REC_NUM  6

//#define MUL_COM1_PHY_SEND_BUF(buf,len)  Load_Usart1_Dma_Snd(buf, len)



/*������������*/
typedef struct{
	
			INT8U      Head;         //֡ͷ
			INT8U      Data2;        //����2
			INT8U      Data3;        //����3
			INT8U      Data4;        //����4
			INT8U      Data5;        //����5
		  INT8U      Data6;        //����6
			INT8U      Data7;        //����7
			INT8U      Data8;        //����8
			INT8U      Data9;        //����9
			INT8U      Data10;       //����10
		  INT8U      EndData;      //��������
	
	  	  
}RFID_DATA_TX;
extern  RFID_DATA_TX volatile RfidDataTx;


/*��������RFID����*/
typedef union{
	
	INT8U RecBuf[RFID_CARD_REC_NUM];    //��������������
	
	struct{
		
			INT8U      Head;         //֡ͷ
			INT8U      Data2;        //����2
			INT8U      Data3;        //����3
			INT8U      Data4;        //����4
			INT8U      Data5;        //����5
		  INT8U      Data6;        //����6
			INT8U      Data7;        //����7
			INT8U      Data8;        //����8
			INT8U      Data9;        //����9
			INT8U      Data10;       //����10
			INT8U      Data11;       //����11
			INT8U      CheckData1;   //У������1
			INT8U      CheckData2;   //У������2
			INT8U      CheckData3;   //У������3
		  INT8U      CheckData4;   //У������4
			INT8U      CheckData5;   //У������5
			INT8U      CheckData6;   //У������6
			INT8U      CheckData7;   //У������7
			INT8U      CheckData8;   //У������8
		  INT8U      CardData1;    //������1
			INT8U      CardData2;    //������2
			INT8U      CardData3;    //������3
			INT8U      CardData4;    //������4
		  INT8U      CheckNum;     //У���  
		
	}DATA;
	
}RFID_DATA_RX;

extern RFID_DATA_RX  volatile RfidDataRx;


extern void Rfid_Data_Rx_Ok(INT8U *buf, INT8U len);

extern void Rfid_Data_Update_Infor(void);

extern void Rfid_Data_Struct_Init(void);


#endif
