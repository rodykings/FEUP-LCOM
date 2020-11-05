#pragma once

/**
 * @file player.h
 * @author Ivo Saavedra
 * @author Rodrigo Reis
 * @brief player.h
 * @version 0.1
 * 
 */

#include "board.h"

typedef struct{

	Board *board;
	Boat *boats[10];
	int destroyed_boats;

}Player;

/**
 * @brief Create a Player object
 * 
 * @return Player* 
 */
Player *createPlayer();

/**
 * @brief Generates a random board with placed boats for the AI
 * 
 * @param ai the AI
 */
void generate_random_matrix(Player *ai);

/**
 * @brief Recieves a player (plr_1) and determines if the shot against the enemy (plr_2) is valid
 * 
 * @param plr_1 the player
 * @param plr_2 the enemy
 * @param cur 	curso sprite
 * @return 1 if shot was valid, 0 otherwise
 */
int player_shot(Player *plr_1, Player *plr_2, Sprite *cur);


