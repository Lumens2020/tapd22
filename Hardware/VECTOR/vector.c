#include "define.h"

/*
vector:���Ա�
*/
/*ÿ�����п��ԼӺܶ�����*/
/*10MS*/
const TASK_S Task_Group0[] =
{
	 
	  
//   {Producer,&P1},  //����1
	 {NULL,NULL},
	
};
/*20MS*/
const TASK_S Task_Group1[] =
{
  
	 {Key_Main_Task,&KeyS},
	 {Combination_Key_Task,&KeyS},
//	{Producer,&P2},  //����2
	 {NULL,NULL},
};

/*100MS*/
const TASK_S Task_Group2[] =
{
  	
//	{Consumer,&C1},  //������1
//	{ReShui_Motor_Consumer,&ReShuiMotorPcmodelC1},
	{NULL,NULL},
};
/*200MS*/
const TASK_S Task_Group3[] =
{
	{Usart1_Rec_Task,NULL},
//{Consumer,&C2},  //������2
	{NULL,NULL},
};
/*500MS*/
const TASK_S Task_Group4[] =
{

  {Get_Temperature_Task,NULL},
	{NULL,NULL},
};

/*1000MS*/
const TASK_S Task_Group5[] =
{
//	{Mdu_Data_Update_Task,NULL},
	{Realy_Heat_Task,NULL},
//  {Led_Toggle,NULL},
	{Time_Base_1S_Task,NULL},
	{NULL,NULL},
	
};

/*2000MS*/
const TASK_S Task_Group6[] =
{

	{Usart1_Send_Task,NULL},
	{NULL,NULL},
};

/*�������8��Task_Group*/
const TASK_GROUP_S Task_List[] =
{
	
	
			{10,Task_Group0},
			{20,Task_Group1},
			{100,Task_Group2},
			{200,Task_Group3},
			{500,Task_Group4},
			{1000,Task_Group5},
			{2000,Task_Group6},
	
		 
};

INT8U Task_Group_Num(void)
{
	return sizeof(Task_List)/sizeof(TASK_GROUP_S);
}

void Init_Task_List(void)
{
	 MultiTask.pTaskList = Task_List;
}