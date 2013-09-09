/*
 * Copyright (c) 2013, Nguyen Quang Huy
 * All rights reserved.
 *
 * This file is part of the Contiki operating system.
 *
 */
#include "contiki.h"
#include "usci_spi.h"
#include "spi_internal.h"
#include "fram.h"

void fram_init(uint8_t spi_mode)
{
  CSDIR |= (1 << CSPIN);
  FRAM_DISABLE;

  HOLDDIR |= (1 << HOLDPIN);
  HOLD_DISABLE;

  WPDIR |= (1 << WPPIN);
  WP_DISABLE;

  #if (SPI_USE_USCI == 1)
    P5DIR &= ~0x80;   // P5.7 = USCI_A1 MISO as input
    P5DIR |= 0x40;    // P5.6 = USCI_A1 MOSI as output
    P3DIR |= 0x40;    // P3.6 = USCI_A1 SCK as output

    P3SEL |= 0x40;    // P3.6    USCI_A1 option selected   0100 0000B
    P5SEL |= 0x0C0;   // P5.7,6  USCI_A1 option selected   1100 0000
  #elif (SPI_USE_USCI == 3)
    P5DIR &= ~0x10;   // P5.4 = USCI_B1 MISO as input
    P3DIR |= 0x80;    // P3.7 = USCI_B1 MOSI as output
    P5DIR |= 0x20;    // P5.5 = USCI_B1 SCK as output

    P3SEL |= 0x80;    // P3.7    USCI_B1 option selected   1000 0000
    P5SEL |= 0x30;    // P5.5,4  USCI_B1 option selected   0011 0000
  #endif

  usci_spi_init(spi_mode);
  FRAM_ENABLE;
}

void fram_writeEnable(void)
{
  usci_spi_sendByte(CMD_WREN);
}

void fram_writeDisable(void)
{
  usci_spi_sendByte(CMD_WRDI);
}

uint8_t fram_readStatusRegister(void)
{
  return (usci_spi_sendByte(CMD_RDSR));
}

void fram_writeStatusRegister(uint8_t val)
{
  fram_writeEnable();
  usci_spi_sendByte(CMD_WRSR);
  fram_writeDisable();
}

uint8_t fram_readMemory(void)
{
  return (usci_spi_sendByte(CMD_READ));
}

uint8_t fram_writeMemory(void)
{
  return (usci_spi_sendByte(CMD_WRITE));
}

void fram_readDeviceID(uint8_t* pDeviceID)
{
  usci_spi_sendByte(CMD_RDID);
  usci_spi_readFrame(pDeviceID, 4);
}

uint8_t fram_fastReadMemory(void)
{
  return (usci_spi_sendByte(CMD_FSTRD));
}

void fram_sleepMode(void)
{
  usci_spi_sendByte(CMD_SLEEP);
}
