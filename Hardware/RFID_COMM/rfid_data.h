
#ifndef _RFID_DATA_H
#define _RFID_DATA_H


#include "stdint.h"



#define RFID_CARD_REC_NUM     24    //有卡24，无卡6
#define RFID_NO_CARD_REC_NUM  6

//#define MUL_COM1_PHY_SEND_BUF(buf,len)  Load_Usart1_Dma_Snd(buf, len)



/*主机发送数据*/
typedef struct{
	
			INT8U      Head;         //帧头
			INT8U      Data2;        //数据2
			INT8U      Data3;        //数据3
			INT8U      Data4;        //数据4
			INT8U      Data5;        //数据5
		  INT8U      Data6;        //数据6
			INT8U      Data7;        //数据7
			INT8U      Data8;        //数据8
			INT8U      Data9;        //数据9
			INT8U      Data10;       //数据10
		  INT8U      EndData;      //结束数据
	
	  	  
}RFID_DATA_TX;
extern  RFID_DATA_TX volatile RfidDataTx;


/*主机接收RFID数据*/
typedef union{
	
	INT8U RecBuf[RFID_CARD_REC_NUM];    //按照最大接收数据
	
	struct{
		
			INT8U      Head;         //帧头
			INT8U      Data2;        //数据2
			INT8U      Data3;        //数据3
			INT8U      Data4;        //数据4
			INT8U      Data5;        //数据5
		  INT8U      Data6;        //数据6
			INT8U      Data7;        //数据7
			INT8U      Data8;        //数据8
			INT8U      Data9;        //数据9
			INT8U      Data10;       //数据10
			INT8U      Data11;       //数据11
			INT8U      CheckData1;   //校验数据1
			INT8U      CheckData2;   //校验数据2
			INT8U      CheckData3;   //校验数据3
		  INT8U      CheckData4;   //校验数据4
			INT8U      CheckData5;   //校验数据5
			INT8U      CheckData6;   //校验数据6
			INT8U      CheckData7;   //校验数据7
			INT8U      CheckData8;   //校验数据8
		  INT8U      CardData1;    //卡数据1
			INT8U      CardData2;    //卡数据2
			INT8U      CardData3;    //卡数据3
			INT8U      CardData4;    //卡数据4
		  INT8U      CheckNum;     //校验和  
		
	}DATA;
	
}RFID_DATA_RX;

extern RFID_DATA_RX  volatile RfidDataRx;


extern void Rfid_Data_Rx_Ok(INT8U *buf, INT8U len);

extern void Rfid_Data_Update_Infor(void);

extern void Rfid_Data_Struct_Init(void);


#endif
