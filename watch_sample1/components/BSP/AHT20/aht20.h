#ifndef _hxy_AHT_H_
#define _hxy_AHT_H_
#include "driver/i2c.h"

// 定义数据结构体
typedef struct {
    float Hum;
    float Tem;
} AHT21_Date_t;

// 声明全局变量（定义在 aht20.c 中）
extern AHT21_Date_t AHT21_Date;
extern uint8_t AHT21_Receive_Date[7];
extern uint8_t AHT21_CRC;

// 函数声明
void AHT21_Init(void);
void AHT20_Read(void);

#endif
