#ifndef LED_DEBUG_H
#define LED_DEBUG_H

#include "contiki.h"
#include "dev/leds.h"
#include "leds-arch.h"

void display_leds(char count);
void display_led123(char count);
void display_led567(char count);

#endif
