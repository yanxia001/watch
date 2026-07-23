#ifndef _hxy_QMC_H_
#define _hxy_QMC_H_
#include <stdint.h>


#define DATA_OUTPUT_X_LSB 0x00
#define DATA_OUTPUT_X_MSB 0x01
#define DATA_OUTPUT_Y_LSB 0x02
#define DATA_OUTPUT_Y_MSB 0x03
#define DATA_OUTPUT_Z_LSB 0x04
#define DATA_OUTPUT_Z_MSB 0x05

#define STATUS 0x09 //状态寄存器

#define BIT_OVFL 0x02 // “0”: normal, “1”: data overflow
#define BIT_DRDY 0x01 // “0”: no new data, “1”: new data is ready



#define CONTROL_1 0x0A //模式转换控制寄存器
#define CONTROL_2 0x0B //控制寄存器 复位


#define CHIP_ID 0x00 //
#define QMC_I2C_ADDR  0x2C  // I2C 7位地址

#define ID_VALUE 0xFF

typedef enum
{
    qmc_status_dor = 0x04,  // 0:正常, 1: 数据跳过阅读
    qmc_status_ovl = 0x02,  // 0:正常, 1: 数据溢出
    qmc_status_drdy = 0x01, // 0:数据未就绪, 1: 新数据已更新
} qmc5883l_status_mask_t;  // 工作模式

typedef enum
{
    qmc_mode_Suspend,      // 暂停
    qmc_mode_NormalMode,   // 正常
    qmc_mode_Single,       //单次
    qmc_mode_ContinuousMode, //连续
} qmc5883l_mode_t;        // 工作模式

typedef enum
{
    qmc_odr_10hz,  // 10HZ
    qmc_odr_50hz,  // 50HZ
    qmc_odr_100hz, // 100HZ
    qmc_odr_200hz, // 200HZ
} qmc5883l_odr_t; // 输出速率

typedef enum
{
    qmc_range_30g,  // 8G
    qmc_range_12g,  // 8G
    qmc_range_8g,  // 8G
    qmc_range_2g,  // 2G
    
} qmc5883l_range_t; // 测量范围

typedef enum
{
    qmc_osr1_8,   // 8
    qmc_osr1_4,   // 4
    qmc_osr1_2,   // 2
    qmc_osr1_1,    // 1
} qmc5883l_osr1_t; // 过采样率, 数值越大噪声越低功耗越高

typedef enum
{
    qmc_osr2_1,   // 8
    qmc_osr2_2,   // 4
    qmc_osr2_4,   // 2
    qmc_osr2_8,    // 1
} qmc5883l_osr2_t; // 过采样率, 数值越大噪声越低功耗越高
  // 校准数据结构
  typedef struct {
      int16_t offset[3];
      float weight[3];
  } qmc_calibration_t;

  // 原始数据范围（用于自动校准）
  typedef struct {
      int16_t x_min, x_max;
      int16_t y_min, y_max;
      int16_t z_min, z_max;
  } qmc_range_t;

  // 外部变量声明
  extern qmc_calibration_t calibration;
  extern qmc_range_t range;
  extern uint8_t range_updata_en;
  extern int16_t qmc5883l_xy_offset_angle;

void qmc5883_init(qmc5883l_mode_t mode, qmc5883l_odr_t odr, qmc5883l_range_t range, qmc5883l_osr1_t osr1,qmc5883l_osr2_t osr2);
  void qmc5883_get_xyz(int16_t *x, int16_t *y, int16_t *z);
  int16_t qmc5883_get_angle();
#endif
