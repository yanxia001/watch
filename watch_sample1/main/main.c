#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "aht20.h"
#include "iic.h"
#include "bmp280.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "qmc5883.h"
#include "jfh142.h"
#include "cst816.h"
#include "st7789.h"
#include "gatts_table_creat_demo.h"
#include "led.h"
#include "lvgl_demo.h"
#include "gui_guider.h"
#include "bsp_time.h"
lv_ui guider_ui;
TaskHandle_t  aht20TaskHandle = NULL; //定义任务句柄 温湿度
TaskHandle_t  bmp280TaskHandle = NULL; //定义任务句柄 海拔
TaskHandle_t  mpu6050TaskHandle = NULL; //定义任务句柄 计步
TaskHandle_t  qmc5833TaskHandle = NULL; //定义任务句柄 指南针
TaskHandle_t  lvglsetTaskHandle = NULL; //定义任务句柄 lvgl设置
TaskHandle_t  lvgl_taskHandle = NULL; //定义任务句柄 lvgl设置

// 传感器数据（aht20Task 写入，lvgl_task 读取显示）
volatile float gui_temperature = 0.0f;
volatile float gui_humidity = 0.0f;
volatile  unsigned long bushu = 0.0f;

volatile double altitude11 = 0.0f;
volatile double pressure11 = 0.0f;

