// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include "mouse.h"
#include "i8024.h"

// Any header files included below this line should have been created by you
extern int cntBytes;
struct packet pp;
extern uint32_t data;
extern int counter_timer;
extern bool discard;


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (mouse_test_packet)(uint32_t cnt) {
    static const struct packet EmptyStruct;
    //ENABLE DATA REPORTING
    mouse_command(0xD4,0xF4);

    //mouse_enable_data_reporting();
    int hook_id = 12;
    sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE, &hook_id);
    uint16_t irq_set = BIT(12);
    while(cnt) {

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
                    if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */

                        mouse_ih();

                        init_packet(data);
                        if (cntBytes == 3){

                            mouse_print_packet(&pp);
                            pp = EmptyStruct;
                            cntBytes = 0;
                            cnt--;
                        }
                        
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
    //DISABLE DATA REPORTING
    mouse_command(0xD4,0XF5);



    return 0;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    static const struct packet EmptyStruct;
    cntBytes = 0;
    uint32_t stat;
    while(cnt){
        //read packet request
        mouse_command(0xD4,0xEB);

        while(1){

            //polling STAT_REG
            sys_inb(STAT_REG, &stat);

            if(stat & OBF){

                sys_inb(OUT_BUF, &data);

                if ( ((stat &(PAR_ERR | TO_ERR)) == 0) && (stat & MOUSE_DATA)) {

                    cntBytes++;
                    init_packet(data);
                    if (cntBytes == 3){
                        mouse_print_packet(&pp);
                        pp = EmptyStruct;
                        cntBytes = 0;
                        cnt--;
                        break;
                    }
                }

            }
            tickdelay(micros_to_ticks(period*1000));
        }
    }

    //SET STREAM MODE
    mouse_command(0xD4,0xEA);
    //DISABLE DATA REPORTING
    mouse_command(0xD4,0xF5);

    uint8_t def = minix_get_dflt_kbc_cmd_byte();
    mouse_command(0x60, def);

    return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
    static const struct packet EmptyStruct;
    //ENABLE DATA REPORTING
    mouse_command(0xD4,0xF4);


    int hook_id = 12;
    sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE, &hook_id);

    //mouse_enable_data_reporting();
    uint8_t bit_no;
    timer_subscribe_int(&bit_no);

    uint16_t irq_set = BIT(12);
    int aux_time = idle_time;
    while(aux_time) { /* You may want to use a different condition */

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

                    if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */

                        aux_time = idle_time;
                        counter_timer = 0;

                        mouse_ih();

                        init_packet(data);

                        if((cntBytes==3) && !discard){
                            mouse_print_packet(&pp);
                            pp = EmptyStruct;
                            cntBytes = 0;
                        }
                        else discard = false;


                    }

                    if (msg.m_notify.interrupts & BIT(0)) { /* subscribed interrupt */

                        timer_int_handler();

                        if(counter_timer%60 == 0){

                            timer_print_elapsed_time();
                            aux_time--;

                        }

                    }

                default:

                    break; /* no other notifications expected: do nothing */

            }

        } else { /* received a standard message, not a notification */

        }

    }

    //unsubscribe

    sys_irqrmpolicy(&hook_id);

    //DISABLE DATA REPORTING
    mouse_command(0xD4,0XF5);

    timer_unsubscribe_int();

    return 0;
}

bool end = false;
int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
    static const struct packet EmptyStruct;
    //ENABLE DATA REPORTING
    mouse_command(0xD4,0xF4);

    int hook_id = 12;
    sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE, &hook_id);
    uint16_t irq_set = BIT(12);
    while(!end) {

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
                    if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */

                        mouse_ih();

                        init_packet(data);
                        if (cntBytes == 3){

                            struct mouse_ev evt;
                            struct mouse_ev* ptr;
                            ptr = &evt;
                            ptr = mouse_detect_event(&pp);

                            check_hor_line(ptr, tolerance, x_len);

                            mouse_print_packet(&pp);
                            pp = EmptyStruct;
                            cntBytes = 0;
                        }

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
    //DISABLE DATA REPORTING
    mouse_command(0xD4,0XF5);

    return 0;
}

