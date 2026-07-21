#ifndef _hxy_BMP_H_
#define _hxy_BMP_H_
#include "driver/i2c.h"

/*============================================================================
 * BMP280 I2C 地址 & 芯片ID
 *============================================================================*/
#define BMP280_ADDR                 (0x77)  // 7位地址（0x76/0x77，看SDO电平）
#define BMP280_DEFAULT_CHIP_ID      (0x58)

/*============================================================================
 * BMP280 寄存器定义
 *============================================================================*/
#define BMP280_ADDR                 (0x77)  // 7位I2C地址（SDO接GND=0x76，接VDD=0x77）
#define BMP280_DEFAULT_CHIP_ID      (0x58)

#define BMP280_CHIP_ID              (0xD0)  /* Chip ID Register */
#define BMP280_RST_REG              (0xE0)  /* Softreset Register */
#define BMP280_STAT_REG             (0xF3)  /* Status Register */
#define BMP280_CTRL_MEAS_REG        (0xF4)  /* Ctrl Measure Register */
#define BMP280_CONFIG_REG           (0xF5)  /* Configuration Register */
#define BMP280_PRESSURE_MSB_REG     (0xF7)  /* Pressure MSB Register */
#define BMP280_PRESSURE_LSB_REG     (0xF8)  /* Pressure LSB Register */
#define BMP280_PRESSURE_XLSB_REG    (0xF9)  /* Pressure XLSB Register */
#define BMP280_TEMPERATURE_MSB_REG  (0xFA)  /* Temperature MSB Reg */
#define BMP280_TEMPERATURE_LSB_REG  (0xFB)  /* Temperature LSB Reg */
#define BMP280_TEMPERATURE_XLSB_REG (0xFC)  /* Temperature XLSB Reg */


typedef struct {
    uint16_t dig_T1;  // 温度校准系数1（16位）
    int16_t dig_T2;  // 温度校准系数2（16位）
    int16_t dig_T3;  // 温度校准系数3（16位）
    uint16_t dig_P1; // 压力校准系数1（16位）
    int16_t dig_P2;  // 压力校准系数2（16位）
    int16_t dig_P3;  // 压力校准系数3（16位）
    int16_t dig_P4;  // 压力校准系数4（16位）
    int16_t dig_P5;  // 压力校准系数5（16位）
    int16_t dig_P6;  // 压力校准系数6（16位）
    int16_t dig_P7;  // 压力校准系数7（16位）
    int16_t dig_P8;  // 压力校准系数8（16位）
    int16_t dig_P9;  // 压力校准系数9（16位）
} bmp280Cal_t;

void bmp280_init();
uint8_t bmp280_read(int16_t *temp, int32_t *pressure);
double calculate_altitude(double pressure);
double calculate_pressure(double altitude);

#endif
