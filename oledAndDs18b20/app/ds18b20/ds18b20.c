#include "ds18b20.h"

/*******************************************************************************

* 函 数 名 : ds18b20_init
* 函数功能 : 初始化 DS18B20 的 IO 口 DQ 同时检测 DS 的存在
* 输 入 : 无
* 输 出 : 1:不存在，0:存在
*******************************************************************************/
u8 ds18b20_init(void) {
    ds18b20_reset();
    return ds18b20_check();
}
/**
 * @brief 复位 DS18B20
 */
void ds18b20_reset(void) {
    DS18B20_PORT = 0;  // 拉低 DQ
    delay_10us(75);    // 拉低 750us
    DS18B20_PORT = 1;  // DQ=1
    delay_10us(2);     // 20US
}

/**
 * @brief 检测 DS18B20 是否存在
 * @return 1:未检测到 DS18B20 的存在，0:存在
 */
u8 ds18b20_check(void) {
    u8 time_temp = 0;
    while (DS18B20_PORT && time_temp < 20)  // 等待 DQ 为低电平
    {
        time_temp++;
        delay_10us(1);
    }
    if (time_temp >= 20)
        return 1;  // 如果超时则强制返回 1
    else
        time_temp = 0;
    while ((!DS18B20_PORT) && time_temp < 20)  // 等待 DQ 为高电平
    {
        time_temp++;
        delay_10us(1);
    }
    if (time_temp >= 20)
        return 1;  // 如果超时则强制返回 1
    return 0;
}

/**
 * @brief 从 ds18b20 得到温度值
 * @return 温度数据
 */
float ds18b20_read_temperture(void) {
    float temp;
    u8 dath = 0;
    u8 datl = 0;
    u16 value = 0;
    ds18b20_start();  // 开始转换
    ds18b20_reset();  // 复位
    ds18b20_check();
    ds18b20_write_byte(0xcc);        // SKIP ROM
    ds18b20_write_byte(0xbe);        // 读存储器
    datl = ds18b20_read_byte();      // 低字节
    dath = ds18b20_read_byte();      // 高字节
    value = (dath << 8) + datl;      // 合并为 16 位数据
    if ((value & 0xf800) == 0xf800)  // 判断符号位，负温度
    {
        value = (~value) + 1;      // 数据取反再加 1
        temp = value * (-0.0625);  // 乘以精度
    } else                         // 正温度
    {
        temp = value * 0.0625;
    }
    return temp;
}

/**
 * @name ds18b20_start
 * @brief 开始温度转换
 */
void ds18b20_start(void) {
    ds18b20_reset();           // 复位
    ds18b20_check();           // 检查 DS18B20
    ds18b20_write_byte(0xcc);  // SKIP ROM
    ds18b20_write_byte(0x44);  // 转换命令
}

/**
 * @brief 从DS18B20读取一个位
 */
u8 ds18b20_read_bit(void) {
    u8 dat = 0;
    DS18B20_PORT = 0;
    _nop_();
    _nop_();
    DS18B20_PORT = 1;
    _nop_();
    _nop_();  // 该段时间不能过长，必须在15us内读取数据
    if (DS18B20_PORT)
        dat = 1;  // 如果总线上为1则数据dat为1，否则为0
    else
        dat = 0;
    delay_10us(5);
    return dat;
}

/**
 * @brief 从DS18B20读取一个字节
 * @return 一个字节数据
 */
u8 ds18b20_read_byte(void) {
    u8 i = 0;
    u8 dat = 0;
    u8 temp = 0;

    for (i = 0; i < 8; i++)  // 循环8次，每次读取一位，且先读低位再读高位
    {
        temp = ds18b20_read_bit();
        dat = (temp << 7) | (dat >> 1);
    }
    return dat;
}

/**
 * @brief 写一个字节到DS18B20
 * @param dat:要写入的字节
 */
void ds18b20_write_byte(u8 dat) {
    u8 i = 0;
    u8 temp = 0;

    for (i = 0; i < 8; i++)  // 循环8次，每次写一位，且先写低位再写高位
    {
        temp = dat & 0x01;  // 选择低位准备写入
        dat >>= 1;          // 将次高位移到低位
        if (temp) {
            DS18B20_PORT = 0;
            _nop_();
            _nop_();
            DS18B20_PORT = 1;
            delay_10us(6);
        } else {
            DS18B20_PORT = 0;
            delay_10us(6);
            DS18B20_PORT = 1;
            _nop_();
            _nop_();
        }
    }
}