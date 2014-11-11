/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING 
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT 
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR 
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE 
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software 
* and to discontinue the availability of this software.  By using this software, 
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011, 2013 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_timer_user.c
* Version      : CodeGenerator for RL78/G12 V2.00.00.07 [22 Feb 2013]
* Device(s)    : R5F10278
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for TAU module.
* Creation Date: 2013/06/04
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
#pragma interrupt INTTM00 r_tau0_channel0_interrupt
#pragma interrupt INTTM01 r_tau0_channel1_interrupt
#pragma interrupt INTTM02 r_tau0_channel2_interrupt
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_timer.h"
/* Start user code for include. Do not edit comment generated here */
#include "u_grobal_func.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */

#define	TRIGGER_SYNC		// Trigger‚ğPWM On/Off‚É‡‚í‚¹‚é, V9‚ÅÀ‘•

static char	g_count_sw_off;
static char	g_count_sw_on;

static uint16_t g_current_energy;
static uint16_t g_energy_mask;
static char	g_current_step;

static uint16_t g_count_seqTime;
static char	g_clockOn;
static char	g_clockOff;
static char	g_bef_input_off;
static char	g_hold;
static char	g_count_input_on;
static char	g_count_input_off;

static uint16_t g_t_input;

static uint16_t g_env_input;
static uint16_t g_attack_coef;
static uint16_t g_decay_coef;

static uint16_t g_count_Trig_zero;

static uint16_t g_step_speed;
static uint16_t g_step_speed_hi;

static char	g_ad_sw;

static const uint16_t g_SpeedCurve[] = {
	36000,18000,12000, 9600, 8000, 6857, 6000, 5143, 4500,
	 4000, 3600, 3273, 3000, 2250, 1500, 1000,  581,
};

void u_change_step(void);
void u_env_follower(uint16_t l_input);

/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_tau0_channel0_interrupt
* Description  : This function is INTTM00 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_tau0_channel0_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
	uint32_t	l_temp32;
	uint16_t	temp_result;
	
	TDR01 = g_current_energy;
	
	switch(g_current_step){
	case STEP1:	l_temp32 = get_step01();	break;
	case STEP2:	l_temp32 = get_step02();	break;
	case STEP3:	l_temp32 = get_step03();	break;
	case STEP4:	l_temp32 = get_step04();	break;
	default:	break;
	}
	l_temp32 = l_temp32 * (uint32_t)g_t_input;
	l_temp32 = l_temp32 >> 10;
	g_current_energy = (uint16_t)l_temp32 & g_energy_mask;

#ifndef TRIGGER_SYNC
	if(g_current_energy > 0x0){
		if(g_count_Trig_zero > 10){
			P4.2 = 1;
		}else{
			g_count_Trig_zero++;
		}
	}
