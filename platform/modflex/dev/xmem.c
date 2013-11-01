/*
 * Copyright (c) 2013, Faculty of Computer Science & Engineering
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/**
 * \author Nguyen Quang Huy
 *
 */

#include "contiki.h"

#include "dev/fram.h"
#include "dev/xmem.h"
#include "dev/watchdog.h"

/*---------------------------------------------------------------------------*/
/*
 * Initialize external FRAM
 */
void xmem_init(void)
{
  if (fram_init(FRAM_MODE0) == ERROR_INIT_FAIL)
  {

  }
  else
  {

  }
}
/*---------------------------------------------------------------------------*/
int xmem_pread(void *_p, int size, unsigned long offset)
{
  /* Energy estimation mechanism */
  ENERGEST_ON(ENERGEST_TYPE_FLASH_READ);

  fram_readMemory(offset, (uint8_t *)_p, size);

  ENERGEST_OFF(ENERGEST_TYPE_FLASH_READ);

  return size;
}

/*---------------------------------------------------------------------------*/
int xmem_pwrite(const void *_buf, int size, unsigned long addr)
{
  ENERGEST_ON(ENERGEST_TYPE_FLASH_WRITE);
  
  fram_writeMemory(addr, (uint8_t *)_buf, size);

  ENERGEST_OFF(ENERGEST_TYPE_FLASH_WRITE);

  return size;
}
/*---------------------------------------------------------------------------*/
int xmem_erase(long size, unsigned long addr)
{
  unsigned long end = addr + size;

  watchdog_stop();

  for (; addr < end; addr++)
  {
    fram_writeByte(addr, 0);
  }

  watchdog_start();

  return size;
}
/*---------------------------------------------------------------------------*/
