//SDA GPIO7    SCL GPIO8

#include "aht20.h"
#include "iic.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include <stdio.h>

// 定义全局变量（仅此一处定义）
AHT21_Date_t AHT21_Date;
uint8_t AHT21_Receive_Date[7];
uint8_t AHT21_CRC;

void AHT21_Init(void)
{
    int sta;
    sta = iic_read_byte(0x38, 0x71);
    if ((sta & (0x1 << 3)) == 0)
    {
        uint8_t init_cmd[2] = {0x08, 0x00};
        iic_write_bytes(0x38, init_cmd, 2);
    }


    sta = iic_read_byte(0x38, 0x71);
    if ((sta & (0x1 << 3)) == 0)
    {
        printf("初始化失败\r\n");
    }
    else
        printf("初始化成功\r\n");
}

// CRC8校验函数
unsigned char CheckCrc8(unsigned char *pDat, unsigned char Lenth)
{
    unsigned char crc = 0xff, i, j;
    for (i = 0; i < Lenth; i++)
    {
        crc = crc ^ *pDat;
        for (j = 0; j < 8; j++)
        {
            if (crc & 0x80) crc = (crc << 1) ^ 0x31;
            else crc <<= 1;
        }
        pDat++;
    }
    return crc;
}

// AHT21 读取温湿度函数
void AHT20_Read(float *hum , float *tem)
{
    uint8_t sta;
    int timeout;

    // 1. 发送触发测量命令 (0xAC, 0x33, 0x00)
    uint8_t measure_cmd[3] = {0xAC, 0x33, 0x00};
    iic_write_bytes(0x38, measure_cmd, 3);

    // 2. 轮询等待测量完成（bit7=0 表示完成），最多等100ms
    timeout = 10;
    while(1)
    {
        sta = iic_read_byte(0x38, 0x71);
        vTaskDelay(pdMS_TO_TICKS(10));
        
        if((sta & 0x80)==0)
            break;
        timeout--;
    }
    if(timeout == 0) {
        // 传感器超时一直忙，处理错误（比如直接退出本次读取，或者打印警告）
        printf("AHT21 忙碌超时\r\n");
        return; 
    }
    // do {
    //     vTaskDelay(pdMS_TO_TICKS(10));
    //     sta = iic_read_byte(0x38, 0x71);
    //     timeout--;
    // } while ((sta & 0x80) && (timeout > 0));

    // if (sta & 0x80)
    // {
    //     printf("AHT20 测量超时，状态=0x%02X\r\n", sta);
    //     return;
    // }

    // 3. 纯I2C读7个字节（不写寄存器地址！）
    memset(AHT21_Receive_Date, 0, sizeof(AHT21_Receive_Date));//清理
    iic_read_data(0x38, AHT21_Receive_Date, 7);

    // 4. CRC校验 (前6个字节数据，第7个字节是校验码)
    AHT21_CRC = CheckCrc8(AHT21_Receive_Date, 6);
    if (AHT21_CRC == AHT21_Receive_Date[6])
    {
        // 5. 湿度计算（20位，高12位在byte1~2，低8位在byte3高4位）
        uint32_t hum_raw = AHT21_Receive_Date[1];
        hum_raw = (hum_raw << 8) | AHT21_Receive_Date[2];
        hum_raw = (hum_raw << 4) | (AHT21_Receive_Date[3] >> 4);
        AHT21_Date.Hum = hum_raw * 100.0 / 1048576.0;

        // 6. 温度计算
        uint32_t tem_raw = AHT21_Receive_Date[3] & 0x0F;
        tem_raw = (tem_raw << 8) | AHT21_Receive_Date[4];
        tem_raw = (tem_raw << 8) | AHT21_Receive_Date[5];
        AHT21_Date.Tem = (tem_raw * 200.0) / 1048576.0 - 50.0;

        // 7. 打印结果
        *hum = AHT21_Date.Hum;
        *tem = AHT21_Date.Tem;
        printf("湿度=%.1f%%  温度=%.1f℃\r\n", AHT21_Date.Hum, AHT21_Date.Tem);
    }
    else
    {
        printf("CRC失败 计算=%d 收到=%d | %02X %02X %02X %02X %02X %02X %02X\r\n",
                AHT21_CRC, AHT21_Receive_Date[6],
                AHT21_Receive_Date[0], AHT21_Receive_Date[1], AHT21_Receive_Date[2],
                AHT21_Receive_Date[3], AHT21_Receive_Date[4], AHT21_Receive_Date[5],
                AHT21_Receive_Date[6]);
    }
}
