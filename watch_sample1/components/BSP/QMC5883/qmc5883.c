#include "qmc5883.h"
#include "stdio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include <math.h>
#include "iic.h"
#include <stdint.h>

qmc_calibration_t calibration = {
    .offset = {0, 0, 0},
    .weight = {1.0f, 1.0f, 1.0f}
};
qmc_range_t range = {
    .x_min =  32767,
    .x_max = -32768,
    .y_min =  32767,
    .y_max = -32768,
    .z_min =  32767,
    .z_max = -32768,
};

uint8_t range_updata_en = 0;
int16_t qmc5883l_xy_offset_angle = 0;

typedef struct
{
    uint8_t mode : 2;
    uint8_t odr : 2;
    uint8_t osr1 : 2;
    uint8_t osr2 : 2;
} reg_control_1_t;
typedef struct
{
    uint8_t set_reset_mode : 2;
    uint8_t range : 2;
    uint8_t null : 2;
    uint8_t self_test : 1;
    uint8_t soft_rst : 1;
} reg_control_2_t;
static reg_control_1_t contron_1_reg_value;//控制寄存器1
static reg_control_2_t contron_2_reg_value;//控制寄存器2
// 工作模式 // 输出速率 // 测量范围 // 过采样率, 数值越大噪声越低功耗越高
void qmc5883_init(qmc5883l_mode_t mode, qmc5883l_odr_t odr, qmc5883l_range_t range, qmc5883l_osr1_t osr1,qmc5883l_osr2_t osr2)
{
    iic_write_byte(QMC_I2C_ADDR,CONTROL_2,0x80);//软重置

    iic_write_byte(QMC_I2C_ADDR,0x29,0x06);//配置极性

    contron_2_reg_value.set_reset_mode = 0;
    contron_2_reg_value.range = range;
    contron_2_reg_value.null =0;
    contron_2_reg_value.soft_rst = 0;
    iic_write_byte(QMC_I2C_ADDR,CONTROL_2, *(uint8_t*)&contron_2_reg_value);//配置寄存器2


    contron_1_reg_value.mode =mode;
    contron_1_reg_value.odr =odr;
    contron_1_reg_value.osr1 = osr1;
    contron_1_reg_value.osr2= osr2;

    iic_write_byte(QMC_I2C_ADDR,CONTROL_1, *(uint8_t*)&contron_1_reg_value);//配置寄存器1

}

void setCalibration(qmc_range_t *range) 
{
    // 安全检查：如果最大最小值相等，说明还没转起来，不计算，防止除零
    if (range->x_max == range->x_min || range->y_max == range->y_min || range->z_max == range->z_min) {
        return; 
    }

    // 1. 计算偏移
    calibration.offset[0] = (range->x_min + range->x_max) / 2;
    calibration.offset[1] = (range->y_min + range->y_max) / 2;
    calibration.offset[2] = (range->z_min + range->z_max) / 2;

    // 2. 计算半径
    float x_avg_delta = (range->x_max - range->x_min) / 2.0f; // 加 f 表示浮点数
    float y_avg_delta = (range->y_max - range->y_min) / 2.0f;
    float z_avg_delta = (range->z_max - range->z_min) / 2.0f;

    float avg_delta = (x_avg_delta + y_avg_delta + z_avg_delta) / 3.0f;

    // 3. 计算权重 (如果平均半径为0也不计算)
    if(avg_delta > 0) {
        calibration.weight[0] = avg_delta / x_avg_delta;
        calibration.weight[1] = avg_delta / y_avg_delta;
        calibration.weight[2] = avg_delta / z_avg_delta;
    }
}



void qmc5883_get_xyz(int16_t *x, int16_t *y, int16_t *z)
{
    int sta;
    sta = iic_read_byte(QMC_I2C_ADDR,STATUS);
    if (sta == 0 )
    {
        printf("无心数据");
        return ;

    }
    
    uint8_t buffer[6];
    int16_t var[3];
    iic_read_bytes(QMC_I2C_ADDR, DATA_OUTPUT_X_LSB, buffer, sizeof(buffer));
    var[0] = (buffer[1] << 8) | buffer[0];
    var[1] = (buffer[3] << 8) | buffer[2];
    var[2] = (buffer[5] << 8) | buffer[4];

    printf("地磁数据 x = %d ,y = %d , z = %d\r\n",var[0],var[1],var[2]);

    if (range_updata_en)//校准用的
    {
        if (var[0] < range.x_min) range.x_min = var[0];
        if (var[0] > range.x_max) range.x_max = var[0];
        if (var[1] < range.y_min) range.y_min = var[1];
        if (var[1] > range.y_max) range.y_max = var[1];
        if (var[2] < range.z_min) range.z_min = var[2];
        if (var[2] > range.z_max) range.z_max = var[2];

        setCalibration(&range);

    }
    if (x != NULL)
    {
        *x = (int16_t)((var[0] - calibration.offset[0]) *calibration.weight[0]);
    }
    if (y != NULL)
    {
        *y = (int16_t)((var[1] - calibration.offset[1]) *calibration.weight[1]);
    }
    if (z != NULL)
    {
        *z = (int16_t)((var[2] - calibration.offset[2]) *calibration.weight[2]);
    }

}
int16_t qmc5883_get_angle()
{
    int16_t x, y, z;
    qmc5883_get_xyz(&x, &y, &z);
    int16_t heading = atan2(y, x) * (180.0 /M_PI);//M_PI:3.14159265358979323846
    heading += qmc5883l_xy_offset_angle;
    
    while (heading < 0) heading += 360;
    while (heading >= 360) heading -= 360;
    return heading;
}

int a=0;
void setMagneticDeclination(int degrees, uint8_t minutes) {
	 a = degrees + minutes / 60;
}
//设置磁偏角