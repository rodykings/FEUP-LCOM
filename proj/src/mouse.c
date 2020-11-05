#include <lcom/lcf.h>
#include "mouse.h"

//a alterar
static struct packet pp;
static int cnt_bytes = 0;
static int ih_counter = 0;
static uint8_t data;

static int delta_x;
static int delta_y;

int get_delta_x() { return delta_x; }
int get_delta_y() { return delta_y; }

int get_cnt_bytes(){return cnt_bytes;}

struct packet get_packet() {return pp;}

static int mouse_hook_id;
void mouse_subscribe_int(){

    mouse_hook_id = 12;
    //ENABLE DATA REPORTING
    mouse_command(0xD4,0xF4);

    sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE, &mouse_hook_id);

}
void mouse_unsubscribe_int(){

    sys_irqrmpolicy(&mouse_hook_id);

    //DISABLE DATA REPORTING
    mouse_command(0xD4,0XF5);

}

void empty_packet(){
    static const struct packet EmptyStruct;
    pp = EmptyStruct;
    cnt_bytes = 0;
}



void (mouse_ih)(){

    while( ih_counter < 5 ) {

        uint32_t stat;
        uint32_t bytes;

        sys_inb(STAT_REG, &stat); /*assuming it returns OK*/
        /*loop while 8042 output buffer is empty*/
        if(stat & OBF) {

            sys_inb(OUT_BUF, &bytes); /*assuming it returns OK*/

            if ( ((stat &(PAR_ERR | TO_ERR)) == 0) && (stat & MOUSE_DATA)) {

                if ((data & BIT(3)) && cnt_bytes == 0){ //asyncronous
                     empty_packet();
                }

                data = bytes;
                cnt_bytes++;
                break;
            }
        }
        ih_counter++;
    }
}

int mouse_command(uint8_t cmd, uint8_t arg){

    int counter  = 10;
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

void init_packet(){

    if(cnt_bytes == 1){

        uint8_t d = (uint8_t)data;

        pp.bytes[0] = d;
        pp.rb = d & BIT(1);
        pp.mb = d & BIT(2);
        pp.lb = d & BIT(0);
        pp.x_ov = d & BIT(6);
        pp.y_ov = d & BIT(7);

        if ((data & BIT(4))) pp.delta_x = 0xFF00;
        if ((data & BIT(5))) pp.delta_y = 0xFF00;

    }else if(cnt_bytes == 2){

        pp.bytes[1] = (uint8_t)data;
        pp.delta_x = (pp.delta_x | (uint16_t)data);

    } else{

        pp.bytes[2] = (uint8_t)data;
        pp.delta_y = (pp.delta_y | (uint16_t)data);
    }
}