#endif
	
	if(ADIF == 1){
		ADIF = 0;
		R_ADC_Get_Result(&temp_result);
		
		if(g_ad_sw == INPUT){
			if(temp_result < 15){
				set_input(0);
			}else{
				set_input(temp_result);
			}
			DI();
			g_t_input = get_input();
			EI();
			if(g_t_input > 0x200){
				if(g_bef_input_off > 0){
					set_input(temp_result);
					g_bef_input_off = 0;
					if (g_clockOff) {
						g_clockOff = 0;
						g_clockOn = 0;
					} else {
						g_clockOn = 1;
					}
				}
			}else{
				if(g_t_input < 0x1E0){
					if (!g_bef_input_off) {
						g_bef_input_off = 1;
						if (g_clockOn) {
							g_clockOn = 0;
							g_clockOff = 0;
						} else {
							g_clockOff = 1;
						}
					}
				}
			}
			g_ad_sw = ELSE; // set read ad not input.
			
			switch(get_kind_param()){
				case SPEED:	set_kind_param(STEP1);
						ADS = _12_AD_INPUT_CHANNEL_18;
						break;
				case STEP1:	set_kind_param(STEP2);
						ADS = _11_AD_INPUT_CHANNEL_17;
						break;
				case STEP2:	set_kind_param(STEP3);
						ADS = _10_AD_INPUT_CHANNEL_16;
						break;
				case STEP3:	set_kind_param(STEP4);
						ADS = _03_AD_INPUT_CHANNEL_3;
						break;
				case STEP4:	set_kind_param(SPEED);
						ADS = _02_AD_INPUT_CHANNEL_2;
						break;
				default:	break;
			}
		}else{
			switch(get_kind_param()){
				case SPEED:	set_speed(temp_result);
						break;
				case STEP1:	set_step01(temp_result);
						break;
				case STEP2:	set_step02(temp_result);
						break;
				case STEP3:	set_step03(temp_result);
						break;
				case STEP4:	set_step04(temp_result);
						break;
				default:	break;
			}
			g_ad_sw = INPUT; // set read ad input.
			ADS = _00_AD_INPUT_CHANNEL_0;
		}
		
		R_ADC_Start();
	}
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_tau0_channel1_interrupt
* Description  : This function is INTTM01 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_tau0_channel1_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_tau0_channel2_interrupt
* Description  : This function is INTTM02 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_tau0_channel2_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
	switch(get_clockType()){
	case INTERNAL:	
			if (g_t_input >= 40) {		// 0.2VˆÈã‚Ìê‡‚Ì‚İ“®‚©‚·
				if(g_count_seqTime > g_step_speed){
					u_change_step();
					g_count_seqTime = 0;
					g_clockOn = 0;
				} else if (g_count_seqTime > g_step_speed_hi) {
					g_energy_mask = 0;
#ifdef TRIGGER_SYNC
					P4.2 = 0;
#endif
				}
			}
			break;
	case EXTERNAL:
			if(g_clockOn){
				g_clockOn = 0;
				u_change_step();
				g_count_seqTime = 0;
			}
			if(g_clockOff) {
				g_clockOff = 0;
				g_energy_mask = 0;
#ifdef TRIGGER_SYNC
				P4.2 = 0;
#endif
			}
			break;
	default:	break;
	}
	
	g_count_seqTime++;
	
	if( P0.0 == INTERNAL && get_clockType() == EXTERNAL ){
		g_count_sw_on = 0;
		g_count_sw_off++;
		if( g_count_sw_off == 3 ){
			set_clockType(INTERNAL);
			g_count_sw_off = 0;
		}
	}else if( P0.0 != INTERNAL && get_clockType() == INTERNAL ){
		g_count_sw_off = 0;
		g_count_sw_on++;
		if( g_count_sw_on == 3 ){
			set_clockType(EXTERNAL);
			g_count_sw_on = 0;
		}
	}
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
void u_change_step(void)
{
#ifndef TRIGGER_SYNC
	P4.2 = 0;
	g_count_Trig_zero = 0;
#else
	P4.2 = 1;
#endif
	g_energy_mask = 0xffff;
	
	switch(g_current_step){
	case STEP1:	g_current_step = STEP2;
			P0.3 = 1; P0.2 = 0;
			break;
	case STEP2:	g_current_step = STEP3;
			P6.0 = 1; P0.3 = 0;
			break;
	case STEP3:	g_current_step = STEP4;
			P6.1 = 1; P6.0 = 0;
			break;
	case STEP4:	g_current_step = STEP1;
			P0.2 = 1; P6.1 = 0;
			break;
	default:	break;
	}
}
void u_calc_step_speed(void)
{
	uint32_t	l_speed32, l_temp32;
	uint16_t	l_temp16, l2_temp16;
	
	l_temp16 	= 0x3FF - get_speed();
	l2_temp16	= l_temp16;
	
	l_temp16 	= (l_temp16 >> 6);
	l2_temp16	-= (l_temp16 << 6);
	
	l_speed32	= g_SpeedCurve[l_temp16];
	l_temp32 	= g_SpeedCurve[l_temp16+1];
	
	l_speed32	*= (0x3FF - l2_temp16);
	l_temp32 	*= l2_temp16;
	
	l_speed32	+= l_temp32;
	l_speed32	= l_speed32 >> 10;
	
	//---- –³ğŒ‚É0.72
	l_speed32	*= 23;
	l_speed32	>>= 5;
	
	l_speed32	&= 0xFFFF;
	l_temp32 	= (l_speed32 >> 2);		// ˆê’UA–³‰¹•”•ªŠÔ‚ÅŒvZ
	
	if (l_temp32 < (12 * 50)) {
		l_temp32 = 12 * 50;
	}
	if (l_speed32 > (l_temp32 + (12 * 3))) {
		l_temp32 = l_speed32 - l_temp32;
	} else {
		l_temp32 = 12 * 3;
	}
	
	DI();
	g_step_speed	= (uint16_t)l_speed32;
	g_step_speed_hi	= (uint16_t)l_temp32;
	EI();
}

void u_timer_Init(void)
{
	g_current_energy= 0;
	g_energy_mask 	= 0;
	g_count_seqTime = 0;
	
	g_current_step 	= STEP1;
		
	P0.2 = 1;
	P0.3 = 0;
	P6.0 = 0;
	P6.1 = 0;
	
	g_ad_sw = ELSE;
	
	g_count_sw_off 	= 0;
	g_count_sw_on 	= 0;

	g_clockOn	= 0;
	g_count_Trig_zero = 0;
	
	g_env_input 	= 0x0;
	g_attack_coef 	= 0x100;
	g_decay_coef 	= 0x200;
	
	g_step_speed	= 0x200;
	g_step_speed	= 0x180;
	
	g_hold	= 0;
	g_count_input_on = 0;
	g_count_input_off= 0;
}
/* End user code. Do not edit comment generated here */
