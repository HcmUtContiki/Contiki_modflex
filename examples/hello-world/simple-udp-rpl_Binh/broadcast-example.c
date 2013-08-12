/*
 * Copyright (c) 2011, Swedish Institute of Computer Science.
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
 */

#include "contiki.h"
#include "lib/random.h"
#include "sys/ctimer.h"
#include "sys/etimer.h"
#include "net/uip.h"
#include "net/uip-ds6.h"
#include "dev/leds.h"
#include "simple-udp.h"
#include "dev/button-sensor.h"
#include "leds-arch.h"
#include <stdio.h>
#include <string.h>

#define UDP_PORT 1234

#define SEND_INTERVAL		(20 * CLOCK_SECOND)
#define SEND_TIME		(random_rand() % (SEND_INTERVAL))

static struct simple_udp_connection broadcast_connection;
extern unsigned short node_id;
static unsigned short sender_node_id = 0; 
static char send_count = 0;
static char recieve_count = 0;

/*---------------------------------------------------------------------------*/
PROCESS(broadcast_example_process, "UDP broadcast example process");
AUTOSTART_PROCESSES(&broadcast_example_process);
/*---------------------------------------------------------------------------*/
void display_led123(char count){
  //led 1
  if(count & BIT0){ leds_on( LEDS_1); }
  else{            leds_off( LEDS_1); }

  //led 2
  if(count & BIT1){ leds_on( LEDS_2); }
  else{            leds_off( LEDS_2); }

  //led 3
  if(count & BIT2){ leds_on( LEDS_3); }
  else{            leds_off( LEDS_3); }
}
void display_led567(char count){
  //led 1
  if(count & BIT0){ leds_on( LEDS_5); }
  else{            leds_off( LEDS_5); }

  //led 2
  if(count & BIT1){ leds_on( LEDS_6); }
  else{            leds_off( LEDS_6); }

  //led 3
  if(count & BIT2){ leds_on( LEDS_7); }
  else{            leds_off( LEDS_7); }
}
/*---------------------------------------------------------------------------*/

static void
receiver(struct simple_udp_connection *c,
         const uip_ipaddr_t *sender_addr,
         uint16_t sender_port,
         const uip_ipaddr_t *receiver_addr,
         uint16_t receiver_port,
         const uint8_t *data,
         uint16_t datalen)
{
  sender_node_id = data[0];
  printf("send from [%u] to [%u]: Data received on port %d from port %d with" 
	"length %d, data[%u]\n", sender_node_id, node_id, receiver_port, 
		sender_port, datalen, sender_node_id);
  display_led567(recieve_count ++);
  if(recieve_count == 8) {recieve_count = 0;}
}
/*---------------------------------------------------------------------------*/

unsigned short get_node_id()
{
    return node_id;
}
/*---------------------------------------------------------------------------*/

PROCESS_THREAD(broadcast_example_process, ev, data)
{
  //unsigned short temp = node_id + 65;
  static struct etimer periodic_timer;
  static struct etimer send_timer;
  uip_ipaddr_t addr;

  PROCESS_BEGIN();
  simple_udp_register(&broadcast_connection, UDP_PORT,
                      NULL, UDP_PORT,
                      receiver);
  etimer_set(&periodic_timer, SEND_INTERVAL);
  while(1) {
      PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
      etimer_reset(&periodic_timer);
      //etimer_set(&send_timer, SEND_TIME);
      //PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&send_timer));
      display_led123(send_count ++);
      if(send_count == 8) {send_count = 0;}
      printf("node_id[%d] Sending broadcast; reveiced[%d]\n", node_id, sender_node_id);  
      uip_create_linklocal_allnodes_mcast(&addr);
      simple_udp_sendto(&broadcast_connection, "BINH", 4, &addr);
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/

