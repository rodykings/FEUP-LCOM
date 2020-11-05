#include <lcom/lcf.h>
#include "player.h"


Player *createPlayer(){

  Player *p = (Player*) malloc(sizeof(Player));
  p->board = create_board(1);
  load_boats(p->boats);
  p->destroyed_boats = 0;

  return p;
}

void generate_random_matrix(Player *ai){

  int line = 0;
  int col = 0;
  //int vert = 1;


  bool placed = false;

  for(int i=0; i<10; i++){

    while (!placed)
    {
      line = rand() % 10;
      col = rand() % 10;
      bool swap = rand() % 2;

      ai->boats[i]->matrix_pos[0] = line;
      ai->boats[i]->matrix_pos[1] = col;

      if (swap){
        rotate_boat(ai->boats[i]);
      }
      if (verify_boat_placement(ai->boats[i],ai->board,col,line,ai->boats[i]->up)){

        placed = true;
        ai->boats[i]->sp->x = col * CELL_WIDTH + BOARD_XI + 2;
        ai->boats[i]->sp->y = line * CELL_WIDTH + BOARD_YI + 7;
        place_boat_cord(ai->boats[i],ai->board,line,col);
      }

    }

    placed = false;

  }
}

int player_shot(Player *plr_1, Player *plr_2, Sprite *cur){

  
  int val = check_matrix_collision(plr_2->board,cur,0,0);

  if (val == 1){  //water

      int col = (cur->x)/50 -1;
      int line = (cur->y)/50 -1;
      
      int cell = plr_2->board->board_mat[line][col];
      if (cell == WATER_CELL|| cell == PROT_CELL) return 0; //cell already hit

      //plr_2->board->hit_cells[hit_cnt] = line*10+col;
      plr_2->board->board_mat[line][col] = WATER_CELL;

      //hit_cnt++
      return 1;
  }
  else if (val == 2){ //boat

      int col = (cur->x)/50 -1;
      int line = (cur->y)/50 -1;

      int cell = plr_2->board->board_mat[line][col];
      if (cell == FIRE_CELL) return 0; //cell already hit

      //plr_2->board->hit_boat_pos[boat_hit_cnt] = line*10 + col;
      plr_2->boats[cell-1]->hit_points--;
      plr_2->board->board_mat[line][col] = FIRE_CELL;

      if (!plr_2->boats[cell-1]->hit_points){
          set_protected_cells(plr_2->board, plr_2->boats[cell-1]);
          plr_2->destroyed_boats++;
      }

      //boat_hit_cnt++;
      return 1;
      
  }
  return 0;

}
