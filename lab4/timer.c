#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdint.h>
#include "i8254.h"

int counter_timer = 0;
int timer_hook_id = 0;

int (timer_subscribe_int)(uint8_t *bit_no) {
    *bit_no = 0;
    timer_hook_id = 0;
    sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id);
    return 0;
}
int (timer_unsubscribe_int)() {
    sys_irqrmpolicy(&timer_hook_id);
    return 0;
}

void (timer_int_handler)() {
    counter_timer++;
}

