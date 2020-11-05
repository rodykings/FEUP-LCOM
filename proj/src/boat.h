#pragma once

/**
 * @file boat.h
 * @author Ivo Saavedra
 * @author Rodrigo Reis
 * @brief 
 * @version 0.1
 * 
 */

#include "img/boat_1.h"
#include "img/boat_2.h"
#include "img/boat_2_hor.h"
#include "img/boat_3.h"
#include "img/boat_3_hor.h"
#include "img/boat_4.h"
#include "img/boat_4_hor.h"
#include "img/b.h"
#include "sprite.h"

#ifndef PROJ_BOAT_H
#define PROJ_BOAT_H

typedef struct{
	int id;
	unsigned int size;
	int matrix_pos[2];
	bool up;
	int xi,yi;
	int hit_points;
	uint8_t *def;
	uint8_t *rev;
	Sprite *sp;
}Boat;

/**
 * @brief Creates a boat object
 * 
 * @param boat_img default boat xpm (vertical)
 * @param boat_img_rev reverse boat xpm (horizontal)
 * @param x initial x coordinate
 * @param y initial y coordinate
 * @param size boat size [1,4]
 * @param id boat id [1,10]
 * @return Boat* 
 */
Boat *create_boat(xpm_map_t boat_img, xpm_map_t boat_img_rev, int x, int y, unsigned int size, int id);

/**
 * @brief Rotates a boat
 * 
 * @param b boat to rotate
 */
void rotate_boat(Boat *b);

/**
 * @brief Boat object destructor
 * 
 * @param boat 
 */
void destroy_boat(Boat *boat);

#endif //PROJ_BOAT_H
