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

void printer_usb(struct gurich_usb * g, uint64_t idProduct)
{
	char libusb_strerror[128];
	int libusb_error;

	libusb_set_debug(g->ctx, PRINTER_LIBUSB_DEBUG);

	g->device_handle = libusb_open_device_with_vid_pid(g->ctx, PRINTER_VENDOR_ID, idProduct);

	if (g->device_handle == NULL)
	{
		libusb_close(g->device_handle);
		libusb_exit(g->ctx);
		fprintf(stderr, "Something did happen with the printer. Quitting.\n\n");
		exit(-1);
	}

	g->device = libusb_get_device(g->device_handle);

	libusb_error = libusb_kernel_driver_active(g->device_handle, 0);
	if (libusb_error == 1) {
		if ((libusb_error = libusb_detach_kernel_driver(g->device_handle, 0)) < 0) {
			goto libusb_fail;
		}
	}

	libusb_error = libusb_claim_interface(g->device_handle, 0);
	if (libusb_error != 0) {
		goto libusb_fail;
	}

	g->initialized = true;
	return;

	libusb_fail:
		snprintf(libusb_strerror, 128, "%s %s", "ERROR: ", libusb_error_name(libusb_error));

		/*
		libusb_release_interface(g->device_handle, GURICH_USB_INTERFACE);
		libusb_attach_kernel_driver(g->device_handle, GURICH_USB_INTERFACE);
		libusb_close(g->device_handle);
		*/

		fprintf(stderr, "Something went wrong with libusb (%s). Quitting.\n\n", libusb_strerror);
		exit(-1);
}

void check_printer_usb(struct gurich_usb * g)
{
	libusb_device **list;
	struct libusb_device_descriptor devdesc;
	ssize_t numusbdevs, i, err;

	err = libusb_init(&g->ctx);
	numusbdevs = libusb_get_device_list(g->ctx, &list);

	for (i = 0; i < numusbdevs; ++i)
	{
		g->device = list[i];

		if (libusb_get_device_descriptor(g->device, &devdesc) < 0)
			continue;

		if (!devdesc.bNumConfigurations || !devdesc.idVendor
			|| !devdesc.idProduct)
			continue;

		if (devdesc.idVendor == PRINTER_VENDOR_ID) {
			/* Found a Ricoh printer. */
			printer_usb(g, devdesc.idProduct);
		}
	}

	if (numusbdevs >= 0) {
		libusb_free_device_list(list, 1);
	}
}

void cleanup_usb(struct gurich_usb * g)
{
	if (g->initialized)
	{
		libusb_release_interface(g->device_handle, 0);
		libusb_attach_kernel_driver(g->device_handle, 0);
		libusb_close(g->device_handle);

		libusb_exit(g->ctx);
	}
}
