#include <lcom/lcf.h>

#include "game.h"

static bool done = false;


int game_sp(char *buffer){

    Player *plr1 = createPlayer();
    Player *ai= createPlayer();

    xpm_image_t btn_img;
    uint8_t *btn_xpm =xpm_load(btn_done, XPM_8_8_8, &btn_img);
    uint8_t *btn_hover_xpm =xpm_load(btn_done_hover, XPM_8_8_8, &btn_img);

    Sprite *btn = create_sprite(btn_done,560,520,0,0);
    game_init_board(plr1, buffer,btn, btn_xpm, btn_hover_xpm);
    print_mat(plr1->board);

    //game_init_board(plr2, buffer);
    //print_mat(plr2->board);

    generate_random_matrix(ai);
    print_mat(ai->board);

    xpm_image_t img;
    uint8_t *back = xpm_load(background, XPM_8_8_8, &img);

    Sprite *cur = create_sprite(cursor,300,400,0,0);

    Sprite *obj[3];
    load_objects(obj);

    xpm_image_t img1;
    uint8_t *ply_mark =xpm_load(player_mark, XPM_8_8_8, &img1);

    xpm_image_t img2;
    uint8_t *pc_mark =xpm_load(ai_mark, XPM_8_8_8, &img2);

    char *board_obj = (char*) malloc(800*600*3);

    while(!done){
      //printf("player1\n");
      game_round_sp(plr1,ai,back,cur,obj,buffer,board_obj, ply_mark);
      idle_time(IDLE_T_2);

      //printf("ai\n");
      if (!done){
        game_round_ai(ai,plr1,back,obj,buffer, pc_mark);
        idle_time(IDLE_T_2);
      }
    }

    free(board_obj);

  return 0;
}

int game_mp(char *buffer){

    Player *plr1 = createPlayer();
    Player *plr2= createPlayer();

    xpm_image_t btn_img;
    uint8_t *btn_xpm =xpm_load(btn_done, XPM_8_8_8, &btn_img);
    uint8_t *btn_hover_xpm =xpm_load(btn_done_hover, XPM_8_8_8, &btn_img);

    Sprite *btn = create_sprite(btn_done,560,520,0,0);
    game_init_board(plr1, buffer,btn, btn_xpm, btn_hover_xpm);
    xpm_image_t img1;
    uint8_t *ply_mark =xpm_load(player_mark, XPM_8_8_8, &img1);


    print_mat(plr1->board);

    game_init_board(plr2, buffer,btn, btn_xpm, btn_hover_xpm);
    print_mat(plr2->board);

    xpm_image_t img;
    uint8_t *back = xpm_load(background, XPM_8_8_8, &img);

    Sprite *cur = create_sprite(cursor,300,400,0,0);

    Sprite *obj[4];
    load_objects(obj);

    char *board_obj = (char*) malloc(800*600*3);

    while(!done){
      //printf("player1\n");
      game_round_sp(plr1,plr2,back,cur,obj,buffer,board_obj,ply_mark);
      idle_time(IDLE_T_2);

      //printf("ai\n");
      if (!done){
        game_round_sp(plr2,plr1,back,cur,obj,buffer,board_obj,ply_mark);
        idle_time(IDLE_T_2);
      }
    }

  free(board_obj);

  return 0;
}



int game_init_board(Player *plr, char *buffer, Sprite *btn, uint8_t *btn_done, uint8_t *btn_hover){

  xpm_image_t img;
  uint8_t *back = xpm_load(background2, XPM_8_8_8, &img);

  Sprite *cur = create_sprite(cursor,300,400,0,0);

  load_boats(plr->boats);


  bool end = false;
  int placed_boats = 0;

  while (!end){
    interrupt_cicle();

    struct interrupt_data d = get_int_data();

    if (d.timer){

      memcpy(buffer,back,800*600*3);
      
      draw_boats(plr->boats,buffer);
      draw_sprite(btn,buffer);

      draw_sprite(cur, buffer);

      memcpy(get_video_mem(), buffer, 800*600*3);
    }
    if (d.mouse){
      
      placed_boats += select_boat_sm(plr->board,plr->boats,cur);

      if (check_sprite_collision(cur, btn)){
        btn->map = btn_hover;

        if (d.lb && (placed_boats == 10)){
            end = true;
        }
      }
      else{
        btn->map = btn_done;
      }
      
      cur->x += d.delta_x;
      cur->y += d.delta_y;

    }
    
    if (d.kbd){
      
      select_boat_sm(plr->board,plr->boats,cur);
      //end = (d.scancode == 0x81); //debug

    }

  }

  return 0;
}


