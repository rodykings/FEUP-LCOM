#include <lcom/lcf.h>
#include "interrupt_cicle.h"

static struct interrupt_data d;
struct interrupt_data get_int_data() {return d;}

int interrupt_cicle(){

    bool exit = false;

    static const struct interrupt_data empty_data;
    d = empty_data;

    while(!exit) {

        uint8_t r;

        int ipc_status;

        message msg;

        //s+o pode ser chamado uma vez
        if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {

            printf("driver_receive failed with: %d", r);
            continue;

        }

        if (is_ipc_notify(ipc_status)) { /* received notification */

            switch (_ENDPOINT_P(msg.m_source)) {

                case HARDWARE: /* hardware interrupt notification */

                    if (msg.m_notify.interrupts & MOUSE_IRQ_SET) {  // mouse interrupt
                        mouse_ih();

                        init_packet();

                        if (get_cnt_bytes() == 3){

                            //packet_to_evt();
                            
                            exit = true;
                            d.mouse = true;
                        }

                    }

                    if (msg.m_notify.interrupts & KBD_IRQ_SET) {   // keyboard interrupt

                        kbc_ih();
                        d.kbd = true;
                        exit = true;

                    }

                    if (msg.m_notify.interrupts & TIMER_IRQ_SET) {    // timer interrupt

                        timer_interrupt_handler();
                        
                        d.timer = true;
                        exit = true;

                    }

                    break;
                default:
                    break;
                
            }

        } else { 
        }

    }

    init_int_data();
    return 0;
}

void init_int_data(){

    if (d.mouse) {
        struct packet p = get_packet();
        d.lb = p.lb;
        d.rb = p.rb;
        d.mb = p.mb;
        d.delta_x = p.delta_x;
        d.delta_y = -p.delta_y;

        empty_packet();
    }
    if (d.kbd) {
        d.scancode = get_scancode();
    }
}

void enable_components(){

  mouse_subscribe_int();
  kbd_subscribe_int();
  timer_subscribe_interrupt();

}

void disable_components(){

    mouse_unsubscribe_int();
    kbd_unsubscribe_int();
    timer_unsubscribe_interrupt();
}

