#​在组件内：优先使用 COMPONENT_DIR，确保路径始终指向组件根目录。
#​在非组件代码中：使用 CMAKE_CURRENT_LIST_DIR（如项目根目录的 CMakeLists.txt）

set(GENERATED_DIR "${COMPONENT_DIR}/Guider_ui")
set(DEVICE_DIR "${COMPONENT_DIR}/device")
set(SETUP_DIR "${COMPONENT_DIR}/setup")

# 递归收集所有子目录的.c文件（关键！）
file(
    GLOB_RECURSE GENERATED_SRCS
    LIST_DIRECTORIES false  # 不包含目录
    "${GENERATED_DIR}/*.c"  # 通配符匹配所有子目录
    "${DEVICE_DIR}/*.c"
    "${SETUP_DIR}/*.c"
)

# 注册组件
idf_component_register(
    # SRC_DIRS
    #     "setup"  #自动寻找源文件，添加源文件要重新构建

    SRCS 
        "main.c"
        # "./setup/setup_gpio.c"
        # "./setup/setup_spi.c"
        # "./setup/setup_uart.c"
        # "./setup/setup_wifi.c"
        # "./device/device_lcd.c"
        # "./device/Digital_key.c"
        "./soft_drv/soft_drv_lvgl_port.c"
        "./lv_ui/lv_ui.c"
        # "./Guider_ui/custom/custom.c"
        ${GENERATED_SRCS}  # 自动包含所有生成的.c文件
        

    INCLUDE_DIRS 
        "."  # 当前目录
        "./setup/include"
        "./device/include"
        "./soft_drv/include"
        "./lv_ui/include"
        # "./Guider_ui/custom"
        ${GENERATED_DIR} # 核心！包含生成文件的根目录
)