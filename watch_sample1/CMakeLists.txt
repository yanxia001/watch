# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.22)

set(EXTRA_COMPONENT_DIRS components/lvgl)#显式告诉 ESP-IDF 去搜索 components/lvgl
#你的 components/ 目录本身就是一个组件（因为它有 CMakeLists.txt 并调用了 idf_component_register）。ESP-IDF
  #的规则是：当一个目录被识别为组件后，不会递归扫描其子目录去寻找其他组件。所以 components/lvgl/ 虽然存在且有自己的
  #CMakeLists.txt，但 ESP-IDF 根本不会进去找它
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(watch_sample1)
