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

struct one_wire_search_data
{
  uint8_t             last_discrepancy ;
  uint8_t             rom_bit_index ;
  uint8_t             discrepancy_marker ;
  romcode             rom ;
} ;


extern uint8_t        numbers_of_one_wire_devices ;

extern uint8_t        one_wire_flags ;
#define ONE_WIRE_SEARCH_DONE_FLAG       0
#define A_BIT                           1
#define B_BIT                           2
#define ONE_WIRE_DEVICES_DETECTED       3

/* ROM commands */
#define OW_READ_ROM                     0x33
#define OW_MATCH_ROM                    0x55
#define OW_SEARCH_ROM                   0xF0
#define OW_ALARM_SEARCH                 0xEC
#define OW_SKIP_ROM                     0xCC


void    one_wire_reset (void) ;
uint8_t one_wire_read_presence_pulse (void) ;
#define one_wire_init()   PIN_OUTPUT (ONE_WIRE_DATA_PORT, ONE_WIRE_DATA_BIT) ;\
                          SETBIT (ONE_WIRE_DATA_PORT, ONE_WIRE_DATA_BIT)
uint8_t one_wire_read_bit (void) ;
void    one_wire_write_bit (uint8_t a) ;
void    one_wire_copy_romcode (romcode * dest, romcode * src) ;
void    one_wire_send_command (uint8_t command) ;
#define one_wire_send_rom_command one_wire_send_command
void    search_one_wire_devices (romcode * list, uint8_t list_size) ;
uint8_t one_wire_search_first (struct one_wire_search_data * search_data) ;
uint8_t one_wire_search_next  (struct one_wire_search_data * search_data) ;

#endif /* _ONE_WIRE_H_ */

