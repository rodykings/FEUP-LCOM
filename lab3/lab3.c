#include <lcom/lcf.h>
#include <lcom/lab3.h>
#include <lcom/lab2.h>
#include <lcom/timer.h>
#include <stdbool.h>
#include <stdint.h>
#include "i8024.h"
#include "keyboard.h"
#include "timer.h"
#include <minix/sysutil.h>

#define DELAY_US    20000




int hook_id = 1;
extern uint8_t scanCode;
uint8_t bit_no=0;
uint32_t cnt = 0;
int counter = 0;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {
  //subscribe
  sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE, &hook_id);

  //wait for ESQ
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
                        print_scanCode(scanCode);
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


  printf("%d", cnt);
  //int kbd_print_no_sysinb(cnt);//print sys_inb counter

  return 0;
}

int(kbd_test_poll)() {

    //bool exit = true;

    //wait for ESQ
    while (scanCode != 0x81) { /* You may want to use a different condition */

        uint32_t stat;
        uint32_t data;

        while (1) {
            #define DLAB3
            sys_inb(KBC_STAT_REG, &stat);
            #ifdef DLAB3
            cnt++;
            #endif
            /*assuming it returns OK*//*loop while 8042 output buffer is empty*/
            if (stat & OBF) {
                #define DLAB3
                sys_inb(OUT_BUF, &data);
                #ifdef DLAB3
                cnt++;
                #endif
                /*assuming it returns OK*/
                if ((stat & (PAR_ERR | TO_ERR)) == 0){
                    scanCode = data;
                    break;
                }
                else
                    break;
            }
            tickdelay(micros_to_ticks(DELAY_US));

        }

        print_scanCode();


    }

    enable_kbd_interrupts();

    kbd_print_no_sysinb(cnt);//print sys_inb counter

    return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {


    //subscribe
    sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE, &hook_id);

    timer_subscribe_int(&bit_no);


    int idleTime = n;

    while(scanCode != 0x81 & counter/60!=n) { /* You may want to use a different condition */

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
                        counter = 0;
                        kbc_ih();
                        print_scanCode();
                    }
                    if (msg.m_notify.interrupts & BIT(0)) { /* subscribed interrupt */

                        timer_int_handler();

                        if(counter%60 == 0){

                            timer_print_elapsed_time();

                            idleTime--;

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
    timer_unsubscribe_int();

    kbd_print_no_sysinb(cnt);//print sys_inb counter

    return 0;

}
