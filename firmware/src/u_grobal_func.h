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

#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_adc.h"
#include "r_cg_timer.h"

#include "r_cg_userdefine.h"

/* common functions declare */
void 		u_g_param_Init(void);

char 		get_speed_change_flag(void);
void 		reset_speed_change_flag(void);

char 		get_clockType(void);
void 		set_clockType(char c);

char 		get_kind_param(void);
void 		set_kind_param(char c);

uint16_t 	get_speed(void);
void 		set_speed(uint16_t u);

uint16_t 	get_step01(void);
void 		set_step01(uint16_t u);

uint16_t 	get_step02(void);
void 		set_step02(uint16_t u);

uint16_t 	get_step03(void);
void 		set_step03(uint16_t u);

uint16_t 	get_step04(void);
void 		set_step04(uint16_t u);

uint16_t 	get_input(void);
void 		set_input(uint16_t u);
