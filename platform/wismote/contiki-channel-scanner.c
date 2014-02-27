/*
 * Author: TuNguyen <tu_nguyen180889@yahoo.com>
 */

#if (SENSOR_CHANNEL_SCAN || GATEWAY_CHANNEL_SCAN)
#define DEBUG 1
#if CONTIKI_TARGET_WISMOTE
#define CC2520_RF 1
#endif
#if CONTIKI_TARGET_SKY
#define CC2420_RF 1
#endif

#include "rpl.h"
#include "sys/ctimer.h"
#include "uip-debug.h"
#if CC2520_RF
#include "cc2520.h"
#define  RF_NAME_PREFIX(function)          cc2520##function
#endif
#if CC2420_RF
#include "cc2420.h"
#define  RF_NAME_PREFIX(function)          cc2420##function
#endif

#define JOINING_NETWORK_TIMER               (60 * CLOCK_SECOND) // constant time to check parent
#define MAX_FAILING_JOIN_NERWORK_TIME        2

PROCESS(channel_scanner, "Channel Scanner");

/*
 * uint8_t gatewayChannelScan
 * -   Description: This function used to scan the most less interference, The result returned by
 *                  compare the value of RSSI of each channel, the smallest value will be return because the
 *                  more value of RSSI indicate that the more interference
 * -   Output     : The best silent channel
 */
#if GATEWAY_CHANNEL_SCAN
#define NBR_SAMPLES          0xff
uint8_t gatewayChannelScan()
{
  // Initialize the base channel and scan
  uint8_t best_channel = MIN_CHANNEL; // The base channel for scanning
  int     min_rssi     = 127;         // Initialize the min_rssi, RSSI is 2's complement 8 bit
                                      // So the maximum value is 127
  int     current_rssi = 0;
  int     channel_rssi_mapping[MAX_CHANNEL - MIN_CHANNEL + 1];

  // Scan the channel
  uint8_t index;
  for(index = 0; index <= MAX_CHANNEL - MIN_CHANNEL; index++) {

    uint8_t scan_count   = NBR_SAMPLES;
    RF_NAME_PREFIX(_set_channel)(MIN_CHANNEL + index);
    // Get the average rssi value in CHANNEL index
    while (scan_count > 0)
    {
       current_rssi = (scan_count ==  NBR_SAMPLES) ? RF_NAME_PREFIX(_rssi)() : current_rssi + RF_NAME_PREFIX(_rssi)();
       scan_count--;
    }
    // Get the mean value of previous and current value
    channel_rssi_mapping[index] = (current_rssi / NBR_SAMPLES);

    // If the current channel have minimum rssi so update the best channel which have less
    // interference
    if (min_rssi > channel_rssi_mapping[index])
    {
       min_rssi     = channel_rssi_mapping[index];
       best_channel = MIN_CHANNEL + index;
    }
    PRINTF("gatewayChannelScan(): channel: %d, current_rssi: %d \n", MIN_CHANNEL + index, channel_rssi_mapping[index]);
  }

  PRINTF("gatewayChannelScan(): best channel: %d, min_rssi: %d \n", best_channel, min_rssi);

  return best_channel;
}
#endif

#if SENSOR_CHANNEL_SCAN

static struct ctimer join_network_timer;
static void
handle_join_network_callback(void *in)
{
    PRINTF("Channel Scan: In the call-back\n");
    static uint8_t  join_failure_count                = 0;
    static uint16_t network_join_success_check        = 0;
    rpl_dag_t* default_dag = rpl_get_any_dag();

    /*Check to see this note is joining any dag
     *   1) If yes that mean this node belong to a specific network
     *   2) Reset the timer as well as find a new chance to join a network within
     *      another channel*/
    if (!default_dag)
    {
      uint8_t current_channel = RF_NAME_PREFIX(_get_channel)();
      current_channel++;
      if (current_channel > MAX_CHANNEL)
      {
        join_failure_count++;
        current_channel   = MIN_CHANNEL;
      }
      if (join_failure_count >= MAX_FAILING_JOIN_NERWORK_TIME)
      {
        //@TODO if can not join the network within specific time so go to
        // deep sleep mode
        join_failure_count = 0;
      }
      else
      {
        PRINTF("Channel Scan: Not joining any network yet ! try with another channel %d \n", current_channel);
        RF_NAME_PREFIX(_set_channel)(current_channel);
      }
      // Reset the timer to check join network or yet
      ctimer_set(&join_network_timer, JOINING_NETWORK_TIMER, handle_join_network_callback, (void*)NULL);
    }
    else
    {
      // If joining network successful let increase the timer by back-off duration
      // The new timeout should only 2^15 * JOINING_NETWORK_TIMER
      PRINTF("Channel Scan: Join network successful \n");
      network_join_success_check++;
      network_join_success_check = (15 == network_join_success_check) ?
                                   0 : network_join_success_check;
      uint32_t new_timeout = (2 << network_join_success_check) * JOINING_NETWORK_TIMER;
      ctimer_set(&join_network_timer, new_timeout, handle_join_network_callback, (void*)NULL);
    }
}
#endif

PROCESS_THREAD(channel_scanner, ev, data)
{
  PROCESS_BEGIN();
#if SENSOR_CHANNEL_SCAN
  ctimer_set(&join_network_timer, JOINING_NETWORK_TIMER, handle_join_network_callback, (void*)NULL);
#endif
#if GATEWAY_CHANNEL_SCAN
  //If defined the node as GATEWAY so it must decide to chose the global channel its network
  uint8_t rf_channel = gatewayChannelScan();
  RF_NAME_PREFIX(_set_channel)(rf_channel);
#endif
  PROCESS_END();
}

void init_scanner(void)
{
  process_start(&channel_scanner, NULL);
}

#endif
