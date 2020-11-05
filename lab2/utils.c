#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {

    val = val & 0x00FF;
    uint8_t val8 = (uint8_t)(val);
    *lsb = val8;

    return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {

    val = val & 0xFF00;
    val = val >> 8;
    uint8_t val8 = (uint8_t)(val);
    *msb = val8;

    return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {


    //printf("%X", port);
    //printf("UTIL: %X\n", *value);
    uint32_t value32;
    sys_inb(port, &value32);

    //printf("%X", value32);

    *value = (uint8_t)value32;


    return 0;
}
