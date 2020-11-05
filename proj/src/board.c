#include <lcom/lcf.h>

#include "board.h"

int check_matrix_collision(Board *board,Sprite *sp, int x_pos, int y_pos){

  if (sp->x + x_pos < BOARD_XI || sp->y + y_pos < BOARD_YI) return 0;
  if (sp->x +x_pos > BOARD_XF || sp->y + y_pos > BOARD_YF) return 0;

  int col = (sp->x + x_pos)/50 -1;
  int line = (sp->y + y_pos)/50 -1;

  int val = board->board_mat[line][col];

  if ((val >= 1 && val <= 10) || val == FIRE_CELL){
    return 2; //a boat has been hit
  }
  //add boat default positions
  
  //printf("colision %d %d\n", line, col);
  //board->board_mat[line][col] = 1;

  return 1; //matrix hit (water);
}


int check_matrix_collision_cord(Board *board, int line, int col){

  int val = board->board_mat[line][col];

  if ((val >= 1 && val <= 10)){
    return 2; //a boat has been hit
  }else if(val == PROT_CELL || val == WATER_CELL){
    return 0;
  }
  else if (val == FIRE_CELL){
    return 3;
  }
  return 1; //matrix hit (water);
}


int check_board_selection(Sprite *cur, Boat *boats[10]){

  //button 560/515  -> 782/563
  for (size_t i = 0; i < 10; i++){
    if (check_sprite_collision(cur, boats[i]->sp)) return i;
  }

  return -1;

}

void load_boats(Boat *boats[10]){

  Boat *b_1 = create_boat(boat_1, boat_1, BOAT1_XI, BOAT1_YI,1,1); //size 1
  Boat *b_2 = create_boat(boat_1, boat_1, BOAT1_XI+50, BOAT1_YI,1,2);
  Boat *b_3 = create_boat(boat_1, boat_1, BOAT1_XI+100, BOAT1_YI,1,3);
  Boat *b_4 = create_boat(boat_1, boat_1, BOAT1_XI+150, BOAT1_YI,1,4);
  Boat *b_5 = create_boat(boat_2, boat_2_hor, BOAT2_XI, BOAT2_YI, 2,5);  //size 2
  Boat *b_6 = create_boat(boat_2, boat_2_hor, BOAT2_XI+50, BOAT2_YI, 2,6);
  Boat *b_7 = create_boat(boat_2, boat_2_hor, BOAT2_XI+100, BOAT2_YI, 2,7);
  Boat *b_8 = create_boat(boat_3, boat_3_hor, BOAT3_XI, BOAT3_YI, 3,8); //size 3
  Boat *b_9 = create_boat(boat_3, boat_3_hor, BOAT3_XI+50, BOAT3_YI, 3,9);
  Boat *b_10 = create_boat(boat_4, boat_4_hor, BOAT4_XI, BOAT4_YI, 4, 10); //size 4

  boats[0] = b_1;
  boats[1] = b_2;
  boats[2] = b_3;
  boats[3] = b_4;
  boats[4] = b_5;
  boats[5] = b_6;
  boats[6] = b_7;
  boats[7] = b_8;
  boats[8] = b_9;
  boats[9] = b_10;

}

void remove_boat_from_matrix(Boat *boat, Board *board){

  int line = boat->matrix_pos[0];
  int col = boat->matrix_pos[1];

  if (boat->up){ //removing vertical boat

    for (int i = -1; i <= (int)boat->size; i++){
      
      if (line + i >= 0 && line + i <= 9){

        for (int j = -1; j < 2; j++){
          if (col + j >= 0 && col + j <= 9){
            if (board->board_mat[line+i][col+j] < 0) { //blocked cell
              board->board_mat[line+i][col+j] -= BLOCK_CELL;
            }
            else{ 
              board->board_mat[line+i][col+j] = 0; //boat cell
              }
          }
        }

      }
    } 

  }
  else{ //removing horizontal boat

  //remove body
  for (int i = -1; i < (int)boat->size; i++){

    for (int j = -1; j < 2; j++){
      if (line +j >= 0 && line+j <= 9){
        if (board->board_mat[line+j][col+i] < 0)
          board->board_mat[line+j][col+i] -= BLOCK_CELL;
        else {
          board->board_mat[line+j][col+i] = 0;
        }
      }
    }
  }

  //remove front 
  if (col + boat->size <= 9){
    for (int i = -1;i < 2; i++){
      if (line+i >= 0 && line+i <= 9) board->board_mat[line+i][col+boat->size] -= BLOCK_CELL;
    }
    
    }

  }

  boat->matrix_pos[0] = -1;
  boat->matrix_pos[1] = -1;
}

