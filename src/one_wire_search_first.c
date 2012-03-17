/*
 * one_wire_search_first.c
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

uint8_t one_wire_search_first (struct one_wire_search_data * search_data)
{
  search_data->last_discrepancy = 0 ;
  CLEARBIT (one_wire_flags, ONE_WIRE_SEARCH_DONE_FLAG) ;
  return one_wire_search_next (search_data) ;
}

