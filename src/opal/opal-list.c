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

/*  opal-stop.c
 *
 *  Toggle reset line of Opal debug board.
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
#include "opal.h"


int main(int argc, char **argv)
{
    struct ftdi_context *ftdi;
    struct ftdi_device_list *devlist, *curdev;

    int ret;


    // Initialize FTDI library
    if ((ftdi = ftdi_new()) == 0)
    {
        fprintf(stderr, "Initializing FTDI library failed\n");
        return EXIT_FAILURE;
    }

    // get devices
    if ((ret = debug_get_devices(ftdi, &devlist))<=0)
    {
        fprintf(stderr, "Error: No opal debug board found.\n");
        return EXIT_FAILURE;
    }

    printf("%u Opal debug board(s) found:\n", ret);

    curdev = devlist;

    while (curdev != NULL) {
        printf("\tBus: %s\tDevice: %s\n", curdev->dev->bus->dirname, curdev->dev->filename);
        curdev = curdev->next;
    }
 

    ftdi_list_free(&devlist);	
    ftdi_free(ftdi);

    return 0;
}
