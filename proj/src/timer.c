//
// Created by ivo_s on 28/11/2019.
//

#include "timer.h"
//#include <lcom/lcf.h>

#include <stdint.h>

#include "i8254.h"

static int timer_counter = 0;

int get_timer_counter() {return timer_counter;}

static int timer_hook_id;

void timer_subscribe_interrupt() {

    timer_hook_id = 0;

    sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id);

}

void timer_unsubscribe_interrupt() {

    sys_irqrmpolicy(&timer_hook_id);

}

void timer_interrupt_handler() {

    timer_counter++;

}


