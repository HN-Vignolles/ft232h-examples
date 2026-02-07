#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "max7456.h"
#include "mpsse.h"
#include "support.h"

/* Based on https://github.com/DFRobot/DFRobot_OSD */
/* Ported by HN-Vignolles and Copilot */

extern struct mpsse_context *mpsse;

void writeAddrData(uint8_t addr, uint8_t data){
    unsigned char tmp[2];
    int buf_len = 0;
    unsigned char *buf = NULL;

    tmp[0] = addr;
    tmp[1] = data;

    /* Build proper MPSSE command buffer (wrap with mpsse->tx opcode + length fields) */
    buf = build_block_buffer(mpsse, mpsse->tx, tmp, 2, &buf_len);
    if (!buf) {
        fprintf(stderr, "build_block_buffer failed\n");
        return;
    }

    if (raw_write(mpsse, buf, buf_len) != MPSSE_OK) {
        perror("mpsse raw_write");
    }

    free(buf);
}

void writeData(uint8_t data){
    unsigned char tmp = data;
    int buf_len = 0;
    unsigned char *buf = build_block_buffer(mpsse, mpsse->tx, &tmp, 1, &buf_len);

    if (!buf) {
        fprintf(stderr, "build_block_buffer failed\n");
        return;
    }

    if (raw_write(mpsse, buf, buf_len) != MPSSE_OK) {
        perror("mpsse raw_write");
    }

    free(buf);
}

void displayChar(uint8_t y, uint8_t x, uint16_t addr, uint8_t attr){ 
    uint16_t ca; //character address
    uint8_t addrH; 

    ca = y * 30 + x; //30 characters per row
    addrH = ca >> 8;

    /* Assert CS for the whole multi-write transaction */
    PinLow(mpsse, OSD7456_CS);

    /*Read factory preset before write (p36)... writeAddrData(OSDBL,0X00); */
    writeAddrData(DMM, 0x40); //8-bit operation mode
    writeAddrData(DMAH, addrH | 0x2); //attribute write flag
    writeAddrData(DMAL, ca); //lower 8 bits of address
    writeAddrData(DMDI, attr);//Write the Character Attribute byte
    
    writeAddrData(DMAH, addrH); 
    writeAddrData(DMAL, ca); 
    writeAddrData(DMDI, addr);
    writeAddrData(VM0, 0x48);

    /* Deassert CS when done */
    PinHigh(mpsse, OSD7456_CS);
}

void displayString(uint8_t y, uint8_t x, const char *s, uint8_t attr){
    uint8_t addrH, c; 
    uint16_t ca, value; //character address
    c = *s++; 
    ca = y * 30 + x;

    /* Assert CS for the whole string write */
    PinLow(mpsse, OSD7456_CS);

    writeAddrData(OSDBL,0x00);
    while (c != 0){
        if ((c >= '0') && (c <='9'))
            value = ((c == '0')? 10 : c - '1' + 1);
        else if ((c >= 'A') && (c <= 'Z'))
            value = (c - 'A' + 11);
        else if ((c >= 'a') && (c <= 'z'))
            value = (c - 'a' + 37);
        else
            value = (0x00);
    
        addrH = ca >> 8; 
        writeAddrData(DMM, 0x40); 
        writeAddrData(DMAH, addrH | 0x2); 
        writeAddrData(DMAL, ca); 
        writeAddrData(DMDI, attr);
        
        writeAddrData(DMAH, addrH); 
        writeAddrData(DMAL, ca); 
        writeAddrData(DMDI, value);
        c = *s++;
        ca = ca+1;
    }
    writeAddrData(VM0, 0x48);

    /* Deassert CS */
    PinHigh(mpsse, OSD7456_CS);
}

void clearOSD(void){
    /* Assert CS for the operation */
    PinLow(mpsse, OSD7456_CS);
    writeAddrData(DMM, 0x01);
    PinHigh(mpsse, OSD7456_CS);
}
