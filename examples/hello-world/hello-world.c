/*
 * Copyright (c) 2006, Swedish Institute of Computer Science.
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
 *
 * $Id: hello-world.c,v 1.1 2006/10/02 21:46:46 adamdunkels Exp $
 */

/**
 * \file
 *         A very simple Contiki application showing how Contiki programs look
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#include "contiki.h"
#include <stdio.h> /* For printf() */
#include "dev/leds.h"
#include "dev/button-sensor.h"
#include "leds-arch.h"

/*---------------------------------------------------------------------------*/
PROCESS(hello_world_process, "Hello world process");
AUTOSTART_PROCESSES(&hello_world_process);
/*---------------------------------------------------------------------------*/
static char led_count = 0x55;
/*---------------------------------------------------------------------------*/
void
display_leds(int count)
{
  //led 1
  if(count & BIT0){ leds_on( LEDS_1); }
  else{            leds_off( LEDS_1); }

  //led 2
  if(count & BIT1){ leds_on( LEDS_2); }
  else{            leds_off( LEDS_2); }

  //led 3
  if(count & BIT2){ leds_on( LEDS_3); }
  else{            leds_off( LEDS_3); }

  //led 4
  if(count & BIT3){ leds_on( LEDS_4); }
  else{            leds_off( LEDS_4); }

  //led 5
  if(count & BIT4){ leds_on( LEDS_5); }
  else{            leds_off( LEDS_5); }

  //led 6
  if(count & BIT5){ leds_on( LEDS_6); }
  else{            leds_off( LEDS_6); }

  //led 7
  if(count & BIT6){ leds_on( LEDS_7); }
  else{            leds_off( LEDS_7); }
}
/*---------------------------------------------------------------------------*/

PROCESS_THREAD(hello_world_process, ev, data)
{
  PROCESS_BEGIN();
  SENSORS_ACTIVATE(button_1_sensor);
  SENSORS_ACTIVATE(button_2_sensor);
  SENSORS_ACTIVATE(button_3_sensor);
  display_leds(led_count);
  while(1){
     printf("Hello, world[this device has contiki OS inside]\n");
     PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event);
     if(data == &button_1_sensor){
        led_count ++;
     }
     else if(data == &button_2_sensor){
        led_count --;
     }
     else if(data == &button_3_sensor){
        led_count ^=0xFF;
     }
     display_leds(led_count);
     //for(i = 0 ; i < 5; i ++){
        clock_delay(6500);
     //}
  }
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
