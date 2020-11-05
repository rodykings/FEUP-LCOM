#pragma once

/**
 * @file kbd.h
 * @author Ivo Saavedra
 * @author Rodrigo Reis
 * @brief 
 * @version 0.1
 */

#include "i8024.h"

#ifndef PROJ_KBD_H
#define PROJ_KBD_H

/**
 * @brief Subscribe keyboard interruptions
 * 
 */
void kbd_subscribe_int();

/**
 * @brief Unsubscribe keyboard interruption
 * 
 */
void kbd_unsubscribe_int();

/**
 * @brief Keyboard IH
 * 
 */
void (kbc_ih)();

/**
 * @brief Enables keyboard interruptions
 * 
 */
void enable_kbd_interrupts();

int get_scancode();

#endif //KBD_H
