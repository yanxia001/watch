
set(src_dirs 

 )
set(include_dirs

 )
set(requires
 )

set(src_dirs
            BSP/AHT20
            BSP/IIC
            BSP/BMP280
            BSP/MPU6050
            BSP/QMC5883
            BSP/JFH142
            BSP/ST7789
            BSP/CST816
            BSP/BLE/gatt_server_service_table/main
            BSP/LED
            BSP/app
            BSP/app/images
            BSP/app/guider_fonts
            BSP/port
            BSP/TIME
            )
set(include_dirs
            BSP/AHT20
            BSP/IIC
            BSP/BMP280
            BSP/MPU6050
            BSP/QMC5883
            BSP/JFH142
            BSP/ST7789
            BSP/CST816
            BSP/BLE/gatt_server_service_table/main
            BSP/LED
            BSP/app
            BSP/port
           
            BSP/TIME)
set(requires
            driver
            esp_driver_uart
            esp_driver_gpio
            esp_lcd
            nvs_flash
            bt
            lvgl)
            
idf_component_register(SRC_DIRS ${src_dirs} INCLUDE_DIRS ${include_dirs} REQUIRES ${requires})


component_compile_options(-ffast-math -O3 -Wno-error=format -Wno-format)