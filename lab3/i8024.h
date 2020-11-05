//
// Created by rodykings on 14/10/19.
//

#ifndef LAB3_I8024_H
#define LAB3_I8024_H

#define KBC_IRQ 1 //keyboard irq code
#define KBC_STAT_REG 0x64 //keyboard status register
#define OUT_BUF 0x60 //output buffer register
#define SCANCODE_SIZE 1
#define WAIT_KBC 2000
#define OBF BIT(0)
#define PAR_ERR BIT(7)
#define TO_ERR (BIT(6)|BIT(5))

#define TIMER0_IRQ 0
#define TIMER_0    0x40 /**< @brief Timer 0 count register */
#define TIMER_1    0x41 /**< @brief Timer 1 count register */
#define TIMER_2    0x42 /**< @brief Timer 2 count register */
#define TIMER_CTRL 0x43 /**< @brief Control register */




#endif //LAB3_I8024_H
