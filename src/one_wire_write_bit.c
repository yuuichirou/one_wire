/*
 * one_wire_write_bit.c
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

#include "one_wire.h"
#include "macros.h"
#include <avr/io.h>
#include <util/delay.h>

void one_wire_write_bit (uint8_t a)
{
  PIN_OUTPUT (ONE_WIRE_DATA_PORT, ONE_WIRE_DATA_BIT) ;
  CLEARBIT (ONE_WIRE_DATA_PORT, ONE_WIRE_DATA_BIT) ;
  _delay_us (10) ;
  if (a)
  {
    SETBIT (ONE_WIRE_DATA_PORT, ONE_WIRE_DATA_BIT) ;
    _delay_us (50) ;
  }
  else
  {
    _delay_us (50) ;
    SETBIT (ONE_WIRE_DATA_PORT, ONE_WIRE_DATA_BIT) ;
  }
}

