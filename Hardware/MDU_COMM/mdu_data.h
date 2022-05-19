#ifndef _MDU_DATA_H
#define _MDU_DATA_H


#include "stdint.h"




#define REC_BUF_NUM 11
#define SND_BUF_NUM 16




/*主控板发送设备状态到龙头*/
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
			INT8U      Head;         //帧头
			INT8U      RandomNum;    //随机数
			INT8U      ReplyType;    //应答类型
			MDU_DATA_DATA3 MduDataData3;  //数据3
			INT8U ReDanTemState;     //热胆温度状态
			INT8U ReDanTemp;          //热胆温度
			INT8U OutWaterType;      //出水类型
			INT8U OriginTdsHigh;     //原水TDS值的高位
			INT8U OriginTdsLow;      //原水TDS值的低位
			INT8U CleanTdsHigh;      //净水TDS值的高位
			INT8U CleanTdsLow;       //净水TDS值的低位
			INT8U CpfLifeHigh;       //CPF滤芯寿命的高位
			INT8U CpfLifeLow;        //CPF滤芯寿命的低位
			INT8U DfLifeHigh;        //DF滤芯寿命的高位
			INT8U DfLifeLow;         //DF滤芯寿命的低位
			INT8U CheckNum;          //校验和
		}DATA;
	
	  	  
}MDU_DATA_TX;
extern  MDU_DATA_TX volatile MduDataTx;

/*龙头下发查询信息及操作指令*/
typedef union{
	
	INT8U RecBuf[REC_BUF_NUM];
	
	struct{
		
	  INT8U      Head;         //帧头
	  INT8U      RandomNum;    //随机数
	  INT8U      CommandType;  //命令类型  
		INT8U      KeyValue;     //按键值
	  INT8U      KeepTempSet;   //保温设定
	  INT8U      BoilPointClear; //沸点清除
		INT8U      FactorySet;    //恢复出厂设置
		INT8U      EngineerMode;  //工程模式
		INT8U      EmptyData8;    //空数据8
		INT8U      EmptyData9;    //空数据9
		INT8U      CheckNum;      //校验和  
		
	}DATA;
	
}MDU_DATA_RX;

extern MDU_DATA_RX volatile MduDataRx;

extern void Mdu_Data_Rx_Exe_Func(void);

extern void Mdu_Data_Auto_Update(INT8U idx,INT8U val,INT8U flag);

extern void Mdu_Data_Struct_Init(void);

extern void Mdu_Data_Update_Task(void *pMsg);

#endif