void destroy_boats(Boat *boats[10]){
  for (size_t i = 0; i < 10;i++){
    destroy_boat(boats[i]);
  }
}
void draw_boats(Boat *boats[10],char *buffer){

  for (size_t i = 0; i < 10; i++){
    draw_sprite(boats[i]->sp,buffer);
  }
}

void draw_destroyed_boats(Boat *boats[10], char *buffer){
  for (size_t i = 0; i < 10; i++){
    if (!boats[i]->hit_points) draw_sprite(boats[i]->sp,buffer);
  }
}

Board *create_board(unsigned int id){

  Board *b = (Board*) malloc(sizeof(Board));

  b->id = id;

  for (int i=0; i < 10; i++){
      for (int j = 0; j< 10; j++){
        b->board_mat[i][j] = 0;
      }
  }
  
  return b;

}

void block_adjacent_cells(Board *board, Boat *boat){

  int line = boat->matrix_pos[0];
  int col = boat->matrix_pos[1];
  int size = boat->size;


  if (boat->up){

     //top
    if (line -1 >= 0){
      for (int j = -1; j < 2; j++)
        if (col + j >= 0 && col + j <= 9) board->board_mat[line-1][col+j] += BLOCK_CELL;
    }

    //sides
    for (size_t i = 0; i < boat->size;i++){
      
      if (col - 1 >= 0) board->board_mat[line+i][col-1] += BLOCK_CELL;
      if (col + 1 <= 9) board->board_mat[line+i][col+1] += BLOCK_CELL;
    }

    //bottom
    if (line + size <= 9){
      for (int j = -1; j < 2; j++)
        if (col + j >= 0 && col + j <= 9) board->board_mat[line+size][col+j] += BLOCK_CELL;
    }

  }
  else{ //horizontal

    //front
    if (col-1 >= 0){
      for (int j = -1; j < 2; j++)
        if (line+j <= 9 && line+j >= 0) board->board_mat[line+j][col-1] += BLOCK_CELL;
    }

     //sides
    for (size_t i = 0; i < boat->size; i++){
      if (line-1 >= 0) board->board_mat[line-1][col+i] += BLOCK_CELL;
      if (line+1 <= 9) board->board_mat[line+1][col+i] += BLOCK_CELL;
    }

    //back
    if (col+size <= 9){
      for (int j = -1; j < 2; j++){
        if (line+j >= 0 && line+j <= 9) board->board_mat[line+j][col+size] += BLOCK_CELL;
      }
    }

  }

}

void set_protected_cells(Board *board, Boat *boat){

  int line = boat->matrix_pos[0];
  int col = boat->matrix_pos[1];
  int size = boat->size;


  if (boat->up){

     //top
    if (line -1 >= 0){
      for (int j = -1; j < 2; j++)
        if (col + j >= 0 && col + j <= 9) board->board_mat[line-1][col+j] = PROT_CELL;
    }

    //sides
    for (size_t i = 0; i < boat->size;i++){
      
      if (col - 1 >= 0) board->board_mat[line+i][col-1] = PROT_CELL;
      if (col + 1 <= 9) board->board_mat[line+i][col+1] = PROT_CELL;
    }

    //bottom
    if (line + size <= 9){
      for (int j = -1; j < 2; j++)
        if (col + j >= 0 && col + j <= 9) board->board_mat[line+size][col+j] = PROT_CELL;
    }

  }
  else{ //horizontal

    //front
    if (col-1 >= 0){
      for (int j = -1; j < 2; j++)
        if (line+j <= 9 && line+j >= 0) board->board_mat[line+j][col-1] = PROT_CELL;
    }

     //sides
    for (size_t i = 0; i < boat->size; i++){
      if (line-1 >= 0) board->board_mat[line-1][col+i] = PROT_CELL;
      if (line+1 <= 9) board->board_mat[line+1][col+i] = PROT_CELL;
    }

    //back
    if (col+size <= 9){
      for (int j = -1; j < 2; j++){
        if (line+j >= 0 && line+j <= 9) board->board_mat[line+j][col+size] = PROT_CELL;
      }
    }

  }
}

