#include <lcom/lcf.h>
#include "kbd.h"

static uint8_t scanCode;

int get_scancode() {return scanCode;}

static int kbd_hook_id;

void kbd_subscribe_int(){
    kbd_hook_id = 1;
    sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE, &kbd_hook_id);
}
void kbd_unsubscribe_int(){
    sys_irqrmpolicy(&kbd_hook_id);
}

void (kbc_ih)(){

    uint32_t data;
    int counter = 0;

    while( counter < 5 ) {
        uint32_t stat;
        sys_inb(STAT_REG, &stat); /*assuming it returns OK*/

        /*loop while 8042 output buffer is empty*/

        if( stat & OBF ) {

            sys_inb(OUT_BUF, &data); /*assuming it returns OK*/

            if ( (stat &(PAR_ERR | TO_ERR)) == 0 )
                scanCode = data;
            return;
        }
        counter++;
    }
}

void enable_kbd_interrupts(){

    uint8_t readCommand = 0x20;
    sys_outb(STAT_REG, readCommand);

    uint32_t readValue;
    sys_inb(OUT_BUF, &readValue);
    readValue |= BIT(0);

    uint8_t writeCommand = 0x60;
    sys_outb(STAT_REG, writeCommand);

    sys_outb(OUT_BUF, readValue);

}

