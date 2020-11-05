#pragma once 
/**
 * @file interrupt_cicle.h
 * @author Ivo Saavedra
 * @author Rodrigo Reis
 * @brief 
 * @version 0.1
 * 
 */

#include "player.h"



struct interrupt_data{

    bool timer, mouse, kbd;
    uint8_t scancode;
    bool lb, rb, mb;
    int delta_x, delta_y;

};


#define MOUSE_IRQ_SET BIT(12)
#define KBD_IRQ_SET BIT(1)
#define TIMER_IRQ_SET BIT(0)

#ifndef PROJ_INTERRUPT_CICLE_H
#define PROJ_INTERRUPT_CICLE_H

struct interrupt_data get_int_data();

/**
 * @brief Receives and handles interruptions from the necessary modules
 * 
 * @return 0
 */
int interrupt_cicle();

/**
 * @brief Initializes the interrupt_data struct tot be used in other files
 * 
 */
void init_int_data();

void enable_components();
void disable_components();

#endif //PROJ_INTERRUPT_CICLE
