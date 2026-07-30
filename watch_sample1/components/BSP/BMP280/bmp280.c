// BMP280 驱动 —— 气压/海拔
#include "bmp280.h"
#include "iic.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include <math.h>

static bmp280Cal_t bmp280Cal;

static void bmp280_read_calib(void)
{
    uint8_t buf[24];
    iic_read_bytes(BMP280_ADDR, 0x88, buf, 24);
    
    bmp280Cal.dig_T1 = (uint16_t)(buf[0]  | (buf[1]  << 8));
    bmp280Cal.dig_T2 = (int16_t) (buf[2]  | (buf[3]  << 8));
    bmp280Cal.dig_T3 = (int16_t) (buf[4]  | (buf[5]  << 8));
    bmp280Cal.dig_P1 = (uint16_t)(buf[6]  | (buf[7]  << 8));
    bmp280Cal.dig_P2 = (int16_t) (buf[8]  | (buf[9]  << 8));
    bmp280Cal.dig_P3 = (int16_t) (buf[10] | (buf[11] << 8));
    bmp280Cal.dig_P4 = (int16_t) (buf[12] | (buf[13] << 8));
    bmp280Cal.dig_P5 = (int16_t) (buf[14] | (buf[15] << 8));
    bmp280Cal.dig_P6 = (int16_t) (buf[16] | (buf[17] << 8));
    bmp280Cal.dig_P7 = (int16_t) (buf[18] | (buf[19] << 8));
    bmp280Cal.dig_P8 = (int16_t) (buf[20] | (buf[21] << 8));
    bmp280Cal.dig_P9 = (int16_t) (buf[22] | (buf[23] << 8));
}
void bmp280_init()
{
    iic_write_byte(BMP280_ADDR,BMP280_RST_REG,0xB6);//确保传感器处于已知初始状态
    vTaskDelay(pdMS_TO_TICKS(10));


    uint8_t chip_id = iic_read_byte(BMP280_ADDR,BMP280_CHIP_ID);
    if(chip_id != BMP280_DEFAULT_CHIP_ID)
    {
      printf("BMP280芯片ID错误！\r\n");
      return; // 终止初始化
    }

    // 3. 配置测量模式寄存器 0xF4
    // 目标: 温度 x1 (001), 压力 x8 (100), 强制模式 (01)
    // 二进制: 001 100 01 = 0x31
    uint8_t measure_mode = 0x31; 
    iic_write_byte(BMP280_ADDR, BMP280_CTRL_MEAS_REG, measure_mode);

    // 4. 配置配置寄存器 0xF5
    // 目标: Standby 0.5ms (000), IIR 2阶滤波 (001), I2C模式 (0)
    // 二进制: 000 001 0 = 0x04
    uint8_t config = 0x04; // 0x02 = 0000 0010（IIR_2 + Standby_0.5ms）
    iic_write_byte(BMP280_ADDR, 0xF5, config);
    bmp280_read_calib();
}


uint8_t bmp280_read(int16_t *temp, int32_t *pressure)
{
    int32_t adc_T;
    int32_t adc_P;
    // 向测量模式寄存器写入配置，触发一次测量 强制模式
    iic_write_byte(BMP280_ADDR, BMP280_CTRL_MEAS_REG, 0x31);
    // OSR_x8的测量时间约13ms，等待15ms确保完成
    vTaskDelay(pdMS_TO_TICKS(25));
    uint8_t tmp[3];//温度
    iic_read_bytes(BMP280_ADDR,BMP280_TEMPERATURE_MSB_REG,tmp,3);
    adc_T = (tmp[0] << 12) | (tmp[1] << 4) | (tmp[2] >> 4);
    uint8_t tmp1[3];//压力
    iic_read_bytes(BMP280_ADDR,BMP280_PRESSURE_MSB_REG,tmp1,3);
    adc_P = (tmp1[0] << 12) | (tmp1[1] << 4) | (tmp1[2] >> 4);
    if (adc_P == 0)
        return 1;
    int32_t var1, var2, t_fine, p;
     //数据校准
 // Temperature
    var1 = (((float)adc_T) / 16384.0 - ((float)bmp280Cal.dig_T1) / 1024.0) *((float)bmp280Cal.dig_T2);
    var2 = ((((float)adc_T) / 131072.0 - ((float)bmp280Cal.dig_T1) / 8192.0) * (((float)adc_T) / 131072.0 - ((float)bmp280Cal.dig_T1) / 8192.0)) *((float)bmp280Cal.dig_T3);
    t_fine = (uint32_t)(var1 + var2);
    if (temp != NULL)
        *temp = (var1 + var2) / 512;
    if (pressure == NULL)
        return 1;
    var1 = ((float)t_fine / 2.0) - 64000.0;
    var2 = var1 * var1 * ((float)bmp280Cal.dig_P6) / 32768.0;
    var2 = var2 + var1 * ((float)bmp280Cal.dig_P5) * 2.0;
    var2 = (var2 / 4.0) + (((float)bmp280Cal.dig_P4) * 65536.0);
    var1 = (((float)bmp280Cal.dig_P3) * var1 * var1 / 524288.0 +((float)bmp280Cal.dig_P2) * var1) / 524288.0;

    var1 = (1.0 + var1 / 32768.0) * ((float)bmp280Cal.dig_P1);
    p = 1048576.0 - (float)adc_P;
    p = (p - (var2 / 4096.0)) * 6250.0 / var1;
    var1 = ((float)bmp280Cal.dig_P9) * p * p / 2147483648.0;
    var2 = p * ((float)bmp280Cal.dig_P8) / 32768.0;
    p = p + (var1 + var2 + ((float)bmp280Cal.dig_P7)) / 16.0;
    *pressure = p;
    return 0;
}

#define SEA_LEVEL_PRESSURE 1013.25 // 标准海平面大气压力(hPa) 
#define TEMPERATURE 15.0 // 标准温度(摄氏度) 
#define GRAVITY 9.8067 // 重力加速度(m/s²) 
#define LAPSE_RATE 0.0065 // 标准气温垂直梯度(°C/m) 
double calculate_altitude(double pressure) { 
 // 使用巴罗米特高度公式计算海拔高度
    double pressure_hpa = pressure / 100.0; 
    return 44330.0 * (1.0 - pow(pressure_hpa / SEA_LEVEL_PRESSURE, 0.1903)); 
} 
double calculate_pressure(double altitude) { 
 // 使用巴罗米特高度公式计算气压
 return SEA_LEVEL_PRESSURE * pow(1.0 - (LAPSE_RATE * altitude) /
(TEMPERATURE + 273.15), GRAVITY / (LAPSE_RATE * 287.053)); 
}