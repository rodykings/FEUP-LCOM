#include <lcom/lcf.h>
#include "state_machine.h"

int select_boat_sm(Board *board, Boat *boats[10], Sprite *cur){
    
    static enum sel_boat_st st = INIT;

    static int b_index = -1;

    struct interrupt_data d = get_int_data();

    int ret = 0;
    switch(st){
        
        case INIT:

            if (d.lb){ //left button pressed
            
                b_index = check_board_selection(cur, boats);   //check wich boat has been selected

                if (b_index != -1){
                    st = DRAG;
                }   
            }
            return ret;

        case DRAG:

            if (!d.lb){
                st = DROP;
            }
            if (d.kbd && d.scancode == 0x39){
                st = ROT;
            }
            else{
                boats[b_index]->sp->x += d.delta_x;
                boats[b_index]->sp->y += d.delta_y;
            }

            return ret;
        case ROT:

                rotate_boat(boats[b_index]);
                cur->x = boats[b_index]->sp->x + boats[b_index]->sp->width/2;
                cur->y = boats[b_index]->sp->y + boats[b_index]->sp->height/2;
                boats[b_index]->sp->y = cur->y;
                st = DRAG;

            return ret;

        case DROP:
            
            if (place_boat(boats[b_index],board)){
                b_index = -1;
                ret = 1;
            }
            else{

                if (boats[b_index]->matrix_pos[0] != -1){
                    
                    remove_boat_from_matrix(boats[b_index], board);
                    ret = -1;
                
                }

                boats[b_index]->sp->x = boats[b_index]->xi;
                boats[b_index]->sp->y = boats[b_index]->yi;

                boats[b_index]->matrix_pos[0] = -1;
                boats[b_index]->matrix_pos[1] = -1;
            }

            st = INIT;
            return ret;

    }
    //printf("%d\n", st);
}

int game_round_ai_sm(Player *ai, Player *plr_1){

    static enum fire_ai st = RANDOM;
    static int line;
    static int col;
    int val;
    int cell;

    //static int hit_cnt = 0;
    //static int boat_hit_cnt = 0;

    switch (st){

        case RANDOM:
            line = rand() % 10;
            col = rand() % 10;

                val = check_matrix_collision_cord(plr_1->board,line,col);

                if(val == 0 || val == 3){
                    return 0; //hit place
                }
                else if (val == 1){  //water
                    plr_1->board->board_mat[line][col] = WATER_CELL;

                    st = RANDOM;
                    return 1;
                }
                else if (val == 2){ //boat

                    cell = plr_1->board->board_mat[line][col];

                    //plr_2->board->hit_boat_pos[boat_hit_cnt] = line*10 + col;
                    plr_1->boats[cell-1]->hit_points--;
                    plr_1->board->board_mat[line][col] = FIRE_CELL;

                    if (!plr_1->boats[cell-1]->hit_points){
                        set_protected_cells(plr_1->board, plr_1->boats[cell-1]);
                        plr_1->destroyed_boats++;
                        st = RANDOM;
                        return 1;
                    }
                    
                    col++;
                    st = MOV_R;
                    return 1;
                    
                }

            break;

        case MOV_R:

                if(col > 9){
                    col-=2;
                    st = MOV_L;
                    return 0;
                }

                val = check_matrix_collision_cord(plr_1->board,line,col);

                if(val == 0 || val == 3){
                    col-=2;
                    st = MOV_L;
                    return 0; //hit place
                }
                else if (val == 1){  //water

                    plr_1->board->board_mat[line][col] = WATER_CELL;

                    col-=2;
                    st = MOV_L;
                    return 1;
                }
                else if (val == 2){ //boat

                    cell = plr_1->board->board_mat[line][col];

                    plr_1->boats[cell-1]->hit_points--;
                    plr_1->board->board_mat[line][col] = FIRE_CELL;

                    if (!plr_1->boats[cell-1]->hit_points){
                        set_protected_cells(plr_1->board, plr_1->boats[cell-1]);
                        plr_1->destroyed_boats++;
                        st = RANDOM;
                        return 1;
                    }
                    
                    col++;
                    st = MOV_R;
                    return 1;
                    
                }

            break;

        case MOV_L:

            if(col < 0){
                col++;
                line--;
                st = MOV_U;
                return 0;
            }

            val = check_matrix_collision_cord(plr_1->board,line,col);

                if(val == 0){
                    col++;
                    line--;
                    st = MOV_U;
                    return 0; //hit place
                }
                if(val == 3){ //fire cell - special case for larger boats
                    col--;
                    return 0;
                }
                else if (val == 1){  //water

                    plr_1->board->board_mat[line][col] = WATER_CELL;

                    col++;
                    line--;
                    st = MOV_U;
                    return 1;
                }
                else if (val == 2){ //boat

                    cell = plr_1->board->board_mat[line][col];

                    //plr_2->board->hit_boat_pos[boat_hit_cnt] = line*10 + col;
                    plr_1->boats[cell-1]->hit_points--;
                    plr_1->board->board_mat[line][col] = FIRE_CELL;

                    if (!plr_1->boats[cell-1]->hit_points){
                        set_protected_cells(plr_1->board, plr_1->boats[cell-1]);
                        plr_1->destroyed_boats++;
                        st = RANDOM;
                        return 1;
                    }
                    
                    //hit = true;
                    col--;
                    st = MOV_L;
                    return 1;
                    
                }
            
            break;

        case MOV_U:

            if(line < 0){
                line+=2;
                st = MOV_D;
                return 0;
            }

            val = check_matrix_collision_cord(plr_1->board,line,col);

                if(val == 0 || val == 3){
                    line+=2;
                    st = MOV_D;
                    return 0; //hit place
                }
                else if (val == 1){  //water
                    
                    plr_1->board->board_mat[line][col] = WATER_CELL;

                    line+=2;
                    st = MOV_D;
                    return 1;
                }
                else if (val == 2){ //boat

                    cell = plr_1->board->board_mat[line][col];

                    //plr_2->board->hit_boat_pos[boat_hit_cnt] = line*10 + col;
                    plr_1->boats[cell-1]->hit_points--;
                    plr_1->board->board_mat[line][col] = FIRE_CELL;

                    if (!plr_1->boats[cell-1]->hit_points){
                        set_protected_cells(plr_1->board, plr_1->boats[cell-1]);
                        plr_1->destroyed_boats++;
                        st = RANDOM;
                        return 1;
                    }
                    
                    line--;
                    st = MOV_U;
                    return 1;
                    
                }
            
            break;

        case MOV_D:

            if(line > 9){
                st = RANDOM;
                return 0;
            }

            val = check_matrix_collision_cord(plr_1->board,line,col);

                if(val == 0 || val == 3){
                    line++;
                    return 0; //hit place
                }
                
                if (val == 2){ //boat

                    cell = plr_1->board->board_mat[line][col];
                

                    //plr_2->board->hit_boat_pos[boat_hit_cnt] = line*10 + col;
                    plr_1->boats[cell-1]->hit_points--;
                    plr_1->board->board_mat[line][col] = FIRE_CELL;

                    if (!plr_1->boats[cell-1]->hit_points){
                        set_protected_cells(plr_1->board, plr_1->boats[cell-1]);
                        plr_1->destroyed_boats++;
                        st = RANDOM;
                        return 1;
                    }
                    
                    line++;
                    st = MOV_D;
                    return 1;
                    
                }
            
            
            break;
    }
    return 0;
}


