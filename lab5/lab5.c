// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>
#include "graphic.h"
#include "keyboard.h"


extern uint8_t *mem;
extern char *video_mem;
extern unsigned h_res;         /* Horizontal resolution in pixels */
extern unsigned v_res;         /* Vertical resolution in pixels */
extern unsigned bits_per_pixel; /* Number of VRAM bits per pixel */


// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int(video_test_init)(uint16_t mode, uint8_t delay){
  
  vg_init(mode);

  sleep(delay);
  //TEXT MODE
  vg_exit();
  return 0;
}

 


int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
    
    
  mem = (uint8_t*)vg_init(mode);

  vg_draw_rectangle(x, y, width, height, color);

  waitForEsq();
    
  //TEXT MODE
  vg_exit();

  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  

  mem = (uint8_t*)vg_init(mode);

  vg_draw_pattern(mode, no_rectangles, first, step);

  waitForEsq();
    
  //TEXT MODE
  vg_exit();

  

  return 1;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {

  mem = (uint8_t*)vg_init(0x105);

  enum xpm_image_type type = XPM_INDEXED;
  xpm_image_t img;
  xpm_load(xpm, type, &img);

    int add = 0;
    if(bits_per_pixel % 8 == 1){
        add = 1;
    }
  uint32_t initial = (h_res* y + x)*(bits_per_pixel/8+add);
  uint32_t max_x = initial + img.width*(bits_per_pixel/8+add);

  uint32_t color;
  uint32_t img_x = 0;
  //uint32_t img_y = 0;

  uint32_t width = img.width;
  uint32_t height = img.height;

  for (size_t h = 0; h <= height; h++){
      for(uint32_t i = initial; i < max_x; i+=(bits_per_pixel/8+add)){
          for(uint32_t j=0; j < bits_per_pixel/8+add; j++) {
              //printf("%d\n", color);
              uint32_t pixel_pos = h*width + img_x;
              color = img.bytes[pixel_pos];
              video_mem[i+j] = (uint8_t) color;
              color >>= 8;
          }
          img_x++;
      }
      y++;
      initial = (h_res* y + x)*(bits_per_pixel/8+add);
      max_x = initial + img.width*(bits_per_pixel/8+add);
      img_x = 0;
  }

  waitForEsq();
    
  //TEXT MODE
  vg_exit();

  return 0;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  /* To be completed */
  printf("%s(%8p, %u, %u, %u, %u, %d, %u): under construction\n",
         __func__, xpm, xi, yi, xf, yf, speed, fr_rate);

  return 1;
}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}