volatile int16_t agnal = 0.0f;
void aht20Task( void * pvParameters )
{
    float hum, tem;
    AHT21_Init();
    while(1)
    {
        AHT20_Read(&hum, &tem);
        gui_temperature = tem;
        gui_humidity = hum;
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
void bmp280Task( void * pvParameters )
{
    int16_t temp;
    int32_t pressure;
    double altitude;
    double pressure1;
    bmp280_init();
    while(1)
    {
        bmp280_read(&temp,&pressure);
        
        altitude =calculate_altitude( pressure);
        pressure1 =  calculate_pressure(altitude);
        printf("海拔为 %.1f    气压为 %.1f \r\n",altitude,pressure1);
        altitude11 = altitude;
        pressure11 = pressure1;
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

}

void mpu6050Task( void * pvParameters )
{
    unsigned long step_count=0;
    // 初始化MPU6050 DMP（加载固件、自检校准、使能计步器）
    uint8_t res = mpu_dmp_init();
    if (res)
    {
        printf("MPU6050 DMP初始化失败，错误码: %d\n", res);
        vTaskDelete(NULL);
        return;
    }
    printf("MPU6050 DMP初始化成功，开始计步\n");
    while(1)
    {
        dmp_get_pedometer_step_count(&step_count);
        bushu = step_count;
        printf("当前步数%ld\n",step_count);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

}

void qmc5833Task( void * pvParameters )
{
    int16_t x ;
    int16_t y ;
    int16_t z ;
    int16_t aaa;
    qmc5883_init(qmc_mode_NormalMode, qmc_odr_100hz, qmc_range_8g, qmc_osr1_2,qmc_osr2_2);
    while(1)
    {
        qmc5883_get_xyz(&x, &y, &z);
        aaa = qmc5883_get_angle();
        printf("地磁数据 x = %d ,y = %d , z = %d\r\n",x,y,z);
        printf("角度 为 %d \n",aaa);
        agnal=aaa;
        vTaskDelay(pdMS_TO_TICKS(1000));

    }
}
// LVGL 线程安全：lvglsetTask 设置切换请求，lvgl_task 负责调用 lv_scr_load
volatile int gui_switch_request = 0;  // 0=不变, 1=需要切换（由 lvgl_task 执行）

void lvglsetTask( void * pvParameters )
{
    int a = 0;
    int last_gesture = 0;
    while(1)
    {
        a = cst816t_get_shoushi();
        if((a == 0x01 || a == 0x02 || a == 0x03 || a == 0x04) && a != last_gesture)
        {
            last_gesture = a;
            gui_switch_request = 1;  // 通知 lvgl_task 切换屏幕
        }
        if(a == 0)
            last_gesture = 0;

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

extern volatile float gui_temperature;
extern volatile float gui_humidity;
extern volatile unsigned long buzhu ;
extern volatile JFH_Sensor_Data_t jfh_data;
extern volatile int16_t agnal ;
extern volatile double altitude11;
extern volatile double pressure11;
void lvgl_task(void *arg)
{
    int on_main = 1;  // 当前是否在主屏幕
    int tick = 0;
    while(1)
    {
        lv_timer_handler();

        // 屏幕切换
        if (gui_switch_request == 1)
        {
            gui_switch_request = 0;
            if (on_main)
            {
                lv_scr_load(guider_ui.screen_1);
                on_main = 0;
            }
            else
            {
                lv_scr_load(guider_ui.screen);
                on_main = 1;
            }
        }
     
        rtc_get_time();
        lv_label_set_text_fmt(guider_ui.screen_label_10, "%02d:%02d:%02d",
                                    calendar.hour, calendar.min, calendar.sec);
        lv_label_set_text_fmt(guider_ui.screen_label_9, "%04d/%02d/%02d",
                                      calendar.year, calendar.month, calendar.date);
           
     
        
        // 更新传感器数据显示（整型显示，避免 picolibc 不支持 %%f）
        lv_label_set_text_fmt(guider_ui.screen_label_3, "%d", (int)gui_temperature);
        lv_label_set_text_fmt(guider_ui.screen_label_6, "%d", (int)gui_humidity);
        lv_label_set_text_fmt(guider_ui.screen_label_2, "%d", (int)bushu);
        lv_label_set_text_fmt(guider_ui.screen_label_8, "%d", (int)jfh_data.heart_rate);

        lv_label_set_text_fmt(guider_ui.screen_2_label_2, "%d", (int)jfh_data.heart_rate);
        lv_label_set_text_fmt(guider_ui.screen_2_label_5, "%d", (int)jfh_data.oxygen);
        lv_label_set_text_fmt(guider_ui.screen_2_label_10, "%d", (int)jfh_data.sys_bp);
        lv_label_set_text_fmt(guider_ui.screen_2_label_10, "%d", (int)jfh_data.dia_bp);
        lv_label_set_text_fmt(guider_ui.screen_2_label_11, "%d", (int)jfh_data.fatigue);


        lv_label_set_text_fmt(guider_ui.screen_3_label_10, "%d", (int)gui_temperature);
        lv_label_set_text_fmt(guider_ui.screen_3_label_9, "%d", (int)gui_humidity);
        lv_label_set_text_fmt(guider_ui.screen_3_label_7, "%d", (int)pressure11);
        lv_label_set_text_fmt(guider_ui.screen_3_label_8, "%d", (int)altitude11);

        // 指南针仪表盘（范围 0~360，与量程一致）
        lv_meter_set_indicator_value(guider_ui.screen_4_meter_1,
                                guider_ui.screen_4_meter_1_scale_0_ndline_0,
                                (int)agnal);
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

void app_main(void)
{
    nvs_flash_init();
    iic_init(IIC_NUM, 7, 8);
    jfh_init();

    uics();


	setup_ui(&guider_ui);          // 创建主屏幕并加载（时钟、步数等）
	setup_scr_screen_1(&guider_ui);//创建界面1（菜单）
	setup_scr_screen_2(&guider_ui);//创建界面2
	setup_scr_screen_3(&guider_ui);//创建界面3
    setup_scr_screen_4(&guider_ui);//创建界面3
	
    rtc_set_time(2016,7,30,12,30,24);
	
    
	BaseType_t xReturned;
	xReturned = xTaskCreate(
						aht20Task,       	//任务主体函数名称
						"aht20",         	//任务的别名
						1024 * 4,            	//任务站空间  128*4 以字为单位划分的4字节 一个字是四字节
						NULL,							//任务主体函数传参
						2,              	//任务优先级
						&aht20TaskHandle);	//任务句柄（和分配的空间有关）
	
	xReturned = xTaskCreate(
						bmp280Task,       	//任务主体函数名称
						"bmp280",         	//任务的别名
						1024 * 4,            	//任务站空间  128*4 以字为单位划分的4字节 一个字是四字节
						NULL,							//任务主体函数传参
						2,              	//任务优先级
						&bmp280TaskHandle);	//任务句柄（和分配的空间有关）
   xReturned = xTaskCreate(mpu6050Task,"mpu6050",1024*2 ,NULL,2,&mpu6050TaskHandle);	//任务句柄（和分配的空间有关）
	
   xReturned = xTaskCreate(qmc5833Task,"qmc5833",1024*4 ,NULL,2,&qmc5833TaskHandle);
    xReturned = xTaskCreate(lvglsetTask,"lvglset",1024*14 ,NULL,2,&lvglsetTaskHandle);
    xTaskCreate(lvgl_task, "lvgl_task", 1024*10, NULL, configMAX_PRIORITIES -2, &lvgl_taskHandle);
	

// 	xReturned = xTaskCreate(RgbTask,"Rgb",128,NULL,1,&RgbTaskHandle); 

	//vTaskStartScheduler();//启动RTOS。只用写一次
	


}


//任务栈溢出钩子函数
//触发条件：当有任务空间不够时，会执行该函数
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    printf("栈溢出！任务: %s\n", pcTaskName);
    while(1) {}
}




void vApplicationTickHook(void)
{
		lv_tick_inc(1);
}



// void app_main(void)
// {
//     nvs_flash_init();
//     iic_init(IIC_NUM, 7, 8);
    
//     AHT21_Init();
//     bmp280_init();
//     qmc5883_init(qmc_mode_NormalMode, qmc_odr_100hz, qmc_range_8g, qmc_osr1_2,qmc_osr2_2);
//     jfh_init();
//     esp_lcd_panel_handle_t dev;
    
    
//     //dev = st_init();
//     //vTaskDelay(100 / portTICK_PERIOD_MS);
//     // cst816t_GPIOinit();  // 已移到 lv_port_indev_init → touchpad_init 中
    
//     uics();
//     lv_example_switch_1();
//     xTaskCreate(lvgl_task, "lvgl_task", 1024*8, NULL, configMAX_PRIORITIES -2, NULL);
    
//     //lcd_clear_screen_line_by_line(dev, 0x001F); // 蓝


//     //ble_gatts_init();





//     int16_t temp;
//     int32_t pressure;
//     int16_t aaa;
//     unsigned long step_count;
    
//     int16_t x ;
//     int16_t y ;
//     int16_t z ;
//     uint16_t touch_x, touch_y;
//     while (1)
//     {
//         AHT20_Read();
//         bmp280_read(&temp, &pressure);

        
//         //cst_getxy(&touch_x, &touch_y); // 传入变量地址，函数内部通过指针修改值
//         //printf("touch_x: %d, touch_y: %d\n", touch_x, touch_y);


//         dmp_get_pedometer_step_count(&step_count);
//         printf("当前步数%ld\n",step_count);


//         qmc5883_get_xyz(&x, &y, &z);
//         aaa = qmc5883_get_angle();
//         printf("1111111111111   地磁数据 x = %d ,y = %d , z = %d\r\n",x,y,z);
//         printf("角度 为 %d \n",aaa);
//         vTaskDelay(pdMS_TO_TICKS(1000));
//         //lv_timer_handler(); // 刷新屏幕显示
//     }
// }
