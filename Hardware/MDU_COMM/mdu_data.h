#ifndef _MDU_DATA_H
#define _MDU_DATA_H


#include "stdint.h"




#define REC_BUF_NUM 11
#define SND_BUF_NUM 16




/*���ذ巢���豸״̬����ͷ*/
typedef union{
	
	INT8U Val;
	struct{
		
	  INT8U Bit7              : 1;
		INT8U NewMembranceRinse : 1;
		INT8U EngineerMode      : 1;
		INT8U FactorySet        : 1;
		INT8U BoilPointClear    : 1;
		INT8U HuanLvState       : 1;
		INT8U ReDanShortWater   : 1;
		INT8U HeatState         : 1;	
		
	}BIT;
	
}MDU_DATA_DATA3;

typedef union{
	
	  INT8U SndBuf[SND_BUF_NUM];
	  struct{
			INT8U      Head;         //֡ͷ
			INT8U      RandomNum;    //�����
			INT8U      ReplyType;    //Ӧ������
			MDU_DATA_DATA3 MduDataData3;  //����3
			INT8U ReDanTemState;     //�ȵ��¶�״̬
			INT8U ReDanTemp;          //�ȵ��¶�
			INT8U OutWaterType;      //��ˮ����
			INT8U OriginTdsHigh;     //ԭˮTDSֵ�ĸ�λ
			INT8U OriginTdsLow;      //ԭˮTDSֵ�ĵ�λ
			INT8U CleanTdsHigh;      //��ˮTDSֵ�ĸ�λ
			INT8U CleanTdsLow;       //��ˮTDSֵ�ĵ�λ
			INT8U CpfLifeHigh;       //CPF��о�����ĸ�λ
			INT8U CpfLifeLow;        //CPF��о�����ĵ�λ
			INT8U DfLifeHigh;        //DF��о�����ĸ�λ
			INT8U DfLifeLow;         //DF��о�����ĵ�λ
			INT8U CheckNum;          //У���
		}DATA;
	
	  	  
}MDU_DATA_TX;
extern  MDU_DATA_TX volatile MduDataTx;

/*��ͷ�·���ѯ��Ϣ������ָ��*/
typedef union{
	
	INT8U RecBuf[REC_BUF_NUM];
	
	struct{
		
	  INT8U      Head;         //֡ͷ
	  INT8U      RandomNum;    //�����
	  INT8U      CommandType;  //��������  
		INT8U      KeyValue;     //����ֵ
	  INT8U      KeepTempSet;   //�����趨
	  INT8U      BoilPointClear; //�е����
		INT8U      FactorySet;    //�ָ���������
		INT8U      EngineerMode;  //����ģʽ
		INT8U      EmptyData8;    //������8
		INT8U      EmptyData9;    //������9
		INT8U      CheckNum;      //У���  
		
	}DATA;
	
}MDU_DATA_RX;

extern MDU_DATA_RX volatile MduDataRx;

extern void Mdu_Data_Rx_Exe_Func(void);

extern void Mdu_Data_Auto_Update(INT8U idx,INT8U val,INT8U flag);

extern void Mdu_Data_Struct_Init(void);

extern void Mdu_Data_Update_Task(void *pMsg);

#endif
