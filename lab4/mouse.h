//
// Created by rodykings on 30/10/19.
//
#include <stdint.h>
#include <stdio.h>
#include "i8024.h"

#ifndef LAB4_MOUSE_H
#define LAB4_MOUSE_H

void (mouse_ih)();
int (mouse_command)(uint8_t cmd, uint8_t arg);
int (mouse_subscribe_int)(uint8_t *bit_no);
void (init_packet)(uint32_t data);
void (check_hor_line)(struct mouse_ev*evt, uint8_t tolerance, uint8_t len);
//struct mouse_ev * (detect_event) (struct packet *pp);
  
#endif //LAB4_MOUSE_H
