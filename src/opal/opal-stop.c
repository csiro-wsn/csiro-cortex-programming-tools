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
 *  Assert reset line of Opal debug board.
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
    struct ftdi_device_list *devlist;
    int ret;

    if (!(argc==1 || argc==3))
    {
       fprintf(stderr, "Usage: opal-init [<bus> <device>]\n");
       return EXIT_FAILURE;
    }



    // Initialize FTDI layer
    if ((ftdi = ftdi_new()) == 0)
    {
        fprintf(stderr, "Initializing FTDI chip failed\n");
        return EXIT_FAILURE;
    }

    if (argc==1)
    {
        // get first matching device
        if ((ret = debug_get_devices(ftdi, &devlist))<=0)
        {
            fprintf(stderr, "Error: No opal debug board found.\n");
            return EXIT_FAILURE;
        }

    } else {

        // get device with matching bus and device identifier
        if ((ret = debug_get_device(ftdi, &devlist, argv[1], argv[2]))<=0)
        {
            fprintf(stderr, "Error: No opal debug board found.\n");
            return EXIT_FAILURE;
        }

    }


    // open device
    printf("Opal: Debug board found (Bus: %s Device: %s).\n", devlist->dev->bus->dirname, devlist->dev->filename);
    if (debug_open_device(ftdi, devlist)==EXIT_FAILURE)
    {
        ftdi_deinit(ftdi);
        return 0;
    }

    // assert reset pin    
    debug_set_pins(ftdi, SET_BITS_LOW, BIT_MASK_RESET_PIN, 0);

    debug_close_device(ftdi);

    printf("Opal: RESET line asserted.\n");

    return 0;
}
