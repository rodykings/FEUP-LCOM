#pragma once

/**
 * @file board.h
 * @author Ivo Saavedra
 * @author Rodrigo Reis
 * @brief 
 * @version 0.1
 */

#include "img/background.h" //standard background
#include "img/background2.h"  //background for init board
#include "img/water.h"  //water cell
#include "img/fire.h"   //fire cell
#include "img/protected.h"  //protected cell
#include "boat.h"

#ifndef BOARD_H
#define BOARD_H

typedef struct{
	
  unsigned int id;
  int board_mat[10][10];

}Board;

#define BOARD_XI 55
#define BOARD_XF 555
#define BOARD_YI 60
#define BOARD_YF 600
#define CELL_WIDTH 50

#define BOAT1_XI 580 //size 1
#define BOAT1_YI 90

#define BOAT2_XI 580 //size 2
#define BOAT2_YI 140

#define BOAT3_XI 600 //size 3
#define BOAT3_YI 230

#define BOAT4_XI 720 //size 4
#define BOAT4_YI 185

//matrix cell state representation
#define BLOCK_CELL -1
#define WATER_CELL -6
#define FIRE_CELL -7
#define PROT_CELL -8
#define WATER_AJUST 6

/**
 * @brief Create a board object
 * 
 * @param id id of the board
 * @return Board* 
 */
Board *create_board(unsigned int id);

/**
 * @brief Checks if a sprite has been placed inside the board matrix
 * 
 * @param board 
 * @param sp  
 * @param x_pos sprite collision point ajustment
 * @param y_pos sprite collision point ajustment
 * @return 0 if the selection was outside the matrix; 1 if water was hit and 2 if a boat was hit
 */
int check_matrix_collision(Board *board,Sprite *sp, int x_pos, int y_pos);

/**
 * @brief Receives a position and acerts what cell type has been hit
 * 
 * @param board board to check
 * @param line y coordinate
 * @param col x coordinate
 * @return 0 if invalid hit, 1 if water was hit and 2 if a boat was hit
 */
int check_matrix_collision_cord(Board *board, int line, int col);

/**
 * @brief  Acerts wich boat has been selected
 * 
 * @param cur cursor sprite (collision)
 * @param boats array containing all the boats
 * @return id of the selected boat
 */
int check_board_selection(Sprite *cur, Boat *boats[10]);

/**
 * @brief Creates boat objects and loads them into an array
 * 
 * @param boats array were the boats will be stored
 */
void load_boats(Boat *boats[10]);

/**
 * @brief Iterates over a boat array and shows the boats
 * 
 * @param boats boat array
 * @param buffer (double buffering)
 */
void draw_boats(Boat *boats[10],char *buffer);

/**
 * @brief Iterates over the boats array and draws the ones wich have 0 hitpoints
 * 
 * @param boats boat array
 * @param buffer (double buffering)
 */
void draw_destroyed_boats(Boat *boats[10], char *buffer);

/**
 * @brief Removes a boat from the board matrix
 * 
 * @param boat selected boat
 * @param board board containing the matrix
 */
void remove_boat_from_matrix(Boat *boat, Board *board);

/**
 * @brief boat object destructor
 * 
 * @param boats array of boats
 */
void destroy_boats(Boat *boats[10]);

/**
 * @brief Increments (with -1) the surrounding cells of a boat
 *        Blocked cells range from -1 to -4
 * 
 * @param board board containing the matrix
 * @param boat 
 */
void block_adjacent_cells(Board *board, Boat *boat);

/**
 * @brief Sets the surrounding cells of a boat to -7
 * 
 * @param board board containingthe matrix
 * @param boat 
 */
void set_protected_cells(Board *board, Boat *boat);

/**
 * @brief Places a boat on the matrix using the sprite
 *        If the boat was previously placed on the matrix, it is first removed an then placed
 *        in the new position
 * 
 * @param boat boat to be placed
 * @param board board containing the matrix
 * @return 1 if boat has been placed successfully; 0 if not 
 */
int place_boat(Boat *boat, Board *board);

/**
 * @brief Places a boat on the matrix in a given position
 * 
 * @param boat boat to be placed
 * @param board board containing the matrix
 * @param line y coordinate
 * @param col x coordinate
 * @return 0
 */
int place_boat_cord(Boat *boat, Board *board, int line, int col);

/**
 * @brief Checks if the boat can be placed in the given postion of the matrix
 * 
 * @param boat boat to be placed
 * @param board board containing the matrix
 * @param col x coordinate
 * @param line y coordinate
 * @param vertical boat orientation
 * @return 1 if valid position; 0 if not
 */
int verify_boat_placement(Boat* boat, Board *board, int col, int line, bool vertical);


/**
 * @brief Loads the objects used for representing water, fire and protected cells 
 * 
 * @param obj array were the objects will be stored
 */
void load_objects(Sprite *obj[3]);

/**
 * @brief Converts the states of the matrix into objects and shows them
 * 
 * @param board board with the matrix states
 * @param obj   array of objects
 * @param buffer (double buffering)
 */
void draw_objects(Board *board, Sprite *obj[3], char *buffer);

//DEBUG
/**
 * @brief Prints a matrix (debug)
 * 
 * @param b board to be printed
 */
void print_mat(Board*b);

#endif //PROJ_BOARD_H
