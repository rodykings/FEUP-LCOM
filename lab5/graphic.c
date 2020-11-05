
#include <lcom/lcf.h>

#include <lcom/lab5.h>
#include "graphic.h"

	uint8_t *mem;
	char *video_mem;
	unsigned h_res;	        /* Horizontal resolution in pixels */
	unsigned v_res;	        /* Vertical resolution in pixels */
	unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
	
	//RED
	uint8_t red_mask_size;
	uint8_t red_field_position;

	//GREEN
	uint8_t green_mask_size;
	uint8_t green_field_position;

	//BLUE
	uint8_t blue_mask_size;
	uint8_t blue_field_position;

void* (vg_init)(uint16_t mode){
	/* Set default text mode */

	vbe_mode_info_t	vmi_p;

	vbe_get_mode_info(mode, &vmi_p); 	

	h_res = vmi_p.XResolution;
	v_res = vmi_p.YResolution;
	bits_per_pixel = vmi_p.BitsPerPixel;
	
	red_mask_size = vmi_p.RedMaskSize;
	red_field_position = vmi_p.RedFieldPosition;

	green_mask_size = vmi_p.GreenMaskSize;
	green_field_position = vmi_p.GreenFieldPosition;

	blue_mask_size = vmi_p.BlueMaskSize;
	blue_field_position = vmi_p.BlueFieldPosition;

		/* Process (virtual) address to which VRAM is mapped */
        /* frame-buffer VM address 
					   (static global variable*/

	int add = 0; 
	if(bits_per_pixel % 8 == 1){
		add = 1;
	}
	uint32_t size = h_res * v_res * (bits_per_pixel / 8 + add);

	struct minix_mem_range mr;
	unsigned int vram_base = vmi_p.PhysBasePtr;  /* VRAM's physical addresss */
	unsigned int vram_size = size;  /* VRAM's size, but you can use
					    the frame-buffer size, instead */
	int r;				    

	/* Use VBE function 0x01 to initialize vram_base and vram_size */

	/* Allow memory mapping */

	mr.mr_base = (phys_bytes) vram_base;	
	mr.mr_limit = mr.mr_base + vram_size;  

	if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
	   panic("sys_privctl (ADD_MEM) failed: %d\n", r);

	/* Map memory */

	video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

	if(video_mem == MAP_FAILED)
	   panic("couldn't map video memory");

	struct reg86 r86;

	  memset(&r86, 0, sizeof(r86));

	  r86.ax = 0x4F02; // VBE call, function 02 -- set VBE mode
	  r86.bx = 1<<14|mode; // set bit 14: linear framebuffer
	  r86.intno = 0x10;
	  if( sys_int86(&r86) != OK ){
	  	printf("set_vbe_mode: sys_int86() failed \n");
	  	return NULL;
	  }

	return video_mem;
} 	

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){

	int add = 0; 
	if(bits_per_pixel % 8 == 1){
		add = 1;
	}
	
	//printf("%d\n", (bits_per_pixel/8+add));
	uint32_t initial = (h_res* y + x)*(bits_per_pixel/8+add);
    uint32_t max_x = initial + len*(bits_per_pixel/8+add);

    uint32_t aux_color = color;

    for(uint32_t i = initial; i < max_x; i+=(bits_per_pixel/8+add)){
    	aux_color = color;
      	for(uint32_t j=0; j < bits_per_pixel/8+add; j++){
      		//printf("%d\n", color);
			video_mem[i+j] = (uint8_t) aux_color;
			aux_color >>= 8;
		}
	}
    return 0;
 }
    
 




int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){

	while(height != 0){
		vg_draw_hline(x, y, width, color);
		y++;
		height--;
	}
	

	return 0;
}

int (vg_draw_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step){
 	
 	//uint16_t x_rec = 0;
 	//uint16_t y_rec = 0;

 	uint16_t width = h_res/no_rectangles;
 	uint16_t height = v_res/no_rectangles;


		//printf("%d %d %d %d %d %d \n",i, x_rec, y_rec, width, height, color);
 		uint32_t color = first;
		for(int i=0; i<no_rectangles; i++){
			for(int j=0; j<no_rectangles; j++){
					color = setColor(mode, i, j, no_rectangles, first, step);
 					vg_draw_rectangle(j*width, i*height, width, height, color);
 					
 					
				}
		}
				
 		//printf("finish\n", x_rec, y_rec, width, height, color);
 		//x_rec+=width;
 		//color = 5;

 	return 0;


}

uint32_t setColor(uint16_t mode, uint16_t row, uint16_t col, uint8_t no_rectangles, uint32_t first, uint8_t step){
	


	if (mode == 0x105){
		return (first + (row * no_rectangles + col) * step) % (1 << bits_per_pixel);
	}
	else{

		int add = 0; 
		if(bits_per_pixel % 8 == 1){
			add = 1;
		}

		uint8_t red = (first >> red_field_position) & BIT(red_mask_size); 
		red += (col*step);
		uint8_t green = (first >> green_field_position) & (BIT(green_mask_size)-1); 
		green += (row*step);
		uint8_t blue = (first >> blue_field_position) & (BIT(blue_mask_size)-1); 
		blue += ((col+row)*step);
	 
		uint32_t color = red << red_field_position | green << green_field_position | blue << blue_field_position;
		return color;
	}
}


