#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>


//IMAGES RTC
#include "img/rtc_0.h"
#include "img/rtc_1.h"
#include "img/rtc_2.h"
#include "img/rtc_3.h"
#include "img/rtc_4.h"
#include "img/rtc_5.h"
#include "img/rtc_6.h"
#include "img/rtc_7.h"
#include "img/rtc_8.h"
#include "img/rtc_9.h"
#include "img/rtc_dots.h"

#include "graphic.h"
#define RTC_IRQ 8
#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71

void readTime();
uint32_t readHour();
uint32_t readMinutes();
uint32_t readSeconds();
void drawGraphicClock(char *buffer);
uint32_t BCDToDecimal(uint32_t BCD);