int game_round_sp(Player *plr_1, Player *plr_2,uint8_t *back, Sprite *cur,Sprite *obj[5], char *buffer, char *board_obj, uint8_t* mark){

  bool shot = false;

  //copy the current state of the board to aux buffer
  memcpy(board_obj,back,800*600*3);
  draw_objects(plr_2->board,obj,board_obj);
  draw_destroyed_boats(plr_2->boats,board_obj);
  draw_xpm(mark,600,150,150, 250, board_obj);

  while (!shot){

    interrupt_cicle();

    struct interrupt_data d = get_int_data();

    if (d.timer){

      memcpy(buffer,board_obj,800*600*3);

      draw_sprite(cur, buffer);

      memcpy(get_video_mem(), buffer, 800*600*3);

    }
    if (d.mouse){
      
      cur->x += d.delta_x;
      cur->y += d.delta_y;

      if (d.lb) shot = player_shot(plr_1,plr_2,cur);


    }
    
    done = (plr_2->destroyed_boats == 10);
    if (done) return winner_sp();

    if (d.kbd){

      shot = (get_scancode() == 0x81);
      done = shot;
    }

  }

  //refresh screen
  draw_objects(plr_2->board,obj,board_obj);
  draw_destroyed_boats(plr_2->boats,board_obj);
  draw_xpm(mark,600,150,150, 250, board_obj);
  memcpy(get_video_mem(), board_obj, 800*600*3);
  idle_time(IDLE_T_1);

  return 0;
}


int game_round_ai(Player *ai, Player *plr_1,uint8_t *back, Sprite *obj[5], char *buffer, uint8_t* mark){

  bool shot = false;

  //draw player objects
  memcpy(buffer,back,800*600*3);
  draw_objects(plr_1->board,obj,buffer);
  draw_boats(plr_1->boats, buffer);
  draw_xpm(mark,600,150,150,250, buffer);
  memcpy(get_video_mem(), buffer, 800*600*3);
    
  while (!shot){
   
    shot = game_round_ai_sm(ai, plr_1);

    done = (plr_1->destroyed_boats == 10);
    if (done) return game_over_sp();

  }

  //refresh screen
  idle_time(IDLE_T_1);
  memcpy(buffer,back,800*600*3);
  draw_objects(plr_1->board,obj,buffer);
  draw_boats(plr_1->boats, buffer);
  draw_xpm(mark,600,150,150,250, buffer);
  memcpy(get_video_mem(), buffer, 800*600*3);

 return 0;
}


void idle_time(int idle){

  int cnt = idle;
  int timer_int = 0;

  struct interrupt_data d = get_int_data();
  
  while (cnt){
    interrupt_cicle();
    if (d.timer) timer_int = get_timer_counter();
    if (timer_int % 60 == 0) cnt--;
  }
}

int game_over_sp(){
    xpm_image_t game_over_img;
    uint8_t *go_img =xpm_load(game_over, XPM_8_8_8, &game_over_img);
    draw_xpm(go_img, 0, 0, 800, 600, get_video_mem());
    idle_time(10);
    return 0;
}

int winner_sp(){
    xpm_image_t game_over_img;
    uint8_t *wn_img =xpm_load(winner, XPM_8_8_8, &game_over_img);
    draw_xpm(wn_img,  0, 0, 800, 600, get_video_mem());
    idle_time(10);
    return 0;
}
