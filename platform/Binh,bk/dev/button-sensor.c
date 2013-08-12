/*
 * Copyright (c) 2011, Swedish Institute of Computer Science
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 */
#include "lib/sensors.h"
#include "dev/hwconf.h"
#include "dev/button-sensor.h"
#include "isr_compat.h"

const struct sensors_sensor button_1_sensor;
const struct sensors_sensor button_2_sensor;
const struct sensors_sensor button_2_sensor;

static struct timer debouncetimer[3];
static int status_1(int type);
static int status_2(int type);
static int status_3(int type);

HWCONF_PIN(BUTTON_1, 2, 0);
HWCONF_IRQ(BUTTON_1, 2, 0);
HWCONF_PIN(BUTTON_2, 2, 1);
HWCONF_IRQ(BUTTON_2, 2, 1);
HWCONF_PIN(BUTTON_3, 2, 2);
HWCONF_IRQ(BUTTON_3, 2, 2);
/*---------------------------------------------------------------------------*/
ISR(PORT2, irq_p2)
{
  ENERGEST_ON(ENERGEST_TYPE_IRQ);

  if(BUTTON_1_CHECK_IRQ() && timer_expired(&debouncetimer[0])) {
    timer_set(&debouncetimer[0], CLOCK_SECOND / 4);
    sensors_changed(&button_1_sensor);
    LPM4_EXIT;
    P2IFG &= ~BIT0;
  }

  if(BUTTON_2_CHECK_IRQ() && timer_expired(&debouncetimer[1])) {
    timer_set(&debouncetimer[1], CLOCK_SECOND / 4);
    sensors_changed(&button_2_sensor);
    LPM4_EXIT;
    P2IFG &= ~BIT1;
  }

  if(BUTTON_3_CHECK_IRQ() && timer_expired(&debouncetimer[2])) {
    timer_set(&debouncetimer[2], CLOCK_SECOND / 4);
    sensors_changed(&button_3_sensor);
    LPM4_EXIT;
    P2IFG &= ~BIT2;
  }

  ENERGEST_OFF(ENERGEST_TYPE_IRQ);
}
/*---------------------------------------------------------------------------*/
static int
value_1(int type)
{
  return BUTTON_1_READ() || !timer_expired(&debouncetimer[0]);
}
/*---------------------------------------------------------------------------*/
static int
configure_1(int type, int value)
{
  if(type == SENSORS_ACTIVE) {
    if(value == 0) {
      /* Deactivate button sensor */
      BUTTON_1_DISABLE_IRQ();
    } else {
      /* Activate button sensor */
      if(!status_1(SENSORS_ACTIVE)) {
	timer_set(&debouncetimer[0], 0);
	BUTTON_1_IRQ_EDGE_SELECTD();

	BUTTON_1_SELECT();
	BUTTON_1_MAKE_INPUT();

	BUTTON_1_ENABLE_IRQ();
      }
    }
    return 1;
  }
  return 0;
}
/*---------------------------------------------------------------------------*/
static int
status_1(int type)
{
  switch(type) {
  case SENSORS_ACTIVE:
  case SENSORS_READY:
    return BUTTON_2_IRQ_ENABLED();
  default:
    return 0;
  }
}
/*---------------------------------------------------------------------------*/
static int
value_2(int type)
{
  return BUTTON_2_READ() || !timer_expired(&debouncetimer[1]);
}
/*---------------------------------------------------------------------------*/
static int
configure_2(int type, int value)
{
  if(type == SENSORS_ACTIVE) {
    if(value == 0) {
      /* Deactivate button sensor */
      BUTTON_2_DISABLE_IRQ();
    } else {
      /* Activate button sensor */
      if(!status_2(SENSORS_ACTIVE)) {
	timer_set(&debouncetimer[1], 0);
	BUTTON_2_IRQ_EDGE_SELECTD();

	BUTTON_2_SELECT();
	BUTTON_2_MAKE_INPUT();

	BUTTON_2_ENABLE_IRQ();
      }
    }
    return 1;
  }
  return 0;
}
/*---------------------------------------------------------------------------*/
static int
status_2(int type)
{
  switch(type) {
  case SENSORS_ACTIVE:
  case SENSORS_READY:
    return BUTTON_2_IRQ_ENABLED();
  default:
    return 0;
  }
}
/*---------------------------------------------------------------------------*/
static int
value_3(int type)
{
  return BUTTON_3_READ() || !timer_expired(&debouncetimer[2]);
}
/*---------------------------------------------------------------------------*/
static int
configure_3(int type, int value)
{
  if(type == SENSORS_ACTIVE) {
    if(value == 0) {
      /* Deactivate button sensor */
      BUTTON_3_DISABLE_IRQ();
    } else {
      /* Activate button sensor */
      if(!status_3(SENSORS_ACTIVE)) {
	timer_set(&debouncetimer[2], 0);
	BUTTON_3_IRQ_EDGE_SELECTD();

	BUTTON_3_SELECT();
	BUTTON_3_MAKE_INPUT();

	BUTTON_3_ENABLE_IRQ();
      }
    }
    return 1;
  }
  return 0;
}
/*---------------------------------------------------------------------------*/
static int
status_3(int type)
{
  switch(type) {
  case SENSORS_ACTIVE:
  case SENSORS_READY:
    return BUTTON_3_IRQ_ENABLED();
  default:
    return 0;
  }
}
/*---------------------------------------------------------------------------*/
SENSORS_SENSOR(button_1_sensor, BUTTON_1_SENSOR, value_1, configure_1, status_1);
SENSORS_SENSOR(button_2_sensor, BUTTON_2_SENSOR, value_2, configure_2, status_2);
SENSORS_SENSOR(button_3_sensor, BUTTON_3_SENSOR, value_3, configure_3, status_3);
