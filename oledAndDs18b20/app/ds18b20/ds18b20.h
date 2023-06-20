#ifndef _ds18b20_H
#define _ds18b20_H

#include "public.h"

// 管脚定义
sbit DS18B20_PORT = P3 ^ 7;

// 函数声明

u8 ds18b20_init(void);
void ds18b20_reset(void);
u8 ds18b20_check(void);
float ds18b20_read_temperture(void);
void ds18b20_start(void);
u8 ds18b20_read_bit(void);
u8 ds18b20_read_byte(void);
void ds18b20_write_byte(u8 dat);

#endif