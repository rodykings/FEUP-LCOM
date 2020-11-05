#pragma once 

/**
 * @file mouse.h
 * @author Ivo Saavedra
 * @author Rodrigo Reis
 * @brief 
 * @version 0.1
 * 
 */

#include "i8024.h"

#ifndef MOUSE_H
#define MOUSE_H

/**
 * @brief Subscribe mouse interrupts
 * 
 */
void mouse_subscribe_int();

/**
 * @brief Unsubscribe mouse interrupts
 * 
 */
void mouse_unsubscribe_int();

/**
 * @brief Mouse IH
 * 
 */
void (mouse_ih)();

/**
 * @brief Sends a command to the STAT_REG
 * 
 * @param cmd command
 * @param arg argument
 * @return 0 if success; 1 if not
 */
int mouse_command(uint8_t cmd, uint8_t arg);

/**
 * @brief Initiates the mouse packet member
 * 
 */
void init_packet();

/**
 * @brief Empties a mouse packet
 * 
 */
void empty_packet();

int get_delta_x();
int get_delta_y();
int get_cnt_bytes();
struct packet get_packet();

#endif //MOUSE_H
