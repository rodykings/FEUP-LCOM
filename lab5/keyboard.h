//
// Created by rodykings on 14/10/19.
//

#include "i8024.h"
#include <stdint.h>

#ifndef LAB5_KEYBOARD_H
#define LAB5_KEYBOARD_H

void (kbc_ih)();
void (print_scanCode)();
void (enable_kbd_interrupts)();
void (waitForEsq)();

#endif //LAB5_KEYBOARD_H
