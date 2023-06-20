#include "public.h"

/**
 * @brief 延时函数，ten_us=1时，大约延时10us
 */
void delay_10us(u16 ten_us) {
    while (ten_us--) {
        ;
    }
}

/**
 * @brief 延时函数，ten_ms=1时，大约延时10ms
 */
void delay_ms(u16 ms) {
    u16 i, j;
    for (i = ms; i > 0; i--)
        for (j = 110; j > 0; j--)
            ;
}