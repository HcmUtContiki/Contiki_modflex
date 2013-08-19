/*
 * Copyright (c) 2007, Swedish Institute of Computer Science.
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
 * $Id: radio-test.c,v 1.6 2010/01/15 10:32:36 nifi Exp $
 *
 * -----------------------------------------------------------------
 *
 * Author  : Adam Dunkels, Joakim Eriksson, Niclas Finne
 * Created : 2006-03-07
 * Updated : $Date: 2010/01/15 10:32:36 $
 *           $Revision: 1.6 $
 *
 * Simple application to indicate connectivity between two nodes:
 *
 * - Red led indicates a packet sent via radio (one packet sent each second)
 * - Green led indicates that this node can hear the other node but not
 *   necessary vice versa (unidirectional communication).
 * - Blue led indicates that both nodes can communicate with each
 *   other (bidirectional communication)
 */

#include "contiki.h"
#include "net/rime.h"
#include "dev/leds.h"
#include "dev/button-sensor.h"
#include "dev/cc2520.h"
#include <stdio.h>
#include <string.h>
#include "leds-arch.h"
PROCESS(radio_test_process, "Radio test");
AUTOSTART_PROCESSES(&radio_test_process);

#define ON  1
#define OFF 0

#define HEADER "RTST"
#define PACKET_SIZE 20
#define PORT 9345

struct indicator {
  int onoff;
  int led;
  clock_time_t interval;
  struct etimer timer;
};
/*---------------------------------------------------------------------*/
static char count = 0;
static unsigned int NoReceivedPacket = 0;
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
}
/*---------------------------------------------------------------------------*/
void 
led_high(){
   leds_on (LEDS_7);
   leds_off(LEDS_6);
}
/*---------------------------------------------------------------------------*/
void 
led_low(){
   leds_on (LEDS_6);
   leds_off(LEDS_7);
}
/*---------------------------------------------------------------------------*/

static void
abc_recv(struct abc_conn *c)
{
  /* packet received */

  if(packetbuf_datalen() < PACKET_SIZE
     || strncmp((char *)packetbuf_dataptr(), HEADER, sizeof(HEADER))) {
    /* invalid message */
    printf("Receive invalid packet !!!! \n");

  } else {
     NoReceivedPacket++;
     if(NoReceivedPacket >= BIT5){
        count = NoReceivedPacket >> 5;
        led_high();
        display_leds(count);
     }else{
        count = NoReceivedPacket & 0xff;
        led_low();
        display_leds(count);
     }
  }
}
static const struct abc_callbacks abc_call = {abc_recv};
static struct abc_conn abc;
/*---------------------------------------------------------------------*/
PROCESS_THREAD(radio_test_process, ev, data)
{
  PROCESS_BEGIN();
  abc_open(&abc, PORT, &abc_call);
  SENSORS_ACTIVATE(button_1_sensor);
  SENSORS_ACTIVATE(button_2_sensor);
  SENSORS_ACTIVATE(button_3_sensor);
  NoReceivedPacket = 0;
  while(1) {
    PROCESS_WAIT_EVENT();
    if(ev == sensors_event){
       if(data == &button_1_sensor){
          leds_off(LEDS_6);
          leds_off(LEDS_7);
          display_leds(0);
       } else if(data == &button_2_sensor){
          count = NoReceivedPacket & 0xff;
          led_low();
          display_leds(count);
       } else if(data == &button_3_sensor){
          count = NoReceivedPacket >> 5;
          led_high();
          display_leds(count);
       }
    }
  }
  PROCESS_END();
}
/*---------------------------------------------------------------------*/
