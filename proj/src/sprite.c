#include <lcom/lcf.h>
#include "sprite.h"


Sprite* create_sprite(xpm_map_t xpm, int x, int y,int xspeed, int yspeed) {
  //allocate space for the "object"
  Sprite* sp = (Sprite*) malloc ( sizeof(Sprite));

  xpm_image_t img;
  if( sp == NULL )
    return NULL;
  
  // read the sprite pixmap 
  sp->map = xpm_load(xpm, XPM_8_8_8, &img);

  if(sp->map == NULL) 
  {
    free(sp);
    return NULL;
  }

  sp->x = x;
  sp->y = y;
  sp->width = img.width; 
  sp->height= img.height;
  sp->xspeed = xspeed;
  sp->yspeed = yspeed;
  
  return sp;
}

void destroy_sprite(Sprite*sp) {

  if( sp == NULL )
    return;
  if( sp->map )
    free(sp->map);
  
  free(sp);sp = NULL;     // XXX: pointer is passed by value//           should do this @ the caller
}


int draw_sprite(Sprite *sp, char *buff){

    if (sp->x < 0 ) sp->x = 0;
    if(sp->y < 0) sp->y = 0;

    draw_xpm(sp->map, sp->x, sp->y, sp->width, sp->height, buff);

  return 0;
}

int check_sprite_collision(Sprite *a, Sprite *b){

  if ((a->x >= b->x && a->y >= b->y) && (a->x <= b->x + b->width && a->y <= b->y + b->height)) return 1;
  return 0;

}


