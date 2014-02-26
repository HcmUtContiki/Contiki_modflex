/*
 *
 * Author: TuNguyen <tu_nguyen180889@yahoo.com>
 */

#ifdef SENSOR_CHANNEL_SCAN

#include "rpl.h"
#include "sys/ctimer.h"
#include "uip-debug.h"
#include "cc2520.h"

#define JOINING_NETWORK_TIMER               (5 * 60 * CLOCK_SECOND) // Five minutes to check parent
#define MAX_FAILING_JOIN_NERWORK_TIME       2
static struct ctimer join_network_timer;

PROCESS(channel_scanner, "Channel Scanner");

static void
handle_join_network_callback(void *in)
{
    printf("Channel Scan: In the callback!\n");
    static join_failure_count = 0;
    rpl_dag_t* default_dag = rpl_get_any_dag();

    /*Check to see this note is joining any dag
     *   1) If yes that mean this node belong to a specific network
     *   2) Reset the timer as well as find a new chance to join a network within
     *      another channel*/
    if (!default_dag)
    {
      uint8_t current_channel = cc2520_get_channel();
      printf("Not joining any network yet ! try with another channel %d \n", current_channel + 1);
      current_channel++;
      cc2520_set_channel(current_channel);
      join_failure_count    = current_channel / (MAX_CHANNEL);
      if (join_failure_count >= MAX_FAILING_JOIN_NERWORK_TIME)
      {
        //@TODO if can not join the network within specific time so go to
        // deep sleep mode
      }
    }
    else
    {
      printf("Joing network successful \n");
    }

    ctimer_reset(&join_network_timer);
}

PROCESS_THREAD(channel_scanner, ev, data)
{
  PROCESS_BEGIN();
  ctimer_set(&join_network_timer, JOINING_NETWORK_TIMER, handle_join_network_callback, (void*)NULL);
  PROCESS_END();
}

#endif
