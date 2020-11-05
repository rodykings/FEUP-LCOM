#include <lcom/lcf.h>
#include "boat.h"

Boat *create_boat(xpm_map_t boat_img, xpm_map_t boat_img_rev, int x, int y, unsigned int size, int id){

  Boat *b = (Boat*) malloc(sizeof(Boat));
  
  b->id = id;

  //boat sprite with default orientation
  b->sp = create_sprite(boat_img,x,y,0,0);

  b->xi = b->sp->x;
  b->yi = b->sp->y;
  
  b->def = b->sp->map;  //assigning default orientation to boat map

  xpm_image_t img;
  b->rev = xpm_load(boat_img_rev, XPM_8_8_8, &img); //reverse orientation for boat map rev

  //boat size in matrix squares
  b->size = size;
  b->hit_points = size;

  b->up = true;
  
  //current boat position in matrix; -1 -> out of matrix
  b->matrix_pos[0] = -1;
  b->matrix_pos[1] = -1;
  

  return b;
  
}

void rotate_boat(Boat *b){
  
  //up to down position
  if (b->up){
    b->up = false;
    b->sp->map = b->rev;

    int w = b->sp->width;
    b->sp->width = b->sp->height;
    b->sp->height = w;
  }
  else{ //down to up position
    b->up = true;
    b->sp->map = b->def;

    int w = b->sp->width;
    b->sp->width = b->sp->height;
    b->sp->height = w;
  }

}

void destroy_boat(Boat *boat) {

  if( boat == NULL )
    return;
  if( boat->def )
    free(boat->def);

  if (boat->rev)
    free(boat->rev);
  
  free(boat);
  boat = NULL; 
}





