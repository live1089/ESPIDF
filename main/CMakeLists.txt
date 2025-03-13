# 定义生成文件的根目录（注意斜杠方向）
set(GENERATED_DIR "${CMAKE_CURRENT_LIST_DIR}/Guider_ui")

# 递归收集所有子目录的.c文件（关键！）
file(
    GLOB_RECURSE GENERATED_SRCS
    LIST_DIRECTORIES false  # 不包含目录
    "${GENERATED_DIR}/*.c"  # 通配符匹配所有子目录
)

# 注册组件
idf_component_register(
    SRCS 
        "main.c"
        "./setup/setup_gpio.c"
        "./setup/setup_spi.c"
        "./device/device_lcd.c"
        "./soft_drv/soft_drv_lvgl_port.c"
        "./lv_ui/lv_ui.c"
        "./Guider_ui/custom/custom.c"
        ${GENERATED_SRCS}  # 自动包含所有生成的.c文件

    INCLUDE_DIRS 
        "."  # 当前目录
        "./setup/include"
        "./device/include"
        "./soft_drv/include"
        "./lv_ui/include"
        "././Guider_ui/custom"
        ${GENERATED_DIR} # 核心！包含生成文件的根目录
)