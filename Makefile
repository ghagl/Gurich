##
##				Gurich
##	**	Ricoh SP110 series driver **
##
##	Copyright (C) 2016, 2017 Gustaf Haglund <ghaglund@bahnhof.se>
##
##	This program is free software: you can redistribute it and/or modify
##	it under the terms of the GNU General Public License as published by
##	the Free Software Foundation, either version 3 of the License, or
##	(at your option) any later version.
##
##	This program is distributed in the hope that it will be useful,
##	but WITHOUT ANY WARRANTY; without even the implied warranty of
##	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##	GNU General Public License for more details.
##
##	You should have received a copy of the GNU General Public License
##	along with this program.  If not, see <http://www.gnu.org/licenses/>.
##

DEBUG = -g3 -ggdb -fvar-tracking -fvar-tracking-assignments
CC = gcc -Wall -pedantic -std=gnu99 #$(DEBUG)
LIBS = $(shell pkg-config --cflags --libs libusb-1.0) -ljbig -lcups

BIN = bin/gurich
BIN_CBACKEND = bin/gurich_cbackend

CFLAGS = -Isrc $(LIBS) -D_DEBUG #-D_NO_PRINT_USB

CBACKEND_DEPEND = src/prntother.o src/basic.o src/prntcommon.o

all: $(patsubst src/%.c, src/%.o,$(wildcard src/*.c)) $(patsubst src/cbackend/%.c, src/cbackend/%.o,$(wildcard src/cbackend/*.c))
	$(CC) $(CFLAGS) -o $(BIN) src/*.o  $(LIBS)
	$(CC) $(CFLAGS) -o $(BIN_CBACKEND) $(CBACKEND_DEPEND) src/cbackend/*.o $(LIBS)

dev:
	rm src/*.o
	rm src/cbackend/*.o

cups: dev all
	sudo cp $(BIN) /usr/lib/cups/filter/
	sudo cp $(BIN_CBACKEND) /usr/lib/cups/backend/

clean:
	rm src/*.o
	rm src/cbackend/*.o
	rm $(BIN)
	rm $(BIN_CBACKEND)
