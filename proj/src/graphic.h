#pragma once

/**
 * @file graphic.h
 * @author Ivo Saavedra
 * @author Rodrigo Reis
 * @brief 
 * @version 0.1
 * 
 */

#ifndef PROJ_GRAPHIC_H
#define PROJ_GRAPHIC_H

char *get_video_mem();
unsigned get_h_res();
unsigned get_v_res();
unsigned get_bytes_per_pixel();

#define TRANSPARENCY_COLOR 0x00FF66
#define VIDEO_MODE 0x115

/**
 * @brief allocates necessary memory and enters the video mode
 * @param mode video mode
 */
void* (vg_init)(uint16_t mode);

/**
 * @brief Copies the xmp into the video mem
 * 
 * @param pixmap image pixmap
 * @param x x coordinate
 * @param y y coordinate
 * @param width image width
 * @param height image height
 * @param buff buffer
 */
void draw_xpm(uint8_t *pixmap, uint16_t x, uint16_t y, uint32_t width, uint32_t height,char *buff);

/**
 * @brief Calculates the color and sets the pixel in the video mem
 * 
 * @param x pixel x coordinate
 * @param y pixel y coordinate
 * @param pixmap image pixmap
 * @param byte_pos position of the first color pixel in the pixmap
 * @param buff buffer
 * @return 0
 */
int draw_pixel(uint32_t x, uint32_t y, uint8_t* pixmap, uint32_t byte_pos, char* buff);
#endif //PROJ_GRAPHIC_H
