#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <ftdi.h>
#include <getopt.h>
#include <signal.h>
#include "mpsse.h"
#include "max7456.h"

#define FTDI 0x0403
#define FT232H 0x6014

/*
 *            UM232H Pin Out, MPSSE configuration
 *                     ┌─────╥─────╥─────┐
 *                     │     ║ USB ║     │
 *                     │     ║     ║     │
 *    AT7456E      J2-1│     ╚═════╝     │J1-1
 *    (Module)      ───┤ SLD         GND ├─P─
 *        ───┐     ┌─P─┤ VIO         5V0 ├─◄─┐
 * P3_17 3V3 ├─────┴─◄►┤ 3V3         USB ├─►─┘
 *           │      ─C─┤ PU2        RST# ├─◄─
 *           │      ─C─┤ PU1         AC9 ├◄►─ **
 * P3_15 GND ├───────P─┤ GND         AC8 ├◄►─ **
 * P4_8  SCK ├── SCK─◄─┤ AD0         AC7 ├◄►─ ***
 * P4_9 MOSI ├── DO ─◄─┤ AD1         AC6 ├◄►─ GPIOH6
 *               DI ─►─┤ AD2         AC5 ├◄►─   ·
 *               CS*─►─┤ AD3         AC4 ├◄►─   ·      AT7456E
 *            GPIOL0───┤ AD4         AC3 ├◄►─   ·       ┌────
 *            GPIOL1───┤ AD5         AC2 ├◄►─ GPIOH2 ───┤ CS P3_3
 *            GPIOL2───┤ AD6         AC1 ├◄►─
 *            GPIOL3───┤ AD7         AC0 ├◄►─
 *                J2-14└─────────────────┘J1-14
 */

struct mpsse_context *mpsse;
int status = 1;

void sigh(int sig){
    printf("Signal>>> %d\n",sig);
    status = 0;
    Close(mpsse);
    exit(0);
}

int main(int argc, char **argv){
    signal(SIGINT, sigh);
    signal(SIGTERM, sigh);
    if((mpsse = MPSSE(SPI3, FIVE_MHZ, MSB)) == NULL){
        fprintf(stderr, "error: MPSSE\n");
        return EXIT_FAILURE;
    }
    printf("Allocated mpsse structure\n");
    if(mpsse->open){
        Start(mpsse);
        PinLow(mpsse, OSD7456_CS);
        displayChar(1, 1, 0xFC, OSD_LBC);
        displayString(2, 2, "Hello World!", OSD_LBC);
        //clearOSD();
    } else {
        fprintf(stderr, "error: mpsse->open\n");
        return EXIT_FAILURE;
    }
    Close(mpsse);
    printf("OK\n");
    return EXIT_SUCCESS;
}
