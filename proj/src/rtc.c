#include <lcom/lcf.h>

#include "rtc.h"


int hook_id;

void readTime(){

	uint32_t reg;
	sys_outb(RTC_ADDR_REG, 10);
	sys_inb(RTC_DATA_REG, &reg);
	
	while(reg & BIT(7)){
		tickdelay(micros_to_ticks(224));
		sys_outb(RTC_ADDR_REG, 10);
		sys_inb(RTC_DATA_REG, &reg);
	}

	uint32_t hour = readHour();
	uint32_t min = readMinutes();
	uint32_t sec = readSeconds();

	printf("%d : %d : %d \n", hour, min, sec);

}


void drawGraphicClock(char *buffer){

	xpm_image_t img_0;
	uint8_t* xpm_0 = xpm_load(rtc_0, XPM_8_8_8, &img_0);
	xpm_image_t img_1;
	uint8_t* xpm_1 = xpm_load(rtc_1, XPM_8_8_8, &img_1);
	xpm_image_t img_2;
	uint8_t* xpm_2 = xpm_load(rtc_2, XPM_8_8_8, &img_2);
	xpm_image_t img_3;
	uint8_t* xpm_3 = xpm_load(rtc_3, XPM_8_8_8, &img_3);
	xpm_image_t img_4;
	uint8_t* xpm_4 = xpm_load(rtc_4, XPM_8_8_8, &img_4);
	xpm_image_t img_5;
	uint8_t* xpm_5 = xpm_load(rtc_5, XPM_8_8_8, &img_5);
	xpm_image_t img_6;
	uint8_t* xpm_6 = xpm_load(rtc_6, XPM_8_8_8, &img_6);
	xpm_image_t img_7;
	uint8_t* xpm_7 = xpm_load(rtc_7, XPM_8_8_8, &img_7);
	xpm_image_t img_8;
	uint8_t* xpm_8 = xpm_load(rtc_8, XPM_8_8_8, &img_8);
	xpm_image_t img_9;
	uint8_t* xpm_9 = xpm_load(rtc_9, XPM_8_8_8, &img_9);
	xpm_image_t img_dots;
	uint8_t* xpm_dots = xpm_load(rtc_dots, XPM_8_8_8, &img_dots);

	uint32_t reg;
	sys_outb(RTC_ADDR_REG, 10);
	sys_inb(RTC_DATA_REG, &reg);
	
	while(reg & BIT(7)){
		tickdelay(micros_to_ticks(224));
		sys_outb(RTC_ADDR_REG, 10);
		sys_inb(RTC_DATA_REG, &reg);
	}

	uint32_t hour = readHour();
	uint32_t min = readMinutes();
	//uint32_t sec = readSeconds();

	uint32_t firstDigit;
	uint32_t secondDigit;

	//draw hour
	if(hour < 10){
		firstDigit = 0;
		secondDigit = hour;
	}
	else{
		secondDigit = hour % 10;
		firstDigit = (hour / 10) % 10;
	}

	switch(firstDigit){
			case 0:
			draw_xpm(xpm_0, 20, 560, 20, 30, buffer);
			break;
			case 1:
			draw_xpm(xpm_1, 20, 560, 20, 30, buffer);
			break;
			case 2:
			draw_xpm(xpm_2, 20, 560, 20, 30, buffer);
			break;
			case 3:
			draw_xpm(xpm_3, 20, 560, 20, 30, buffer);
			break;
			case 4:
			draw_xpm(xpm_4, 20, 560, 20, 30, buffer);
			break;
			case 5:
			draw_xpm(xpm_5, 20, 560, 20, 30, buffer);
			break;
			case 6:
			draw_xpm(xpm_6, 20, 560, 20, 30, buffer);
			break;
			case 7:
			draw_xpm(xpm_7, 20, 560, 20, 30, buffer);
			break;
			case 8:
			draw_xpm(xpm_8, 20, 560, 20, 30, buffer);
			break;
			case 9:
			draw_xpm(xpm_9, 20, 560, 20, 30, buffer);
			break;
	}

	switch(secondDigit){
			case 0:
			draw_xpm(xpm_0, 40, 560, 20, 30, buffer);
			break;
			case 1:
			draw_xpm(xpm_1, 40, 560, 20, 30, buffer);
			break;
			case 2:
			draw_xpm(xpm_2, 40, 560, 20, 30, buffer);
			break;
			case 3:
			draw_xpm(xpm_3, 40, 560, 20, 30, buffer);
			break;
			case 4:
			draw_xpm(xpm_4, 40, 560, 20, 30, buffer);
			break;
			case 5:
			draw_xpm(xpm_5, 40, 560, 20, 30, buffer);
			break;
			case 6:
			draw_xpm(xpm_6, 40, 560, 20, 30, buffer);
			break;
			case 7:
			draw_xpm(xpm_7, 40, 560, 20, 30, buffer);
			break;
			case 8:
			draw_xpm(xpm_8, 40, 560, 20, 30, buffer);
			break;
			case 9:
			draw_xpm(xpm_9, 40, 560, 20, 30, buffer);
			break;
	}
	
	
	//dots
	draw_xpm(xpm_dots, 60, 560, 20, 30, buffer);

	//draw hour
	if(min < 10){
		firstDigit = 0;
		secondDigit =  min;
	}
	else{
		secondDigit = min % 10;
		firstDigit = (min / 10) % 10;
	}

	switch(firstDigit){
			case 0:
			draw_xpm(xpm_0, 80, 560, 20, 30, buffer);
			break;
			case 1:
			draw_xpm(xpm_1, 80, 560, 20, 30, buffer);
			break;
			case 2:
			draw_xpm(xpm_2, 80, 560, 20, 30, buffer);
			break;
			case 3:
			draw_xpm(xpm_3, 80, 560, 20, 30, buffer);
			break;
			case 4:
			draw_xpm(xpm_4, 80, 560, 20, 30, buffer);
			break;
			case 5:
			draw_xpm(xpm_5, 80, 560, 20, 30, buffer);
			break;
			case 6:
			draw_xpm(xpm_6, 80, 560, 20, 30, buffer);
			break;
			case 7:
			draw_xpm(xpm_7, 80, 560, 20, 30, buffer);
			break;
			case 8:
			draw_xpm(xpm_8, 80, 560, 20, 30, buffer);
			break;
			case 9:
			draw_xpm(xpm_9, 80, 560, 20, 30, buffer);
			break;
	}

	switch(secondDigit){
			case 0:
			draw_xpm(xpm_0, 100, 560, 20, 30, buffer);
			break;
			case 1:
			draw_xpm(xpm_1, 100, 560, 20, 30, buffer);
			break;
			case 2:
			draw_xpm(xpm_2, 100, 560, 20, 30, buffer);
			break;
			case 3:
			draw_xpm(xpm_3, 100, 560, 20, 30, buffer);
			break;
			case 4:
			draw_xpm(xpm_4, 100, 560, 20, 30, buffer);
			break;
			case 5:
			draw_xpm(xpm_5, 100, 560, 20, 30, buffer);
			break;
			case 6:
			draw_xpm(xpm_6, 100, 560, 20, 30, buffer);
			break;
			case 7:
			draw_xpm(xpm_7, 100, 560, 20, 30, buffer);
			break;
			case 8:
			draw_xpm(xpm_8, 100, 560, 20, 30, buffer);
			break;
			case 9:
			draw_xpm(xpm_9, 100, 560, 20, 30, buffer);
			break;
	}
	


}






uint32_t readHour(){
	uint32_t hour;
	sys_outb(RTC_ADDR_REG, 4);
	sys_inb(RTC_DATA_REG, &hour);
	return BCDToDecimal(hour); 


}
uint32_t readMinutes(){
	uint32_t min;
	sys_outb(RTC_ADDR_REG, 2);
	sys_inb(RTC_DATA_REG, &min);
	return BCDToDecimal(min);
}
uint32_t readSeconds(){
	uint32_t sec;
	sys_outb(RTC_ADDR_REG, 0);
	sys_inb(RTC_DATA_REG, &sec);
	return BCDToDecimal(sec);
}

uint32_t BCDToDecimal(uint32_t BCD)
{
   return (((BCD>>4)*10) + (BCD & 0xF));
}


