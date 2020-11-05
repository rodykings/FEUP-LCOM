//
// Created by ivo_s on 27/11/2019.
//
#include <lcom/lcf.h>
#include "graphic.h"


//uint8_t *mem;

static char *video_mem;
static unsigned h_res;	        /* Horizontal resolution in pixels */
static unsigned v_res;	        /* Vertical resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

char *get_video_mem() {return video_mem;}
unsigned get_h_res() {return h_res;}
unsigned get_v_res() {return v_res;}
unsigned get_bytes_per_pixel() {return bits_per_pixel/8 + (bits_per_pixel % 8 != 0);}


void* (vg_init)(uint16_t mode){
    /* Set default text mode */

    vbe_mode_info_t	vmi_p;

    vbe_get_mode_info(mode, &vmi_p);

    h_res = vmi_p.XResolution;
    v_res = vmi_p.YResolution;
    bits_per_pixel = vmi_p.BitsPerPixel;

    /* Process (virtual) address to which VRAM is mapped */
    /* frame-buffer VM address
                   (static global variable*/


    unsigned bytes_per_pixel = bits_per_pixel/8 + (bits_per_pixel % 8 != 0);

    uint32_t size = h_res * v_res * bytes_per_pixel;

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


void draw_xpm(uint8_t *pixmap, uint16_t x, uint16_t y, uint32_t width, uint32_t height, char*buff){

    unsigned bytes_per_pixel = get_bytes_per_pixel();

    for (size_t h = 0; h < height; h++){

        for(uint32_t w = 0; w < width; w++){

            uint32_t byte_pos = (h*width + w)*bytes_per_pixel;  //first color byte

            draw_pixel(w+x,h+y,pixmap,byte_pos,buff);
        }
    }
}

int draw_pixel(uint32_t x, uint32_t y, uint8_t* pixmap, uint32_t byte_pos, char* buff){

    if (x > h_res || y > v_res) return 1;

    unsigned bytes_per_pixel = get_bytes_per_pixel();
    uint32_t pos = (h_res* y + x)*bytes_per_pixel;

    uint32_t color = 0x000000;
    if (color == TRANSPARENCY_COLOR) return 1;

    //fetching color field from the pixmap
    uint32_t byte_0 = pixmap[byte_pos];
    uint32_t byte_1 = pixmap[byte_pos+1];
    byte_1 <<= 8;
    uint32_t byte_2 = pixmap[byte_pos+2];
    byte_2 <<= 16;

    color = (color | (byte_2|byte_1|byte_0));

    if (color == TRANSPARENCY_COLOR) return 1;  //don't draw
    
    //set pixel
    for (uint32_t i = 0;i < bytes_per_pixel; i++){
        //color = pixmap[byte_pos+i];
        buff[pos+i] = (uint8_t) color;
        color >>=8;
    }

    return 0;

}

