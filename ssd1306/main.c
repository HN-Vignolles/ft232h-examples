/* HN-Vignolles */
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <ftdi.h>
#include <string.h>
#include "src/mpsse.h"
#include "ssd1306_ftdi.h"
#include "font5x7.h"

#define FTDI 0x0403
#define FT232H 0x6014

struct mpsse_context *mpsse;
int status = 1;

void sigh(int sig){
    printf("Signal>>> %d\n",sig);
    status = 0;
    Close(mpsse);
    exit(0);
}

int main(int argc, char **argv){
    char sstr[128];
    //while((c = getopt(argc,argv,"abc:")) != -1)
    //  switch(c){

    /*
     *      UM232H Pin Out, MPSSE configuration
     *               ┌─────╥─────╥─────┐
     *               │     ║ USB ║     │
     *               │     ║     ║     │
     *           J2-1│     ╚═════╝     │J1-1
     *            ───┤ SLD         GND ├─P─
     *           ┌─P─┤ VIO         5V0 ├─◄─┐
     *           └─◄►┤ 3V3         USB ├─►─┘
     *            ─C─┤ PU2        RST# ├─◄─
     * SSD1306    ─C─┤ PU1         AC9 ├◄►─ **
     *  ───┐      ─P─┤ GND         AC8 ├◄►─ **
     *  D0 ├── SCK─◄─┤ AD0         AC7 ├◄►─ ***
     *  D1 ├── DO ─◄─┤ AD1         AC6 ├◄►─ GPIOH6
     *         DI ─►─┤ AD2         AC5 ├◄►─   ·
     *         CS*─►─┤ AD3         AC4 ├◄►─   ·      SSD1306
     *      GPIOL0───┤ AD4         AC3 ├◄►─   ·       ┌────
     *      GPIOL1───┤ AD5         AC2 ├◄►─ GPIOH2 ───┤ CS
     *      GPIOL2───┤ AD6         AC1 ├◄►─ GPIOH1 ───┤ RST
     *      GPIOL3───┤ AD7         AC0 ├◄►─ GPIOH0 ───┤ DC
     *          J2-14└─────────────────┘J1-14
     */
    signal(SIGINT,sigh);
    signal(SIGTERM,sigh);
    if((mpsse = MPSSE(SPI3,TWELVE_MHZ,MSB)) == NULL){
        fprintf(stderr,"error: MPSSE\n");
        return EXIT_FAILURE;
    }
    printf("Allocated mpsse structure\n");
    if(mpsse->open){
        Start(mpsse);
        SSD1306_Init();
        SSD1306_Orientation(LCD_ORIENT_NORMAL);
        SSD1306_Contrast(127);    
        LCD_PixelMode = LCD_PSET;//FIXME
        SSD1306_Fill(0x00);
        sprintf((char *)sstr,"Hello World");
        LCD_PutStr(19,43,sstr,fnt5x7);
        SSD1306_Flush();
        sleep(5);
        while(status){
            SSD1306_Fill(0x00);
            for(int i=0;i<128;i++){
                for(int j=0;j<64;j++){
                    int k = rand();
                    LCD_Pixel(i,j,((k > RAND_MAX/2)?LCD_PSET:LCD_PRES));
                }
            }
            SSD1306_Flush();
        }
    } else {
        fprintf(stderr,"error: mpsse->open\n");
        return EXIT_FAILURE;
    }
    Close(mpsse);
    printf("OK\n");
    return EXIT_SUCCESS;
}
