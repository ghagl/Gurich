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

struct gurich_status gurich_status(struct gurich_usb * g)
{
	struct gurich_status stat;
	unsigned char statusbuf[1024];

	libusb_control_transfer(g->device_handle, 0xc1, 9, 0x00, 0, statusbuf, 1024, 5000);

	stat.state = 0;
	stat.ref = statusbuf[10];

	/* stat.state is currently unused. */

	switch (statusbuf[10])
	{
		case 0x35:
			stat.status = "BAD";
			stat.state = 1;
			break;
		case 0x30:
			stat.status = "ENERGY SAVING MODE 1";
			break;
		case 0x31:
			stat.status = "GOOD / ENERGY SAVING MODE 2 / IDLE";
			break;
		case 0x37:
			stat.status = "PREPARING / RELAXING (UNKNOWN?)";
			break;
		case 0x33:
			stat.status = "PRINTING / WARMING UP";
			break;
		case 0x32:
			stat.status = "GOING BACK TO IDLE (UNKNOWN?)";
			break;
		default:
			stat.status = "UNKNOWN";
	}

	return stat;
}

unsigned short int gurich_toner(struct gurich_usb * g)
{
	unsigned char tonerbuf[1024];
	libusb_control_transfer(g->device_handle, 0xc1, 149, 0x00, 0, tonerbuf, 1024, 5000);
	return tonerbuf[6] * 10;
}

size_t gurich_printed(struct gurich_usb * g)
{
	unsigned char prbuf[1024];
	libusb_control_transfer (g->device_handle, 0xc1, 193, 0x00, 0, prbuf, 1024 /* formely 6 */, 5000);

	if (prbuf[25] == 0x00) {
		return prbuf[24] + 15;
	} else if (prbuf[1] == 0x00 && prbuf[61] == 0x00) {
		return prbuf[0];
	} else {
		return (prbuf[0] + prbuf[24])/2 + 264;
	}
	/* prbuf[56] -> failures */
}

void gurich_testpage(struct gurich_usb * g)
{
	unsigned char testpagebuf[8];
	libusb_control_transfer(g->device_handle, 0x41, 162, 0x0000, 0, testpagebuf, 8, 5000);
}
