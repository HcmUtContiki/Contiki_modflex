/*
 * Copyright (c) 2013, Nguyen Quang Huy
 * All rights reserved.
 *
 * This file is part of the Contiki operating system.
 *
 */
#ifndef __FRAM_H__
#define __FRAM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "usci_spi.h"


#define FRAM_MODE0  SPI_MODE0
#define FRAM_MODE3  SPI_MODE3

/* Define I/O for controlling CS, WP and HOLD pin */
#define CSDIR       P1DIR
#define CSOUT       P1OUT
#define CSPIN       0       // P1.0

#define HOLDDIR     P7DIR
#define HOLDOUT     P7OUT
#define HOLDPIN     7       // P7.7

#define WPDIR       P1DIR
#define WPOUT       P1OUT
#define WPPIN       1       // P1.1

#define FRAM_DISABLE     (CSOUT |= (1 << CSPIN))
#define FRAM_ENABLE      (CSOUT &= ~(1 << CSPIN))

#define HOLD_DISABLE     (HOLDOUT |= (1 << HOLDPIN))
#define HOLD_ENABLE      (HOLDOUT &= ~(1 << HOLDPIN))

#define WP_DISABLE       (WPOUT |= (1 << WPPIN))
#define WP_ENABLE        (WPOUT &= ~(1 << WPPIN))


/* FRAM Command */
#define CMD_WREN        0x06  // Set Write Enable Latch     0000 0110B
#define CMD_WRDI        0x04  // Reset Write Enable Latch   0000 0100B
#define CMD_RDSR        0x05  // Read Status Register       0000 0101B
#define CMD_WRSR        0x01  // Write Status Register      0000 0001B
#define CMD_READ        0x03  // Read Memory Code           0000 0011B
#define CMD_WRITE       0x02  // Write Memory Code          0000 0010B
#define CMD_RDID        0x9F  // Read Device ID             1001 1111B
#define CMD_FSTRD       0x0B  // Fast Read Memory Code      0000 1011B
#define CMD_SLEEP       0xB9  // Sleep Mode                 1011 1001B

void fram_init(uint8_t spi_mode);
uint8_t fram_readStatusRegister(void);
uint8_t fram_readMemory(void);
void fram_readDeviceID(uint8_t *pDeviceID);
uint8_t fram_fastReadMemory(void);
void fram_sleepMode(void);

#endif /* __FRAM_H__ */

