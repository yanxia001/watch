#ifndef _hxy_iic_H_
#define _hxy_iic_H_
#include "driver/i2c.h"

#define IIC_NUM I2C_NUM_1  //iic总线1是屏幕
void iic_init(int i2c_gpio,int sda_gpio,int scl_gpio);
uint8_t iic_read_byte(   uint8_t a      ,       uint8_t b);
void iic_write_byte( uint8_t a      ,       uint8_t b ,  uint8_t dat);
void iic_read_bytes(   uint8_t a      ,       uint8_t b              ,uint8_t*data ,size_t len);
void iic_write_bytes(   uint8_t a    ,uint8_t*data ,size_t len);
void iic_read_data(     uint8_t a    ,uint8_t*data ,size_t len);  // 纯读，不写寄存器地址（AHT20用）
#endif
