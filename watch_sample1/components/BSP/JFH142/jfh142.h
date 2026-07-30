#ifndef _hxy_jfh_H_
#define _hxy_jfh_H_

#include <stdint.h>

// 定义一个结构体，方便管理数据
typedef struct {
    uint8_t heart_rate;    // 心率
    uint8_t oxygen;        // 血氧
    uint8_t sys_bp;        // 收缩压 (高压)
    uint8_t dia_bp;        // 舒张压 (低压)
    uint8_t fatigue;       // 疲劳指数
    uint8_t update_flag;   // 数据更新标志位 (0=未更新, 1=有新数据)
} JFH_Sensor_Data_t;

// 全局变量声明（定义在 jfh142.c 中），加上 volatile 关键字防止编译器优化
extern volatile JFH_Sensor_Data_t jfh_data;


void jfh_init(void);
#endif
