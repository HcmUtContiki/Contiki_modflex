#!/bin/sh
rm *.modflex; rm *.hex;
make -f Makefile.sink TARGET=modflex;
make -f Makefile.sender1 TARGET=modflex; mv udp-sender.hex udp-sender1.hex
touch ../../../platform/modflex/contiki-modflex-main.c;
make -f Makefile.sender2 TARGET=modflex; mv udp-sender.hex udp-sender2.hex
