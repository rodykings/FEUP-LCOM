#pragma once

/**
 * @file game.h
 * @author Ivo Saavedra
 * @author Rodrigo Reis
 * @brief game.h 
 * @version 0.1
 * 
 */

//color = 0x6FF00
//MODULES
#include "state_machine.h"
#include "i8024.h"

//IMAGES

#include "img/cursor.h"
#include "img/btn_done.h"
#include "img/btn_done_hover.h"
#include "img/ai_mark.h"
#include "img/player_mark.h"
#include "img/winner.h"
#include "img/game_over.h"

#ifndef PROJ_INTERRUPTS_H
#define PROJ_INTERRUPTS_H

#define IDLE_T_1 1
#define IDLE_T_2 2
/**
 * @brief Single Player
 *        Subscribes interruptions of the necessary modules
 *        Iniciates the boards from the respective players
 *        Starts the game loop (1 turn per player)
 * 
 * @return 0 
 */
int game_sp(char *buffer);





int game_mp(char *buffer);
int game_over_sp();
int winner_sp();




/**
 * @brief Lets the player initialize the board unsign a graphical interface
 *        10 boats must be positioned before exiting
 * 
 * @param plr the player
 * @param buffer (double buffering);
 * @return 0
 */
int game_init_board(Player *plr, char *buffer, Sprite *btn, uint8_t *btn_done, uint8_t *btn_hover);

/**
 * @brief Lets the player fire against the enemy's boats using a graphical interface
 *        Shows previously hit cells and destroyed boats (AI)
 * 
 * @param plr_1 the player
 * @param plr_2 player_2 (enemy)
 * @param back  background pixmap
 * @param cur   cursor sprite
 * @param obj   object sprites used to display hit cells (fire, water, protected)
 * @param buffer (double buffering)
 * @param board_obj auxiliar background
 * @param btn done button
 * @return 0 
 */
int game_round_sp(Player *plr_1, Player *plr_2,uint8_t *back, Sprite *cur,Sprite *obj[5], char *buffer, char *board_obj, uint8_t* mark);
//int game_round_sp_2(Player *plr_1, Player *plr_2,uint8_t *back, Sprite *cur,Sprite *obj[5], char *buffer);

/**
 * @brief Randomly fires upon player's boats using a state machine to simulate a strategy
 *        In case of a hit the AI fires to adjacent cells until the boat is destroyed
 *        Displays the players boats and the cells where the AI fired
 * 
 * @param plr_2 the player
 * @param plr_1 the AI
 * @param back  background pixmap
 * @param obj   object sprites used to display hit cells (fire, water, protected)
 * @param buffer (double buffering)
 * @return 0
 */
int game_round_ai(Player *plr_2, Player *plr_1,uint8_t *back, Sprite *obj[5], char *buffer,uint8_t* mark);

/**
 * @brief Uses timer interruptions to wait a predetermined time
 * 
 * @param idle time to wait (seconds)
 */
void idle_time(int idle);

#endif //PROJ_INTERRUPTS_H
