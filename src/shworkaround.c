/*

				Gurich
	**	Ricoh SP110 series driver **

	Copyright (C) 2016, 2017 Gustaf Haglund <kontakt@ghaglund.se>

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

void gurich_workaround_pbmgen(
	struct gurich_usb * g, const char * res,
	char * papertype, const char * psfile)
{
	char gs_cmd[250];

	if (strcmp(res, "1200") == 0) {
		res = "1200x600";
	}

	for (size_t i = 0; papertype[i] != '\0'; i++){
		papertype[i] = tolower(papertype[i]);
	}

	snprintf(
		gs_cmd,
		250,
		"/usr/bin/gs -sDEVICE=pbmraw -sOutputFile=%s%%03d-page.pbm "
		"-r%s -dQUIET -dBATCH -dNOPAUSE -sPAPERSIZE=%s %s",
		GURICH_TEMP_DIR,
		res,
		papertype,
		psfile
	);

	//syslog (LOG_NOTICE, gs_cmd);

	#ifdef _DEBUG
	fprintf(stderr, "DEBUG: gs_cmd %s\n", gs_cmd);
	#endif

	system(gs_cmd);
}
