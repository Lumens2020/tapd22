/*!
    \file    gd32e23x_it.c
    \brief   interrupt service routines
    
    \version 2019-02-19, V1.0.0, firmware for GD32E23x
*/

/*
    Copyright (c) 2019, GigaDevice Semiconductor Inc.

    All rights reserved.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#include "gd32e23x_it.h"
#include "main.h"
#include "systick.h"
#include "multitask.h"
#include "define.h"


INT32U  TimeStamp = 0x00;

/*!
    \brief      this function handles NMI exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void NMI_Handler(void)
{
}

/*!
    \brief      this function handles HardFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HardFault_Handler(void)
{
    /* if Hard Fault exception occurs, go to infinite loop */
    while (1){
    }
}

/*!
    \brief      this function handles SVC exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SVC_Handler(void)
{
}

/*!
    \brief      this function handles PendSV exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void PendSV_Handler(void)
{
}

/*!
    \brief      this function handles SysTick exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
/*1MS中断一次*/
void SysTick_Handler(void)
{

			 static INT16U count0 = 0;
			 static INT16U count1 = 0;
 
		   /*3S状态计时*/
				if( DeviceInfor.Run.Status == DeviceStandy )
				{
						if( ++count0 >= 3000 )   //3S
						{
								count0 = 0;		
								DeviceInfor.Run.Self3sStartFlag = FALSE;
								DeviceInfor.Run.Self3sFlag      = TRUE; 
						}
				}
			
			 /*冲洗状态计时*/
			 else if( DeviceInfor.Run.Status == DeviceRinse )
			 {
				    if( ++count1 >= 1000 )   //1S
						{
								count1 = 0;	
                New_Filter_Rinse_Time();  //累计新膜冲洗时间 							
						}
			 }
			 
			 /*正常工作状态计时*/
			 else if( DeviceInfor.Run.Status == DeviceWork )
			 {
					 Time_Slice();	        //时间片任务	 					 
			 }
			

		
	   delay_decrement();
	  
	   TimeStamp++;          //时间戳
			 
	   Usart0_Rec_TimeOut_FeeDog();  //串口0软件看门狗
    			 
			 
}
