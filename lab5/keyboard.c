//
// Created by rodykings on 14/10/19.
//

#include <lcom/lcf.h>
#include <lcom/lab5.h>
#include "i8024.h"

int hook_id = 1;
uint8_t bit_no=0;
uint32_t cnt = 0;
int counter = 0;
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

void (waitForEsq)(){
        sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE, &hook_id);
 
      while(scanCode != 0x81) {

            uint8_t r;

            int ipc_status;

            message msg;

          if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {

                printf("driver_receive failed with: %d", r);

                continue;

            }

            if (is_ipc_notify(ipc_status)) { /* received notification */

                switch (_ENDPOINT_P(msg.m_source)) {

                    case HARDWARE: /* hardware interrupt notification */

                        if (msg.m_notify.interrupts & BIT(1)) { /* subscribed interrupt */

                            kbc_ih();

                        }

                        break;

                    default:

                        break; /* no other notifications expected: do nothing */

                }

            } else { /* received a standard message, not a notification */

                /* no standard messages expected: do nothing */

            }

        }

      sys_irqrmpolicy(&hook_id);
}
