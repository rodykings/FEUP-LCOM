#pragma once

#include "interrupt_cicle.h"

/**
 * @file state_machine.h
 * @author Ivo Saavedra
 * @author Rodrigo Reis
 * @brief 
 * @version 0.1
 * 
 */

enum fire_ai{RANDOM, MOV_R, MOV_L, MOV_D, MOV_U};
enum sel_boat_st {INIT,DRAG, DROP, ROT};

/**
 * @brief State Machine used for dragging and droping boats
 *        Places boats on the board and updates the atributes of the respective boat      
 * 
 * @param board Board where the boats will be placed
 * @param boats array conatining all the boats that are to be placed
 * @param cur   cursor sprite (used for collisions)
 * @return 1 if a boat was placed; -1 if a boat was removed; 0 otherwise
 */
int select_boat_sm(Board *board, Boat *boats[10], Sprite *cur);

/**
 * @brief State Machine used for simulating a game strategy
 *        Generates a random position and then fires on that position
 *        If a boat is hit, then the AI fires to adjacent positions until the boat is destroyed
 * 
 * @param ai the AI
 * @param plr_1 the player (enemy)
 * @return 1 if shot was valid, 0 otherwise
 */
int game_round_ai_sm(Player *ai, Player *plr_1);

