#include "led-debug.h"

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
void display_led123(int count)
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
}
void display_led567(int count)
{
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

