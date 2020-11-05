#pragma once

/**
 * @file timer.h
 * @author Ivo Saavedra
 * @author Rodrigo Reis
 * @brief 
 * @version 0.1
 */

#include <lcom/lcf.h>


#ifndef PROJ_TIMER_H
#define PROJ_TIMER_H

int get_timer_counter();

/**
 * @brief Subscribes timer interruptions
 * 
 */
void timer_subscribe_interrupt();

/**
 * @brief Unsubscribes timer interruptions
 * 
 */
void timer_unsubscribe_interrupt();

/**
 * @brief Timer IH
 * 
 */
void timer_interrupt_handler();


#endif //PROJ_TIMER_H
