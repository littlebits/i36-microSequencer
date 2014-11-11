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

#include "u_grobal_func.h"
#include <math.h>

// common parameters
static char	g_speed_change_flag;

static char	g_clockType;

static char	g_kind_param;

static uint16_t g_speed;
static uint16_t g_step01;
static uint16_t g_step02;
static uint16_t g_step03;
static uint16_t g_step04;

static uint16_t g_input_signal;

// common functions  
void u_g_param_Init(void)
{
	g_speed_change_flag = 0;
	
	g_clockType = 0;

	g_kind_param = 0;
	g_speed  = 128;
	g_step01 = 0;
	g_step02 = 0;
	g_step03 = 0;
	g_step04 = 0;
	
	g_input_signal = 0;
}
// speed changed /////
char get_speed_change_flag(void)
{
	return g_speed_change_flag;
}
void reset_speed_change_flag(void)
{
	g_speed_change_flag = 0;
}
// clock type /////
char get_clockType(void)
{
	return g_clockType;
}

void set_clockType(char c)
{
	if(g_clockType != c){
		g_clockType = c;
	}
}
// kind param (Which step value knob is looked now) /////
char get_kind_param(void)
{
	return g_kind_param;
}
void set_kind_param(char c)
{
	g_kind_param = c;
}
// speed /////
uint16_t get_speed(void)
{
	return g_speed;
}
void set_speed(uint16_t u)
{
	if(g_speed != u){
		g_speed = (u + g_speed) >> 1;
		g_speed_change_flag = 1;
	}
}
// step01 value /////
uint16_t get_step01(void)
{
	return g_step01;
}
void set_step01(uint16_t u)
{
	if(g_step01 != u){
		g_step01 = (u + g_step01) >> 1;
	}
}
// step02 value /////
uint16_t get_step02(void)
{
	return g_step02;
}
void set_step02(uint16_t u)
{
	if(g_step02 != u){
		g_step02 = (u + g_step02) >> 1;
	}
}
// step03 value /////
uint16_t get_step03(void)
{
	return g_step03;
}
void set_step03(uint16_t u)
{
	if(g_step03 != u){
		g_step03 = (u + g_step03) >> 1;
	}
}
// step04 value /////
uint16_t get_step04(void)
{
	return g_step04;
}
void set_step04(uint16_t u)
{
	if(g_step04 != u){
		g_step04 = (u + g_step04) >> 1;
	}
}
// input /////
uint16_t get_input(void)
{
	return g_input_signal;
}
void set_input(uint16_t u)
{
	if(g_input_signal != u){
		g_input_signal = (u + g_input_signal) >> 1;
	}
}
