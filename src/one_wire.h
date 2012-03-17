/*
 * one_wire.h
 * This file is part of the set of functions to handle Dallas "One Wire"
 * protocol and devices.
 *
 * Copyright (C) 2012 Krzysztof Kozik
 *
 * This set is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef _ONE_WIRE_H_
#define _ONE_WIRE_H_

#include "config.h"
#include <stdint.h>

struct sromcode
{
  uint8_t             family_code ;
  uint8_t             serial[6] ;
  uint8_t             crc ;
} ;

union uromcode
{
  struct sromcode     romcode ;
  uint8_t             byte[8] ;
} ;

typedef union uromcode romcode ;


void one_wire_reset (void) ;
uint8_t one_wire_read_presence_pulse (void) ;
#define one_wire_init()   PIN_OUTPUT (ONE_WIRE_DATA_PORT, ONE_WIRE_DATA_BIT) ;\
                          SETBIT (ONE_WIRE_DATA_PORT, ONE_WIRE_DATA_BIT)
uint8_t one_wire_read_bit (void) ;
void one_wire_write_bit (uint8_t a) ;

#endif /* _ONE_WIRE_H_ */

