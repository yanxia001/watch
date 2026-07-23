#include "jfh142.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "string.h"
#include "driver/gpio.h"

static const int RX_BUF_SIZE = 1024;

#define GPIO_JFH_RST (2)
#define GPIO_JFH_RX (4)   // UART RX 引脚
#define GPIO_JFH_TX (3)   // UART TX 引脚
static void rx_task(void *arg);

void jfh_init(void)
{
    // GPIO 配置（RST 引脚）
    gpio_config_t gpio_init_struct = {0};
    gpio_init_struct.intr_type = GPIO_INTR_DISABLE;       // 失能中断
    gpio_init_struct.mode = GPIO_MODE_OUTPUT;            // 仅输出（RST 通常为输出）
    gpio_init_struct.pull_up_en = GPIO_PULLUP_DISABLE;    // 失能上拉（RST 不需要）
    gpio_init_struct.pull_down_en = GPIO_PULLDOWN_DISABLE;// 失能下拉
    gpio_init_struct.pin_bit_mask = 1ull << GPIO_JFH_RST;// 设置 RST 引脚
    gpio_config(&gpio_init_struct);
    gpio_set_level(GPIO_JFH_RST, 1);                    // 拉高 RST（使能模块）
    vTaskDelay(200);                                  // 模块使能后等待

    // UART 配置
    const uart_config_t uart_config = {
        .baud_rate = 38400,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    // 安装 UART 驱动：RX 缓冲区 2048 字节，TX 缓冲区 0（不使用）
    uart_driver_install(UART_NUM_1, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_1, &uart_config);
    // 设置 UART 引脚：TX=GPIO_JFH_TX, RX=GPIO_JFH_RX，流控引脚不使用
    uart_set_pin(UART_NUM_1, GPIO_JFH_TX, GPIO_JFH_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    // sendData("JFH 测试", &cmd[10]);
    printf("jfh 测试\r\n");
    uint8_t cmd[10]={0x8a};
    uart_write_bytes(UART_NUM_1, &cmd[0], 1);//发送采集数据指令
    xTaskCreate(rx_task, "uart_rx_task", 1024 * 2, NULL, configMAX_PRIORITIES- 3, NULL);//开启接收数据的任务
    // xTaskCreate(tx_task, "uart_tx_task", 1024 * 2, NULL, configMAX_PRIORITIES - 2, NULL);
}

uint8_t JFHdata[100];
static void rx_task(void *arg)
{
 // static const char *RX_TASK_TAG = "RX_TASK";
 // esp_log_level_set(RX_TASK_TAG, ESP_LOG_INFO);
    printf("jfh 测试***\r\n");
 
    while (1) {
        const int rxBytes = uart_read_bytes(UART_NUM_1, JFHdata, 100, 1000);
        printf("rxBytes = %d\r\n", rxBytes);  // 加这行看有没有收到数据
        if (rxBytes > 0) 
        {
            JFHdata[rxBytes] = 0;
            printf("心率血压数据:\r\n");
            for (size_t i = 0; i < 100; i++)
            {

                printf("%02X ",JFHdata[i]);
                
            }
            printf("数据打印完毕\r\n");
            printf("心率 = %d 血氧 = %d 收缩压 = %d 舒张压 = %d ,疲劳指数 = %d\r\n",JFHdata[65],JFHdata[66],JFHdata[71],JFHdata[72],JFHdata[68]);
 // ESP_LOGI(RX_TASK_TAG, "Read %d bytes: '%s'", rxBytes, data);
 // ESP_LOG_BUFFER_HEXDUMP(RX_TASK_TAG, data, rxBytes, ESP_LOG_INFO);
        }
    }
}