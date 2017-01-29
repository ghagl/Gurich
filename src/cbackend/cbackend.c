/*

				Gurich
	**	Ricoh SP110 series driver **

	Copyright (C) 2016, 2017 Gustaf Haglund <ghaglund@bahnhof.se>

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

/* Cups backend. */

#include <gurich.h>

static void tell_cups()
{
	printf("direct gurich://RICOH/SP 112 DDST?serial=X025M233243\n");
	/* TODO: A better way, this will not work currently */
}

int main(int argc, char ** argv)
{
	struct gurich_usb g;
	struct gurich_transferdata bin;
	g.initialized = false;

	check_printer_usb(&g);

	if (!check_printer_status(&g)) {
		cleanup_usb(&g);
		return -1;
	}

	if (argc == 1) {
		tell_cups();
		return 0;
	}

	char binbuf[8192];
	size_t readlen;
	int fp = 0;

	if (argc < 6) {
		fprintf(stderr, "ERROR: Not enough arguments\n");
	} else if (argc > 6) {
		fp = open(argv[6], O_RDONLY);
	}

	while ((readlen = read(fp, binbuf, 8192)) > 0) {
		data_modify(&bin, binbuf, readlen, 0);
	}

	if (fp != 0) close(fp);

	do_send_usb(&g, &bin);
	cleanup_usb(&g);

	return 0;
}
