#include <lcom/lcf.h>
#include "menu.h"


int menu(char *buffer){

    xpm_image_t img;
    uint8_t *menu =xpm_load(m, XPM_8_8_8, &img);


    xpm_image_t btn1_img;
    uint8_t *btn1_xpm =xpm_load(btn_sp, XPM_8_8_8, &btn1_img);
    xpm_image_t btn1h_img;
    uint8_t *btn1_hover =xpm_load(btn_sp_hover, XPM_8_8_8, &btn1h_img);

    
    xpm_image_t btn2_img;
    uint8_t *btn2_xpm =xpm_load(btn_mp, XPM_8_8_8, &btn2_img);
    xpm_image_t btn2h_img;
    uint8_t *btn2_hover =xpm_load(btn_mp_hover, XPM_8_8_8, &btn2h_img);

    xpm_image_t btn3_img;
    uint8_t *btn3_xpm =xpm_load(btn_ex, XPM_8_8_8, &btn3_img);
    xpm_image_t btn3h_img;
    uint8_t *btn3_hover =xpm_load(btn_ex_hover, XPM_8_8_8, &btn3h_img);


    Sprite *cur = create_sprite(cursor,400,250,0,0);
    Sprite *btn1 = create_sprite(btn_sp,275,250,0,0);
    Sprite *btn2 = create_sprite(btn_mp,275,340,0,0);
    Sprite *btn3 = create_sprite(btn_ex,275,450,0,0);
    

  bool end = false;

  while (!end){
    interrupt_cicle();

    struct interrupt_data d = get_int_data();

    if (d.timer){
      //draw_xpm(menu,0,0,img.width,img.height,buffer);
      memcpy(buffer,menu,800*600*3);
      draw_sprite(btn1, buffer);
      draw_sprite(btn2, buffer);
      draw_sprite(btn3, buffer);
      draw_sprite(cur, buffer);
      drawGraphicClock(buffer);
      

      memcpy(get_video_mem(), buffer, 800*600*3);
    }
    
    if (d.mouse){
    
      if (check_sprite_collision(cur, btn1)){ //single player
        btn1->map = btn1_hover;
        if(d.lb)
          return 1;

      }
      else{
        btn1->map = btn1_xpm;
      }
      if (check_sprite_collision(cur, btn2)){ //multiplayer

        btn2->map = btn2_xpm;
        if(d.lb){
          return 2;

        } 
      }else{          
          btn2->map = btn2_hover;
      }    

      if (check_sprite_collision(cur, btn3)){ //exit

        btn3->map = btn3_xpm;
        if(d.lb){
            return 3;
        } 
      }else{
          btn3->map = btn3_hover;
      }     

      cur->x += d.delta_x;
      cur->y += d.delta_y;

    }
  }
  return 0;
}

int select_menu_opt(int opt, char* buffer){

  switch (opt){
    case 1:
      return game_sp(buffer);
    case 2:
      return game_mp(buffer);
    case 3:
      return 1;
    default:
      return 0;
  }

  return 0;
}

int game_main_cicle(){

  bool exit = false;
  int opt = 0;

  vg_init(VIDEO_MODE);
  char *buffer = (char*) malloc(800*600*3);

  enable_components(); //enable interrupts

  while (!exit){

    opt = menu(buffer);
    exit = select_menu_opt(opt, buffer);

  }

  disable_components(); //disable interrupts
  free(buffer);
  vg_exit();

  return 0;
}


