#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdint.h>
#include "i8254.h"

int counter = 0;
int hook_id;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {

    uint16_t t_frequency = (uint16_t)(TIMER_FREQ/freq);

    uint8_t ptrConf;
    timer_get_conf(timer, &ptrConf);

    //timer mask
    ptrConf = ptrConf & 0x0F;
    if (timer == 0) {
        ptrConf = ptrConf | 0x30;
    } else if (timer == 1) {
        ptrConf = ptrConf | 0x70;
    } else if (timer == 2) {
        ptrConf = ptrConf | 0xB0;
    } else {
        return 0;
    }

    //command to write lsb followed by lsb
    sys_outb(TIMER_CTRL, ptrConf);

    //frequency LSB
    uint8_t LSB;
    util_get_LSB(t_frequency, &LSB);
    sys_outb(TIMER_0, LSB);

    //frequency MSB
    uint8_t MSB;
    util_get_MSB(t_frequency, &MSB);
    sys_outb(TIMER_0, MSB);

    return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
    *bit_no = 0;
    hook_id = 0;
    sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id);
    return 0;
}
int (timer_unsubscribe_int)() {
    sys_irqrmpolicy(&hook_id);
    return 0;
}

void (timer_int_handler)() {
    counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
    uint8_t readBackCommand;
    if (timer == 0)
        readBackCommand = 0xE2;
    else if (timer == 1)
        readBackCommand = 0xE4;
    else if (timer == 2)
        readBackCommand = 0xE8;
    else
        return 0;

    sys_outb(TIMER_CTRL, readBackCommand);
    util_sys_inb(TIMER_0 + timer, st);

    return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {

    //VERIFY READ MODE
    uint32_t bcd = st & 0x01;

    //PROGRAMMED MODE
    uint32_t opm = st;
    opm = opm >> 1;
    opm = opm & 0x07;
    //TYPE OF ACESS
    uint32_t ta = st & 0x30;
    ta = ta >> 4;
    //NULL COUNT
    uint32_t nc = st & 0x40;
    nc = nc >> 6;
    //OUTPUT
    uint32_t output = st & 0x80;
    output = output >> 7;

    union timer_status_field_val status;

    switch (field){
        case 0://tsf_all
            status.byte = st;
            break;
        case 1://tsf_initial
            if(ta == 0x01){
                status.in_mode = LSB_only;
            }else if(ta == 0x02){
                status.in_mode = MSB_only;
            }else if(ta == 0x03){
                status.in_mode = MSB_after_LSB;
            }else{
                status.in_mode = INVAL_val;
            }
            break;
        case 2://tsf_mode
            status.count_mode = opm;
            break;
        case 3://tsf_base
            if(bcd == 0x01)
                status.bcd = true;
            else
                status.bcd = false;
            break;
    }
    timer_print_config(timer, field, status);

    return 0;
}




