#include "led.h"
#include "driver/gpio.h"

// 初始化GPIO5为输出模式
void gpio_init() {
    gpio_reset_pin(GPIO_NUM_5);       // 重置GPIO引脚（可选，但推荐）
    gpio_set_direction(GPIO_NUM_5, GPIO_MODE_OUTPUT); // 设置为输出模式
}

void led_on()
{
    printf("LED ON: GPIO5 set to 0\n");  // 低电平使LED亮
    gpio_set_level(GPIO_NUM_5, 0);
    printf("GPIO5 actual level: %d\n", gpio_get_level(GPIO_NUM_5));
}
void led_off()
{
    printf("LED OFF: GPIO5 set to 1\n"); // 高电平使LED灭
    gpio_set_level(GPIO_NUM_5, 1);
    printf("GPIO5 actual level: %d\n", gpio_get_level(GPIO_NUM_5));
}


