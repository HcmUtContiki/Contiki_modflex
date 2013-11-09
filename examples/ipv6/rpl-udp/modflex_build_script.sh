#!/bin/sh
rm *.modflex; rm *.hex;
make -f Makefile.server TARGET=modflex;
make -f Makefile.client1 TARGET=modflex; mv udp-client.hex udp-client1.hex;
touch ../../../platform/modflex/contiki-modflex-main.c;
make -f Makefile.client2 TARGET=modflex; mv udp-client.hex udp-client2.hex
