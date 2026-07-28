#include "iic.h"
void iic_init(int i2c_num, int sda_gpio, int scl_gpio)
{
    i2c_config_t i2c_conf={
        .mode = I2C_MODE_MASTER,
        .sda_io_num = sda_gpio,     // 自定义sda的编号
        .scl_io_num = scl_gpio,     // 自定义scl的编号
        .scl_pullup_en = GPIO_PULLUP_ENABLE,  // 上拉避免悬空
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 400000,
    };

    i2c_param_config(i2c_num, &i2c_conf);
    i2c_driver_install(i2c_num, I2C_MODE_MASTER, 0, 0, 0);
}

//读取寄存器的一个字节   I2C 设备的 7 位地址   指向 要读取的寄存器地址 的指针
uint8_t iic_read_byte(   uint8_t a      ,       uint8_t b)
{
    uint8_t dat = 0 ;
    i2c_master_write_read_device(IIC_NUM,a,&b,1,&dat,1,1000/1);
    return dat;

}

//向寄存器写入一个字节  I2C 设备的 7 位地址    写入的寄存器地址  写入寄存器的数据
void iic_write_byte( uint8_t a      ,       uint8_t b  ,  uint8_t dat)
{
    uint8_t write_buf[2]={b,dat};
    i2c_master_write_to_device(IIC_NUM,a,write_buf,2,1000/1);
}

//读取寄存器的多个字节   I2C 设备的 7 位地址   指向 要读取的寄存器地址 的指针
void iic_read_bytes(   uint8_t a      ,       uint8_t b              ,uint8_t*data ,size_t len)
{
    
    i2c_master_write_read_device(IIC_NUM,a,&b,1,data,len,1000/1);
    

}
void iic_read_bytes_0(   uint8_t a      ,       uint8_t b              ,uint8_t*data ,size_t len)
{
    
    i2c_master_write_read_device(I2C_NUM_0,a,&b,1,data,len,1000/1);
    

}

//向寄存器中写入多个字节
void iic_write_bytes(   uint8_t a    ,uint8_t*data ,size_t len)
{
    uint8_t write_buf[10];
    uint8_t i =0;
    i2c_master_write_to_device(IIC_NUM,a,data,len,1000/1);


}



// 从指定寄存器地址开始连续写入多个字节（MPU6050等传感器用）
void iic_write_reg_bytes(uint8_t addr, uint8_t reg, uint8_t *data, size_t len)
{
    uint8_t write_buf[len + 1];
    write_buf[0] = reg;
    for (size_t i = 0; i < len; i++)
        write_buf[i + 1] = data[i];
    i2c_master_write_to_device(IIC_NUM, addr, write_buf, len + 1, 1000 / 1);
}

//纯读数据，不写寄存器地址（AHT20等传感器用）
void iic_read_data(uint8_t a, uint8_t* data, size_t len)
{
    i2c_master_read_from_device(IIC_NUM, a, data, len, 1000 / 1);
}



  // 伪代码示意：
int i2c_write(uint8_t reg, uint8_t *data, uint8_t len) 
{
    iic_write_reg_bytes(0x68,reg,data,len);
      // 调用你已有的 iic_write_reg_bytes(0x68, reg, data, len)
      return 0;
}

int i2c_read(uint8_t reg, uint8_t *data, uint8_t len) 
{
    iic_read_bytes(0x68, reg, data, len);
      // 调用你已有的 iic_read_bytes(0x68, reg, data, len)
    return 0;
}

void delay_ms(unsigned long ms) 
{
    vTaskDelay(pdMS_TO_TICKS(ms));
}