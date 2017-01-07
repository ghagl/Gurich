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

#include <gurich.h>

static void init_msg()
{
	unsigned short int i;

	puts("\t* Gurich - printer driver *\n");
	puts("Copyright (c) 2016, 2017 Gustaf Haglund");
	puts(
		"\n"
		"License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\n"
		"This is free software: you are free to change and redistribute it.\nThere is NO WARRANTY, to the extent permitted by law.\n"
	);

	for (i = 0; i < 45; ++i) {
		printf("_");
	}

	puts("\n");
}

static void display_usage(const char * exec, bool cpr)
{
	printf("print: %s -p [ps/pdf file, resolution (600|1200), copies, paper type (A4 is tested)]\n", exec);
	printf("status: %s -s\n", exec);
	printf("testpage: %s -t\n", exec);
	printf("cups filter: %s -c [filter arguments] (wrapper for -p, does the same job)\n", exec);

	if (cpr) {
		puts("\nCopyright (c) 2016, 2017 Gustaf Haglund\n");
	}
}

static void display_status(struct gurich_usb * g)
{
	struct gurich_status data;
	unsigned int ink;
	unsigned int pr;
	char inkstr[75];

	data = gurich_status(g);
	ink = gurich_toner(g);
	pr = gurich_printed(g);

	snprintf(
		inkstr,
		75,
		"Printer toner information: %d %% left %s",
		ink,
		(ink <= 10 ? "- please consider buying more toner" : "")
	);

	printf("Printer state: %s, reference: 0x%hx\n", data.status, data.ref);
	puts(inkstr);
	printf("Printed out pages (stats): %d\n", pr);
}

static void nofilter_print(struct gurich_usb * g, int argc, char ** argv)
{
	if (argc < 6) {
		fprintf(stderr, "Not enough arguments. Quitting.\n");
		return;
	}

	if (!gurich_dir_checkup()) {
		fprintf(stderr, "This driver stumbled over an unknown directory management error. Quitting.\n");
		return;
	}

	gurich_prnt(g, get_username(), argv[3], argv[2], argv[4], argv[5], false);
}

static void cups_filter_print(struct gurich_usb * g, char ** argv)
{
	int psf;
	char * ps;
	char * res;
	FILE * psfp;
	size_t psbeg;
	size_t pslen;
	/*char * title;*/
	size_t psread;
	char * copies;
	char * username;
	char * papertype;
	char psbuf[8192];
	char psfn[BUFSIZ];
	/*char * additional;*/

	psbeg = 0;
	pslen = 0;
	ps = malloc(8192);
	gurich_alloc_set(ps);

	psf = cupsTempFd(psfn, BUFSIZ);
	psfp = fdopen(psf, "wb+");

	username = argv[2];
	/*title = argv[3];*/
	copies = argv[4];
	/*additional = argv[5];*/
	res = "600";
	papertype = "A4";

	/*
	 * TODO:
	 * -Full CUPS filter support with user-supplied options
	 *
	 */

	while ((psread = read(0, psbuf, 8192)) > 0)
	{
		ps = realloc(ps, pslen += psread);
		for (unsigned short int r = 0; r < psread; ++r, ++psbeg) {
			ps[psbeg] = psbuf[r];
		}
	}

	fwrite(ps, pslen, 1, psfp);
	fclose(psfp);

	free(ps);

	if (!gurich_dir_checkup()) {
		fprintf(
			stderr,
			"ERROR: Quitting, since this driver stumbled over an unknown directory management error.\n"
		);
		return;
	}

	gurich_prnt(g, username, res, psfn, copies, papertype, true);
}

int main(int argc, char ** argv)
{
	const char *arg;
	struct gurich_usb g;
	g.initialized = false;

	if (argc < 2) {
		display_usage(argv[0], true);
		return 0;
	}

	arg = argv[1];

	if (arg[0] == '-')
	{
		init_msg();
		check_printer_usb(&g);

		switch(arg[1])
		{
			case 'p':
				nofilter_print(&g, argc, argv);
				break;
			case 's':
				display_status(&g);
				break;
			case 't':
				gurich_testpage(&g);
				break;
			case 'h':
				display_usage(argv[0], false);
				break;
			default:
				display_usage(argv[0], false);
		}

		puts("");
	} else {
		cups_filter_print(&g, argv);
	}

	cleanup_usb(&g);
	return 0;
}
