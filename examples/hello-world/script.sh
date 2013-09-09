#!/bin/bash

echo "Creating hex file"
msp430-objcopy hello-world.modflex -O ihex hello-world.hex

echo "Programming device..."
mspdebug rf2500 "prog hello-world.hex" "run"

echo "Done"
