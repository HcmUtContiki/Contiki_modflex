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
#define MAXPACKET 100
#define HEADER "RTST"
#define PACKET_SIZE 20
#define PORT 9345

struct indicator {
  int onoff;
  int led;
  clock_time_t interval;
  struct etimer timer;
};
static struct etimer send_timer;
static struct indicator flash;

/*---------------------------------------------------------------------*/
static void
set(struct indicator *indicator, int onoff) {
  if(indicator->onoff ^ onoff) {
    indicator->onoff = onoff;
    if(onoff) {
      leds_on(indicator->led);
    } else {
      leds_off(indicator->led);
    }
  }
  if(onoff) {
    etimer_set(&indicator->timer, indicator->interval);
  }
}
/*---------------------------------------------------------------------------*/
static unsigned int NoPacket = 0;
void
leds_count(char count) 
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
/*---------------------------------------------------------------------*/
static void
abc_recv(struct abc_conn *c)
{
}
static const struct abc_callbacks abc_call = {abc_recv};
static struct abc_conn abc;
/*---------------------------------------------------------------------*/
PROCESS_THREAD(radio_test_process, ev, data)
{
  static uint8_t txpower;
  static uint8_t txpower_idx = 4; 
  static uint8_t txpower_arr[] = { 0x03, 0x2C, 0x88,
                                   0x81, 0x32, 0x13,
                                   0xAB, 0xF2, 0xF7};
/*
	  0x03 -> -18 dBm
	  0x2C -> -7 dBm
	  0x88 -> -4 dBm
	  0x81 -> -2 dBm
	  0x32 -> 0 dBm
	  0x13 -> 1 dBm
	  0xAB -> 2 dBm
	  0xF2 -> 3 dBm
	  0xF7 -> 5 dBm
*/
  PROCESS_BEGIN();


  /* Initialize the indicators */
  flash.interval = 1;
  flash.led = LEDS_6;
  
  abc_open(&abc, PORT, &abc_call);
  etimer_set(&send_timer, CLOCK_SECOND);
  SENSORS_ACTIVATE(button_1_sensor);
  SENSORS_ACTIVATE(button_2_sensor);
  SENSORS_ACTIVATE(button_3_sensor);
  leds_count(txpower_idx + 1) ;
  NoPacket = 0;
  while(1) {
    PROCESS_WAIT_EVENT();
    if (ev == PROCESS_EVENT_TIMER) {
       if(data == &send_timer) {
          if(NoPacket == MAXPACKET){
             leds_on(LEDS_7);
          }else{
             etimer_reset(&send_timer);
             NoPacket ++;
          }
          packetbuf_copyfrom(HEADER, sizeof(HEADER));
          ((char *)packetbuf_dataptr())[sizeof(HEADER)] = OFF;
          /* send arbitrary data to fill the packet size */
          packetbuf_set_datalen(PACKET_SIZE);
          set(&flash, ON);
          abc_send(&abc);
       }else if(data == &flash.timer){
          set(&flash, OFF);
       }
    }
    if(ev == sensors_event){
       if(data == &button_1_sensor){
          etimer_set(&send_timer, CLOCK_SECOND);
          leds_off(LEDS_7);
          NoPacket =0;
       } else if(data == &button_2_sensor){
          txpower_idx ++;
          if(txpower_idx > 8) {txpower_idx = 0;}
       } else if(data == &button_3_sensor){
          txpower_idx --;
          if(txpower_idx > 8) {txpower_idx = 8;}
       }
       leds_count(txpower_idx + 1) ;
       txpower = txpower_arr[txpower_idx];
       cc2520_set_txpower(txpower) ;
    }
  }
  PROCESS_END();
}
/*---------------------------------------------------------------------*/
