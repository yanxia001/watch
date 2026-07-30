#include "cst816.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include <stdio.h>
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "iic.h"
/*
函数功能：cst816t触摸IC引脚初始化函数
*/
void cst816t_GPIOinit(void)
{
    //将 RST 引脚配置为 开漏输出 (Open-Drain) 并开启 内部上拉电阻。
    //rst引脚为低时将复位
	gpio_config_t touchGPIO_InitConfig = {0};
    touchGPIO_InitConfig.pin_bit_mask = 1ull << CST816T_RST_PIN;//这行代码就是把引脚号（比如 5）转换成底层驱动能看懂的“二进制地图坐标”。
    touchGPIO_InitConfig.mode = GPIO_MODE_OUTPUT_OD;// 开漏输出模式
    touchGPIO_InitConfig.pull_up_en = GPIO_PULLUP_ENABLE; // 内部上拉
    touchGPIO_InitConfig.pull_down_en = GPIO_PULLUP_DISABLE;
    ESP_ERROR_CHECK(gpio_config(&touchGPIO_InitConfig));
 
    touchGPIO_InitConfig.pin_bit_mask = 1ull << CST816T_INIT_PIN;
    touchGPIO_InitConfig.mode = GPIO_MODE_INPUT;
    touchGPIO_InitConfig.pull_up_en = GPIO_PULLUP_ENABLE;
    touchGPIO_InitConfig.pull_down_en = GPIO_PULLUP_DISABLE;
    touchGPIO_InitConfig.intr_type = GPIO_INTR_LOW_LEVEL;
    //低电平中断 (GPIO_INTR_LOW_LEVEL)：当手指触摸屏幕，CST816T 芯片会将这个引脚拉低，ESP32 检测到低电平后立即触发中断函数 cst816t_touchIRQHandler


    ESP_ERROR_CHECK(gpio_install_isr_service(ESP_INTR_FLAG_EDGE));// 安装全局中断服务
    ESP_ERROR_CHECK(gpio_config(&touchGPIO_InitConfig));
    //ESP_ERROR_CHECK(gpio_isr_handler_add(CST816T_INIT_PIN,cst816t_touchIRQHandler,(void*) CST816T_INIT_PIN)); //添加中断服务函数
    ESP_ERROR_CHECK(gpio_intr_enable(CST816T_INIT_PIN));// 使能 GPIO 中断

    iic_init(CST816T_I2C_NUM, CST816T_SDA_PIN, CST816T_SCL_PIN);


        // 3. 硬件复位序列
    CST816T_RST_PIN_RESET(); // 拉低复位脚
    delay_ms(10);            // 保持低电平 10ms
    CST816T_RST_PIN_SET();   // 拉高复位脚（结束复位）
    delay_ms(50);            // 等待芯片内部电路稳定（延时很重要）

    // 4. 禁用自动休眠，否则芯片2秒无触摸后会进入低功耗，I2C不响应
    iic_write_byte_0(CST816T_ADDR, DisAutoSleep, 0x01);
}
 
void cst_getxy(uint16_t *x,uint16_t *y)
{
    uint8_t data [4] ={0};
    iic_read_bytes_0(CST816T_ADDR,XposH,data,4);
    *x = ((data[0] & 0x0F) << 8) | data[1];
    *y = ((data[2] & 0x0F) << 8) | data[3];
}

int cst816t_get_touch_points_num()
{ 
    uint8_t a = 0;
    iic_read_bytes_0(CST816T_ADDR,FingerNum,&a,1);
    return (int)a;
}
 
int cst816t_get_shoushi()
{ 
    uint8_t a = 0;
    iic_read_bytes_0(CST816T_ADDR,GestureID,&a,1);
    return (int)a;
}
// /*
// 函数功能：触摸屏触摸中断服务函数
// 形   参：中断引脚号
// */
// void IRAM_ATTR cst816t_touchIRQHandler(void* arg) 
// {
//     int pin = (int) arg;
//     touch_IRQFlag = 1;
// }
