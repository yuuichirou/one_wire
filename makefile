#
#  makefile
#  This file is part of the set of functions to handle Dallas "One Wire"
#  protocol and devices.
#
#  Copyright (C) 2012 Krzysztof Kozik
#
#  This set is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330,
#  Boston, MA 02111-1307, USA.
#

TARGET          = libone_wire

#------------------------- Sources --------------------------------------------
sources         = src/one_wire_reset.c\
                  src/one_wire_read_presence_pulse.c\
                  src/one_wire_read_bit.c\
                  src/one_wire_write_bit.c\
                  src/one_wire_copy_romcode.c

# put object files in place where are source files
objects = $(subst .c,.o,$(sources))

#------------------------- C Preprocessor options -----------------------------
ifdef F_CPU
_F_CPU          = -DF_CPU=$(F_CPU)UL
endif
CPPFLAGS        = $(_F_CPU) -std=gnu89 -Wextra #-pedantic
CPPFLAGS       += -Wall -Wunused-macros -Wsystem-headers

#------------------------- C Compiler options ---------------------------------
ifdef OPT
_OPT            = -O$(OPT)
endif
ifdef TARGET_MCU
_TARGET_MCU     = -mmcu=$(TARGET_MCU)
endif
CFLAGS          = -gdwarf-2 $(_OPT) $(_TARGET_MCU) $(CPPFLAGS)
CFLAGS         += -funsigned-char -funsigned-bitfields
CFLAGS         += -fshort-enums -fpack-struct -mshort-calls
CFLAGS         += -Wall -Wundef -Wsign-compare -Wstrict-prototypes
CFLAGS         += -Wmissing-parameter-type -Wmissing-prototypes
CFLAGS         += -Wmissing-declarations -Winline -Wunreachable-code

#------------------------- Archiver options -----------------------------------
ARFLAGS         = -crsv

#------------------------- Programs -------------------------------------------
SHELL           = /bin/sh
CC              = avr-gcc
LD              = avr-ld
OBJCOPY         = avr-objcopy
SIZE            = avr-size
AR              = avr-ar

#------------------------- Rules ----------------------------------------------

# do not show commands
.SILENT:

.PHONY: no_target
no_target:
	echo "Usage: make <target> <definitons>\nfor more info type make help"


HELP_ECHO = "Usage: make <target> <definitons>\
\ntarget:\
\n  help                     Show this screen\
\n  compile                  Compile source files\
\n  lib                      Create library\
\n  clean                    Delete all files created by building project\
\n  cleanall                 Delete all files created by building project,\
\n                           dependency and debug files\
\n  toolsversion             Show tools version numbers\
\n  savetoolsversion         Dump tools version numbers to file 'toolsversion'\
\n  debug                    Create debug files by objdump, nm, size, readelf\
\n  cleandebug               Delete all files created by debugging rule\
\ndefinitions:\
\n  TARGET_MCU=[]\
\n  F_CPU=[]\
\n  OPT=[]                   0,1,2,3,s\
\n"

.PHONY: help
help:
	echo $(HELP_ECHO)


.PHONY: compile
compile: $(objects)

%.o: %.c makefile
	echo "compiling a file: $@"
	$(CC) -c $(CFLAGS) $< -o $@


.PHONY: lib
lib: $(TARGET).a
	-ln -s src/$(subst lib,,$(TARGET)).h $(subst lib,,$(TARGET)).h
	-ln -s src/config.h config.h

$(TARGET).a: $(objects)
	echo "creating library: $@"
	$(AR) $(ARFLAGS) $@ $^


ifeq ($(filter-out no_target help %toolsversion clean%,$(MAKECMDGOALS)),)
else
-include $(subst .c,.d,$(sources))
endif

%.d: %.c
	echo "generating dependencies for file: $<"
	set -e; rm -f $@;\
	$(CC) -MM $(CFLAGS) $< -MF $@.$$$$;\
	sed 's|$(notdir $*).o|$*.o $*.d|' <$@.$$$$ >$@;\
	rm -f $@.$$$$


.PHONY: toolsversion
toolsversion:
	echo ------------------------------
	$(CC) --version
	echo ------------------------------
	$(LD) --version
	echo ------------------------------
	$(OBJCOPY) --version
	echo ------------------------------
	$(AR) -V
	echo ------------------------------
	LANG=EN make --version
	echo ------------------------------
	echo binutils-avr package `dpkg -p binutils-avr | grep Version`
	echo ------------------------------

.PHONY: savetoolsversion
savetoolsversion:
	rm -f toolsversion
	$(CC) --version         | head -n 1     >> toolsversion
	$(LD) --version         | head -n 1     >> toolsversion
	$(OBJCOPY) --version    | head -n 1     >> toolsversion
	$(AR) -V                | head -n 1     >> toolsversion
	LANG=EN make --version  | head -n 1     >> toolsversion
	echo binutils-avr package `dpkg -p binutils-avr | grep Version`\
                                                >> toolsversion


.PHONY: debug
debug: $(addsuffix .debug, $(objects) $(TARGET).a)

%.debug: % makefile
	echo "creating debug files: $@.*"
#	avr-objdump -W -w -z    $< > $@.objdump_W
#	avr-objdump -t -w -z    $< > $@.objdump_t
	avr-objdump -d -w -z    $< > $@.objdump_d
	avr-objdump -S -w -z    $< > $@.objdump_S
#	avr-objdump -s -w -z    $< > $@.objdump_s
#	avr-objdump -h -w -z    $< > $@.objdump_h
	avr-nm -a -f sysv -l    $< > $@.nm_a_fsysv
#	avr-nm -a -S -l         $< > $@.nm_a_S
	avr-size -A --common    $< > $@.size_A
#	avr-size -B --common -t $< > $@.size_B_t
#	avr-readelf -w -W -a    $< > $@.readelf


.PHONY: clean
clean:
	echo "removing files needed to complete project"
	rm -rf src/*.o
	rm -f lib*
	rm -f $(subst lib,,$(TARGET)).h config.h


.PHONY: cleandep
cleandep:
	echo "removing dependency files"
	rm -f $(subst .c,.d,$(sources))


directories = $(dir $(objects) $(TARGET).a)

.PHONY: cleandebug
cleandebug:
	echo "removing all files created by debugging rule"
	$(foreach directory,$(directories),rm -f $(directory)*.debug.*;)


.PHONY: cleanall
cleanall: cleanall_echo clean cleandebug cleandep

.PHONY: cleanall_echo
cleanall_echo:
	echo "removing all files that can be regenerated by make"


ifeq ($(filter-out no_target help %toolsversion clean%,$(MAKECMDGOALS)),)
else
  $(if $(F_CPU)     ,,$(warning not specified F_CPU))
  $(if $(OPT)       ,,$(warning not specified OPT))
  $(if $(TARGET_MCU),,$(warning not specified TARGET_MCU))
  $(if $(and $(F_CPU),$(OPT),$(TARGET_MCU)),,$(error error: need <definitons>))
endif

