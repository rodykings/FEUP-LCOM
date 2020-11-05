#pragma once

/**
 * @file sprite.h
 * @brief 
 * @version 0.1
 * 
 */

#include "graphic.h"
#include "mouse.h"
#include "kbd.h"
#include "timer.h"

#ifndef PROJ_SPRITE_H
#define PROJ_SPRITE_H

typedef struct {
  int x, y; // current position
  int width, height;  // dimensions
  int xspeed, yspeed; // current speed
  uint8_t *map;          // the pixmap
} Sprite;


Sprite*create_sprite(xpm_map_t xpm, int x, int y,int xspeed, int yspeed);

void destroy_sprite(Sprite*sp);

/**
 * @brief Shows the sprite
 * 
 * @param sp sprite
 * @param buff buffer
 * @return 0
 */
int draw_sprite(Sprite *sp, char* buff);

/**
 * @brief Checks sprite collision between two sprites
 * 
 * @param a sprite a
 * @param b sprite b
 * @return 1 if collision; 0 if not
 */
int check_sprite_collision(Sprite *a, Sprite *b);

#endif //PROJ_SPRITE_H