int place_boat(Boat *boat, Board *board){

  int x_pos, y_pos;
  if (boat->up){
    x_pos = boat->sp->width/2; //collision point ajustment
    y_pos = 1;
  }
  else{
    x_pos = 1;
    y_pos = boat->sp->height/2;
  }

  if(check_matrix_collision(board, boat->sp, x_pos,y_pos)){

      int col = (boat->sp->x + x_pos)/50 -1;
      int line = (boat->sp->y + y_pos)/50 -1;
      
      if (boat->up){ //placing boat vertically

        if (verify_boat_placement(boat,board,col,line,true)){

          if (boat->matrix_pos[0] != -1) remove_boat_from_matrix(boat,board);

          boat->matrix_pos[1] = col;  //assigning current boat position to boat object
          boat->matrix_pos[0] = line;
          
          //printf("%d %d\n", boat->matrix_pos[0], boat->matrix_pos[1]);
          for (unsigned int i = 0; i < boat->size; i++){

            board->board_mat[line+i][col] = boat->id;

          }

          block_adjacent_cells(board,boat); //fill adjacent cells with -1

          return 1; //success
        }

      }
      else{ //placing boat horizontally

        if (verify_boat_placement(boat,board,col,line,false)){
          //boat has already been placed in board (undo last placement)
          if (boat->matrix_pos[0] != -1) remove_boat_from_matrix(boat,board);

          boat->matrix_pos[1] = col; //assigning current boat position to boat object
          boat->matrix_pos[0] = line;

          for (unsigned int i = 0; i < boat->size; i++){
            board->board_mat[line][col+i] = boat->id;
          }

          block_adjacent_cells(board,boat); //fill adjacent cells with -1

          return 1; //success
        }
      }

  }

  //out of matrix - return boat to original position
  boat->sp->x = boat->xi;
  boat->sp->y = boat->yi;
  return 0;
}

int place_boat_cord(Boat *boat, Board *board, int line, int col){

    if (boat->up){ //placing boat vertically

      boat->matrix_pos[1] = col;  //assigning current boat position to boat object
      boat->matrix_pos[0] = line;
      
      for (unsigned int i = 0; i < boat->size; i++){
        board->board_mat[line+i][col] = boat->id;
      }

      block_adjacent_cells(board,boat); //fill adjacent cells with -1

      return 0;

    }
    else{ //placing boat horizontally

        boat->matrix_pos[1] = col; //assigning current boat position to boat object
        boat->matrix_pos[0] = line;

        for (unsigned int i = 0; i < boat->size; i++){
          board->board_mat[line][col+i] = boat->id;
        }

        block_adjacent_cells(board,boat); //fill adjacent cells with -1

        return 0;
    }
    return 0;
}

int verify_boat_placement(Boat* boat, Board *board, int col, int line, bool vertical){

  if (vertical){

    if (boat->size + line > 10) return 0; //boat doesn't fit (failure)

    for (unsigned int i = 0; i < boat->size; i++){
      if (board->board_mat[line+i][col] != 0) return 0; //space already occupied
    }
    return 1; //success
  }
  else{

    if (boat->size + col > 10) return 0; //boat doesn't fit (failure)

    for (unsigned int i = 0; i < boat->size; i++){
      if (board->board_mat[line][col+i] != 0) return 0;
    }
    return 1; //success
  }
  
  return 0;
}

void load_objects(Sprite *obj[3]){

  Sprite *wtr = create_sprite(water,0,0,0,0);
  Sprite *fr = create_sprite(fire,0,0,0,0);
  Sprite *prt = create_sprite(protected,0,0,0,0);

  obj[0] = wtr;
  obj[1] = fr;
  obj[2] = prt;

}

void draw_objects(Board *board, Sprite *obj[3], char *buffer){

  //water
  for (int i = 0; i < 10; i++){

    for (int j = 0; j < 10; j++){
      
      int curr = board->board_mat[i][j];
      if (curr == WATER_CELL){

        int x_pos = j * CELL_WIDTH + BOARD_XI;
        int y_pos = i * CELL_WIDTH + BOARD_YI + WATER_AJUST;

        obj[0]->x = x_pos;
        obj[0]->y = y_pos;

        draw_sprite(obj[0],buffer);
      }
      else if (curr == PROT_CELL){

        int x_pos = j * CELL_WIDTH + BOARD_XI;
        int y_pos = i * CELL_WIDTH + BOARD_YI + WATER_AJUST;

        obj[2]->x = x_pos;
        obj[2]->y = y_pos;

        draw_sprite(obj[2],buffer);

      }
      else if (curr == FIRE_CELL){
        int x_pos = j * CELL_WIDTH + BOARD_XI + 2;
        int y_pos = i * CELL_WIDTH + BOARD_YI + 7;

        obj[1]->x = x_pos;
        obj[1]->y = y_pos;

        draw_sprite(obj[1],buffer);
      }
    }
  
  }

}

void print_mat(Board*b){

  printf("\n");
  for (int i=0; i < 10; i++){
      for (int j = 0; j< 10; j++){
        if (b->board_mat[i][j] < 0 || b->board_mat[i][j] > 9) printf("%d ",b->board_mat[i][j]);
        else printf(" %d ",b->board_mat[i][j]);
      }
      printf("\n");
  }
}
