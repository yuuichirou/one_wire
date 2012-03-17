/*
 * one_wire_search_next.c
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

uint8_t one_wire_search_next (struct one_wire_search_data * search_data)
{
  uint8_t return_value ;
  uint8_t index = 0, bit = 0 ;

  return_value = FALSE ;
  if (BITSET (one_wire_flags, ONE_WIRE_SEARCH_DONE_FLAG))
  {
    CLEARBIT (one_wire_flags, ONE_WIRE_SEARCH_DONE_FLAG) ;
    return return_value ;
  }
  one_wire_reset () ;
  if (!one_wire_read_presence_pulse())
  {
    search_data->last_discrepancy = 0 ;
    return return_value ;
  }
  search_data->rom_bit_index = 1 ;
  search_data->discrepancy_marker = 0 ;
  one_wire_send_rom_command (OW_SEARCH_ROM) ;

  do
  {
    one_wire_read_bit () ?   SETBIT (one_wire_flags, A_BIT) :
                           CLEARBIT (one_wire_flags, A_BIT) ;
    one_wire_read_bit () ?   SETBIT (one_wire_flags, B_BIT) :
                           CLEARBIT (one_wire_flags, B_BIT) ;
    if (BITSET (one_wire_flags, A_BIT) && BITSET (one_wire_flags, B_BIT))
    {
      search_data->last_discrepancy = 0 ;
      return return_value ;
    }
    index = (uint8_t)(search_data->rom_bit_index-1) >> 3 ;
    bit   = (uint8_t)(search_data->rom_bit_index-1) & 0x07 ;
    if (BITCLEAR (one_wire_flags, A_BIT) && BITCLEAR (one_wire_flags, B_BIT))
    {
      if (search_data->rom_bit_index == search_data->last_discrepancy)
      {
        SETBIT (search_data->rom.byte[index], bit) ;
      }
      else
      {
        if (search_data->rom_bit_index > search_data->last_discrepancy)
        {
          CLEARBIT (search_data->rom.byte[index], bit) ;
          search_data->discrepancy_marker = search_data->rom_bit_index ;
        }
        else
        {
          if (BITCLEAR(search_data->rom.byte[index], bit))
            search_data->discrepancy_marker = search_data->rom_bit_index ;
        }
      }
    }
    else
    {
      if (BITSET(one_wire_flags, A_BIT))
        SETBIT (search_data->rom.byte[index], bit) ;
      else
        CLEARBIT (search_data->rom.byte[index], bit) ;
    }
    one_wire_write_bit (BITSET (search_data->rom.byte[index], bit)) ;
    search_data->rom_bit_index++ ;
  }
  while (search_data->rom_bit_index <= 64) ;

  search_data->last_discrepancy = search_data->discrepancy_marker ;
  if (search_data->last_discrepancy == 0)
    SETBIT (one_wire_flags, ONE_WIRE_SEARCH_DONE_FLAG) ;
  return_value = TRUE ;
  return return_value ;
}

