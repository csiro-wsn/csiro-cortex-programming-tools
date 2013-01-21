/*
 * Copyright (c) 2013, CSIRO
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the
 *   distribution.
 * - Neither the name of the copyright holders nor the names of
 *   its contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*  opal.c
 *
 *  Access methods for the FTDI chip on the Opal debug board.
 *
 *  We are using MPSSE mode to control the direction and status of the GPIO pins on the FTDI chip.
 *  See FTDI "Application Note AN_108 Command Processor for MPSSE and MCU Host Bus Emulation Modes" for details.
 *  The low byte would be ADBUS 7-0, the high byte would be ACBUS7-0 (not used). This implementation uses libftdi.
 *
 *  Author: Philipp Sommer <philipp.sommer@csiro.au>
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ftdi.h>
#include <usb.h>
#include "opal.h"



int debug_get_devices(struct ftdi_context *ftdi, struct ftdi_device_list **devlist)
{
    // get all matching devices
    return ftdi_usb_find_all(ftdi, devlist, OPAL_FTDI_VENDOR_ID, OPAL_FTDI_PRODUCT_ID);
}


int debug_get_device(struct ftdi_context *ftdi, struct ftdi_device_list **devlist, int bus, int device)
{

    // get all devices and look for matching bus and device identifier
    if (ftdi_usb_find_all(ftdi, devlist, OPAL_FTDI_VENDOR_ID, OPAL_FTDI_PRODUCT_ID)>0)
    {
        struct ftdi_device_list *curdev;
        curdev = *devlist;

        while (curdev != NULL) {

            if ((atoi(curdev->dev->bus->dirname) == bus) && (atoi(curdev->dev->filename) == device))
            {
                devlist = &curdev;
                return 1;
            }
            curdev = curdev->next;
        }
    }

    return 0;

}


int debug_open_device(struct ftdi_context *ftdi, struct ftdi_device_list *devlist)
{
    int ret;

    // select channel
    if ((ret = ftdi_set_interface(ftdi, OPAL_INTERFACE))!=0)
    {
        fprintf(stderr, "Error: Unable to set ftdi interface: %d (%s)\n", ret, ftdi_get_error_string(ftdi));
        return EXIT_FAILURE;
    }

    // open device
    ret = ftdi_usb_open_dev(ftdi, devlist->dev);
    if (ret < 0 && ret != -5)
    {
        fprintf(stderr, "Error: Unable to open ftdi device: %d (%s)\n", ret, ftdi_get_error_string(ftdi));
        return EXIT_FAILURE;
    }

    // enable MPSSE mode
    if ((ret = ftdi_set_bitmode(ftdi, 0, BITMODE_MPSSE))!=0)
    {
        fprintf(stderr, "Error: Unable to set mpsse mode: %d (%s)\n", ret, ftdi_get_error_string(ftdi));
        return EXIT_FAILURE;
    }

    return 0;
}

int debug_close_device(struct ftdi_context *ftdi)
{
    ftdi_usb_close(ftdi);
    ftdi_free(ftdi);
}



int debug_set_pins(struct ftdi_context *ftdi, int address, int direction, int value)
{
    unsigned char buf[3];
    buf[0] = address;
    buf[1] = value;
    buf[2] = direction; // set output direction
    return ftdi_write_data(ftdi, buf, 3);
}

