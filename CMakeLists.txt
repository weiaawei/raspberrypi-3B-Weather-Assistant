cmake_minimum_required(VERSION 3.10)
project(home_temp_monitor CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找必要的库
find_package(Threads REQUIRED)
find_library(GPIOD_LIBRARY NAMES gpiod)
find_path(GPIOD_INCLUDE_DIR NAMES gpiod.h)

# 添加FastDDS支持（如果需要）
# find_package(FastDDS REQUIRED)
# find_package(fastcdr REQUIRED)

# 包含目录
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GPIOD_INCLUDE_DIR}
)

# 添加源文件
file(GLOB SOURCES "src/*.cpp")

# 创建可执行文件
add_executable(home_temp_monitor ${SOURCES})

# 链接库
target_link_libraries(home_temp_monitor
    ${GPIOD_LIBRARY}
    Threads::Threads
    # ${FastDDS_LIBRARIES}
    # ${fastcdr_LIBRARIES}
    rt
    i2c
)

# 安装
install(TARGETS home_temp_monitor DESTINATION bin) 