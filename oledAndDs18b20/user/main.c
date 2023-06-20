#include <STDIO.H>

#include "ds18b20.h"
#include "oled.h"
void main() {
    u8 i = 0;
    int tmp_val, a, b;
    u8 tmp_buf[5];
    OLED_Init();
    ds18b20_init();
    delay_ms(1500);
    OLED_Clear();

    OLED_ShowChinese(0, 2, 0, 16);    // 当
    OLED_ShowChinese(16, 2, 1, 16);   // 前
    OLED_ShowChinese(32, 2, 2, 16);   // 温
    OLED_ShowChinese(48, 2, 3, 16);   // 度
    OLED_ShowChinese(64, 2, 4, 16);   // :
    OLED_ShowChinese(112, 2, 5, 16);  // ℃

    while (1) {
        i++;
        if (i % 50 == 0) {
            tmp_val = ds18b20_read_temperture() * 10;
        }
        a = tmp_val % 10;
        b = tmp_val / 10;
        OLED_ShowNum(80, 2, b, 2, 16);
        OLED_ShowChar(96, 2, '.', 16);
        OLED_ShowNum(102, 2, a, 1, 16);
    }
}