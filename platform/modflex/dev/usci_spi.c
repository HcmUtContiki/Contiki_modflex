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

void usci_spi_init(uint8_t spi_mode)
{
  SPI_UCCTL1 |= UCSWRST;                             /* Hold peripheral in reset state */
  SPI_UCCTL0 = spi_mode + UCMSB + UCMST + UCSYNC;
  SPI_UCCTL1 = (SPI_CLK_SRC << 6) + UCSWRST;
  SPI_UCBR = SPI_CLK_DIV;

  #if(SPI_USE_USCI < 2)
    SPI_UCMCTL = 0;
  #endif

  SPI_UCCTL1 &= ~UCSWRST;
}

void usci_spi_uninit(void)
{
  SPI_UCCTL1 |= UCSWRST;
}

//--------------------------------------------------------------------------------------------------
uint8_t usci_spi_sendByte(uint8_t data)
{
  SPI_UCTXBUF = data;                     // write
  while ((SPI_UCIFG & UCRXIFG) == 0);     // wait for transfer to complete
  return (SPI_UCRXBUF);                   // dummy read to clear the RX flag
}

//--------------------------------------------------------------------------------------------------
void usci_spi_readFrame(uint8_t* pBuffer, uint16_t size)
{
  uint8_t i = 0;
  for (i = 0; i < size; i++)
  {
    SPI_UCTXBUF = DUMMY_CHAR;             // dummy write
    while ((SPI_UCIFG & UCRXIFG) == 0);   // wait for transfer to complete
    pBuffer[i] = SPI_UCRXBUF;
  }
}

//--------------------------------------------------------------------------------------------------
void usci_spi_sendFrame(const uint8_t* pBuffer, uint16_t size)
{
  uint16_t i = 0;
  volatile uint8_t x;
  for (i = 0; i < size; i++)
  {
    SPI_UCTXBUF = pBuffer[i];
    while ((SPI_UCIFG & UCTXIFG) == 0);  // wait for TX buffer to be ready
  }
  while ((SPI_UCSTAT & UCBUSY) == 0);    // wait for transfer to complete
  x = SPI_UCRXBUF;                       // dummy read to clear the RX flag
}

