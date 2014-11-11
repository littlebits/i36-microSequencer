/***********************************************************************************************************************
 * Copyright 2014 littleBits Electronics
 *
 * This file is part of i36-microSequencer.
 *
 * i36-microSequencer is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * i36-microSequencer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License at <http://www.gnu.org/licenses/> for more details.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_main.c
* Version      : CodeGenerator for RL78/G12 V2.00.00.07 [22 Feb 2013]
* Device(s)    : R5F10278
* Tool-Chain   : CA78K0R
* Description  : This file implements main function.
* Creation Date: 2013/06/04
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_adc.h"
#include "r_cg_timer.h"
/* Start user code for include. Do not edit comment generated here */
#include "u_grobal_func.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
void R_MAIN_UserInit(void);

/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
    R_MAIN_UserInit();
    /* Start user code. Do not edit comment generated here */
   	R_ADC_Start();
	R_TAU0_Channel0_Start();
	R_TAU0_Channel2_Start();
	
	while (1U)
	{
		if(get_speed_change_flag()){
			DI();
			reset_speed_change_flag();
			EI();
			u_calc_step_speed();
		}
	}
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: R_MAIN_UserInit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MAIN_UserInit(void)
{
    /* Start user code. Do not edit comment generated here */
    	EI();
	u_timer_Init();
	u_g_param_Init();

	TDR02	= 0x07FF;
	
	TDR00	= 0x03FF;		/* 24MHz / (1023+1)  nearly  24kHz , PWM master */
	TDR01	= 0x0200;		/* PWM slave */


	IICE0	= 0;

	SO0	|= 0x0001;
	SOE0	&= 0xFFF0;

	PM6.0	= 0;
	PM6.1	= 0;
	
	ADIF	= 0;

	set_kind_param(SPEED);

	ADS	= _02_AD_INPUT_CHANNEL_2;
	R_ADC_Set_OperationOn();
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
