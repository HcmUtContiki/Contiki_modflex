#include "contiki.h"
#include "net/rime.h"
#include "dev/leds.h"
#include "dev/button-sensor.h"
#include "dev/serial-line.h"
//#include "dev/cc2520.h"
#include <stdio.h>
#include <string.h>
#include "leds-arch.h"
PROCESS(radio_test_process, "Radio test");
AUTOSTART_PROCESSES(&radio_test_process);


PROCESS_THREAD(radio_test_process, ev, data)
{
  PROCESS_BEGIN();
  uart0_writeString("Hello world");
  while(1)
  {
    static struct etimer et;
    etimer_set(&et, CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    uart0_writeString("Hello world");
  }
  PROCESS_END();
}
