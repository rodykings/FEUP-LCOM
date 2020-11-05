//
// Created by rodykings on 14/10/19.
//

#include <lcom/lcf.h>
#include "i8024.h"


extern uint32_t cnt;
bool e0 = false;
uint8_t scanCode;

void (kbc_ih)(){

    uint32_t data;

    int counter = 0;
    while( counter < 5 ) {
        uint32_t stat;
        #define DLAB3
        sys_inb(KBC_STAT_REG, &stat); /*assuming it returns OK*/
        #ifdef DLAB3
        cnt++;
        #endif
        /*loop while 8042 output buffer is empty*/
        if( stat & OBF ) {
            #define DLAB3
            sys_inb(OUT_BUF, &data); /*assuming it returns OK*/
            #ifdef DLAB3
            cnt++;
            #endif
            if ( (stat &(PAR_ERR | TO_ERR)) == 0 )
                scanCode = data;
            return;
        }
        counter++;
    }


}
void print_scanCode(){

    uint8_t bytes[4];

    if(scanCode == 0xE0){
        e0 = true;
    }
    else if(e0){
        e0 = false;
        bytes[1] = scanCode;
        bytes[0] = 0xE0;
        if(BIT(7) & scanCode){
            kbd_print_scancode(false, SCANCODE_SIZE+1, bytes);
        }
        else{
            kbd_print_scancode(true, SCANCODE_SIZE+1, bytes);
        }
    }
    else{
        bytes[0] = scanCode;
        if(BIT(7) & scanCode){
            kbd_print_scancode(false, SCANCODE_SIZE, bytes);
        }
        else {
            kbd_print_scancode(true, SCANCODE_SIZE, bytes);
        }
    }
}

void enable_kbd_interrupts(){

    uint8_t readCommand = 0x20;
    sys_outb(KBC_STAT_REG, readCommand);

    uint32_t readValue;
    #define DLAB3
    sys_inb(OUT_BUF, &readValue);
    #ifdef DLAB3
    cnt++;
    #endif

    readValue |= BIT(0);

    uint8_t writeCommand = 0x60;
    sys_outb(KBC_STAT_REG, writeCommand);

    sys_outb(OUT_BUF, readValue);
}


