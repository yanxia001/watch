#ifndef _hxy_CST_H_
#define _hxy_CST_H_



#include <stdio.h>
#include <math.h>

/*定义硬件i2c相关引脚*/
#define CST816T_SCL_PIN    GPIO_NUM_12  
#define CST816T_SDA_PIN    GPIO_NUM_11  
#define CST816T_RST_PIN    GPIO_NUM_10  
#define CST816T_INIT_PIN   GPIO_NUM_9  
 
#define CST816T_I2C_NUM    I2C_NUM_0  //
#define CST816T_ADDR       0x15
 
// 复位设置
#define CST816T_RST_PIN_SET()   do{ gpio_set_level(CST816T_RST_PIN,1); }while(0)
#define CST816T_RST_PIN_RESET() do{ gpio_set_level(CST816T_RST_PIN,0); }while(0)

#define GestureID       0x01 //手势码。0x00：无手势，0x01：上滑，0x02：下滑，0x03：左滑，0x04：右滑，0x05：单击，0x0B：双击，0x0C：长按。
#define FingerNum       0x02 //手指个数。0：无手指 1：1个手指
#define XposH           0x03 //X坐标高4位
#define XposL           0x04 //X坐标低8位
#define YposH           0x05 //Y坐标高4位
#define YposL           0x06 //Y坐标低8位
#define ChipID          0xA7 //芯片型号
#define ProjID          0xA8 //工程编
#define FwVersion       0xA9 //软件版本号
#define FactoryID       0xAA //TP厂家ID
#define BPC0H           0xB0 //BPC0值的高8位
#define BPC0L           0xB1 //BPC0值的低8位
#define BPC1H           0xB2 //BPC1值的高8位
#define BPC1L           0xB3 //BPC1值的低8位
#define SleepMode       0xE5 //值为0x03时进入休眠状态（无触摸唤醒功能）
#define ErrResetCtl     0xEA //bit1:使能大面积触摸复位功能   bit0:使能双指复位功能
#define LongPressTick   0xEB //长按时间门限，默认为100。大约1S。
#define MotionMask      0xEC //使能左右、上下、双击动作（bit0：双击 bit1：上下 bit2：左右）。
#define IrqPluseWidth   0xED //中断低脉冲输出宽度。单位1ms，可选值：1～200。默认值为10。
#define NorScanPer      0xEE //正常快速检测周期。此值会影响到LpAutoWakeTime和AutoSleepTime。单位10ms，可选值：1～30。默认值为1。
#define MotionSlAngle   0xEF //手势检测滑动分区角度控制。Angle=tan(c)*10,c为以x轴正方向为基准的角度。
#define LpScanRaw1H     0xF0 //低功耗扫描1号通道的基准值的高8位。
#define LpScanRaw1L     0xF1 //低功耗扫描1号通道的基准值的低8位。
#define LpScanRaw2H     0xF2 //低功耗扫描2号通道的基准值的高8位。
#define LpScanRaw2L     0xF3 //低功耗扫描2号通道的基准值的低8位。
#define LpAutoWakeTime  0xF4 //低功耗时自动重校正周期。单位1分钟，可选值：1～5。默认值为5。
#define LpScanTH        0xF5 //低功耗扫描唤醒门限。越小越灵敏。可选值：1～255。默认值为48。
#define LpScanWin       0xF6 //低功耗扫描量程。越大越灵敏，功耗越高。可选值：0，1，2，3。默认值为3。
#define LpScanFreq      0xF7 //低功耗扫描频率。越小越灵敏。可选值：1～255。默认值为7。
#define LpScanIdac      0xF8 //低功耗扫描电流。越小越灵敏。可选值：1～255。
#define AutoSleepTime   0xF9 //x秒内无触摸时，自动进入低功耗模式。单位1S，默认值为2S。
#define IrqCtl          0xFA //bit7:中断引脚测试，使能后自动周期性发出低脉冲。  bit6:检测到触摸时，周期性发出低脉冲。  bit5:检测到触摸状态变化时，发出低脉冲。  bit4:检测到手势时，发出低脉冲。  bit0:长按手势只发出一个低脉冲信号。
#define AutoReset       0xFB //x秒内有触摸但无有效手势时，自动复位。单位1S，为0时不启用此功能。默认为5。
#define LongPressTime   0xFC //长按x秒后自动复位。单位1S，为0时不启用此功能。默认为10。
#define IOCtl           0xFD //bit2:主控通过拉低IRQ引脚实现触控的软复位功能 0禁止 1使能   bit1:IIC引脚驱动模式 0电阻上拉，1开漏    bit0:IIC和IRQ引脚电平选择，默认为VDD电平 0：VDD 1：1.8V
#define DisAutoSleep    0xFE //默认为0，使能自动进入低功耗模式。为非0值时，禁止自动进入低功耗模式。




void cst816t_GPIOinit(void);
void cst_getxy(uint16_t *x,uint16_t *y);

int cst816t_get_touch_points_num();
#endif
