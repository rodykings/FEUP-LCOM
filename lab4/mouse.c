//
// Created by rodykings on 30/10/19.
//

#include <lcom/lcf.h>
#include "i8024.h"
#include "mouse.h"

int counter = 0;
int cntBytes = 0;
extern struct packet pp;
uint32_t data;
bool discard = false;

void (mouse_ih)(){

    while( counter < 5 ) {

        uint32_t stat;
        uint32_t bytes;

        sys_inb(STAT_REG, &stat); /*assuming it returns OK*/

        /*loop while 8042 output buffer is empty*/

        if(stat & OBF) {

            sys_inb(OUT_BUF, &bytes); /*assuming it returns OK*/
            if ( ((stat &(PAR_ERR | TO_ERR)) == 0) && (stat & MOUSE_DATA)) {

                //if ((data & BIT(3)) && cntBytes == 0) discard = true;

                data = bytes;
                cntBytes++;
                break;
            }

        }
        counter++;
    }
}


int (mouse_command)(uint8_t cmd, uint8_t arg){

    int counter  = 5;
    uint32_t status;
    uint32_t ack = 0xFE;

    while(counter && ack != 0xFA){
        sys_outb(STAT_REG, cmd);
        sys_inb(STAT_REG, &status);
        if(!(status & IBF)){ //input buffer not full
            sys_outb(OUT_BUF, arg);

            sys_inb(OUT_BUF,&ack);

            if (ack == 0xFC) return 1;
            if (ack == 0xFA) return 0;
        }
        tickdelay(micros_to_ticks(DELAY_US));
        counter--;
    }
    return 0;
}

void (init_packet)(uint32_t data){

    if(cntBytes == 1){
        uint8_t d = (uint8_t)data;
        pp.bytes[0] = d;
        pp.rb = d & BIT(1);
        pp.mb = d & BIT(2);
        pp.lb = d & BIT(0);
        pp.x_ov = d & BIT(6);
        pp.y_ov = d & BIT(7);

        if ((data & BIT(4))) pp.delta_x = 0xFF00;
        if ((data & BIT(5))) pp.delta_y = 0xFF00;

    }else if(cntBytes == 2){
        pp.bytes[1] = (uint8_t)data;
        pp.delta_x = (pp.delta_x | (uint16_t)data);
    } else{
        pp.bytes[2] = (uint8_t)data;
        pp.delta_y = (pp.delta_y | (uint16_t)data);
    }
}


extern bool end;

void (check_hor_line)(struct mouse_ev*evt, uint8_t tolerance, uint8_t len) {
    // initial state; keep state
    static int total_len = 0;
    static state_t st = INIT;

    switch (st) {
        case INIT:
            total_len = 0;
            if( evt->type == LB_PRESSED){

                st = DRAW_UP;
            }
            break;
        case DRAW_UP:

            if( evt->type == MOUSE_MOV){
                total_len += evt->delta_x;
                if (((abs(evt->delta_x) > tolerance) && evt->delta_x < 0) || ((abs(evt->delta_y) > tolerance) && evt->delta_y < 0)){
                    st = INIT;
                    break;
                }

             } else if( evt->type == LB_RELEASED ){

                if (total_len < len){
                    st = INIT;
                    break;
                }
                st = VERT;
            }
            break;
        case VERT:
            if( evt->type == MOUSE_MOV){

                if (abs(evt->delta_x) > tolerance && abs(evt->delta_y) > tolerance){
                    st = INIT;
                    break;
                }

            } else if( evt->type == RB_PRESSED){

                st = DRAW_DW;
            }
            break;
        case DRAW_DW:
            if( evt->type == MOUSE_MOV){

                total_len += evt->delta_x;

                if (((abs(evt->delta_x) > tolerance) && evt->delta_x < 0) || ((abs(evt->delta_y) > tolerance) && evt->delta_y > 0)){
                    st = INIT;
                    break;
                }

            } else if( evt->type == RB_RELEASED ){

                if (total_len < len){
                    st = INIT;
                    break;
                }
                end = true;
            }
             break;
        default:
            break;
    }
}

/*
struct mouse_ev * (detect_event) (struct packet *pp){

    static int last_rb = 0;
    static int last_lb = 0;

    static int last_x = 0;
    static int last_y = 0;

    struct mouse_ev ev;
    struct mouse_ev *ptr;
    ptr = &ev;

    if ((pp->delta_x != 0) || (pp->delta_y != 0)){
        ev.delta_x = pp->delta_x;
        ev.delta_y = pp->delta_y;
        last_x = ev.delta_x;
        last_y = ev.delta_y;
        ev.type = MOUSE_MOV;
        return ptr;
    }
    else if (pp->lb){
        ev.type = LB_PRESSED;
        last_lb = 1;
        ev.delta_x = 0;
        ev.delta_y = 0;
        return ptr;
    }
    else if (pp->rb) {
        ev.type = RB_PRESSED;
        last_rb = 1;
        ev.delta_x = last_x;
        ev.delta_y = last_y;
        return ptr;
    }
    else if (last_rb){
                ev.type = RB_RELEASED;
                last_rb = 0;
                ev.delta_x = last_x;
                ev.delta_y = last_y;
                return ptr;
            }
    else if (last_lb){
                ev.type = LB_RELEASED;
                last_rb = 0;
                ev.delta_x = last_x;
                ev.delta_y = last_y;
                return ptr;
            }
    else {
        ev.type = MOUSE_MOV;
        ev.delta_x = 0;
        ev.delta_y = 0;
    }

    return ptr;
}
*/


