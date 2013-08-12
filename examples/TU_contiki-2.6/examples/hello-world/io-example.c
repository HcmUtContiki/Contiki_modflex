 #include "contiki.h"
 #include "dev/serial-line.h"
 #include <stdio.h>
 #include "dev/leds.h"

#define GETBITVECTOR(LEDS_COLOR) (1<<LEDS_COLOR)

//The function used to toggle one three led
// Green, Yellow, Red based on LED identifiers
void toggleLeds(unsigned char led_identifier, unsigned char *p_led_status)
{
   unsigned char led_status = *p_led_status;
   switch (led_identifier)
   {
      case LEDS_GREEN:
      case LEDS_YELLOW:
      case LEDS_RED:
      {         
         led_status ^= 1 << led_identifier;
         break;
      }
      case LEDS_ALL:
      {
         led_status ^= 1 << LEDS_GREEN;
         led_status ^= 1 << LEDS_YELLOW;
         led_status ^= 1 << LEDS_RED;
         break;
      }
      default:
      {
         printf("Unknow led_identifier");
         break;
      }
   }
   
   *p_led_status = led_status; 
}

 PROCESS(test_serial, "Serial line test process");
 AUTOSTART_PROCESSES(&test_serial);
 
 PROCESS_THREAD(test_serial, ev, data)
 {
   PROCESS_BEGIN();
   unsigned char led_status = 0;
   //init the led driver for board
   leds_init();
   
   for(;;) {
     PROCESS_YIELD();
     if(ev == serial_line_event_message) {
       printf("received line: %s\n", (char *)data);
       led_status = leds_get();
       
       //printf("Current status: GREEN = %d YELLOW = %d RED = %d", )
       printf("Current status: 0x%x \n", led_status);
       
       //Get the first letter user input line
       unsigned char choosen = ((char *)data)[0];
       switch (choosen)
       {
          case 'g':
          case 'G':
          {             
             leds_toggle(GETBITVECTOR(LEDS_GREEN));
			 led_status = leds_get();
             printf("Toggle GREEN LED --> LED status: 0x%x \n", led_status);       
             break;
          }
          
          case 'y':
          case 'Y':
          {             
             leds_toggle(GETBITVECTOR(LEDS_YELLOW));
			 led_status = leds_get();
             printf("Toggle YELLOW LED --> LED status: 0x%x \n", led_status);       
             break;
          }
          
          case 'r':
          case 'R':
          {
             leds_toggle(GETBITVECTOR(LEDS_RED));
			 led_status = leds_get();
             printf("Toggle RED LED --> LED status: 0x%x \n", led_status);
             break;
          }
          
          case 'a':
          case 'A':
          {             
             leds_toggle(GETBITVECTOR(LEDS_GREEN) | GETBITVECTOR(LEDS_YELLOW) | GETBITVECTOR(LEDS_RED));
			 led_status = leds_get();
             printf("Toggle all three LED --> LED status: 0x%x \n", led_status);
             break;
          }
          default:
          {
             printf("Unkown command, please try again \n" );
             break;
          }
       }
     }
   }
   PROCESS_END();
 }
