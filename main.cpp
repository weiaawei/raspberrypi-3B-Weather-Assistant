#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <csignal>
#include <atomic>

#include "../include/dht11.h"
#include "../include/oled_display.h"

// 全局变量，用于信号处理
std::atomic<bool> running(true);

// 信号处理函数
void signalHandler(int signum)
{
    std::cout << "接收到信号 " << signum << "，正在退出..." << std::endl;
    running = false;
}

// 获取当前时间字符串
std::string getCurrentTime()
{
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    struct tm timeinfo;
    localtime_r(&time, &timeinfo);

    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << timeinfo.tm_hour << ":"
       << std::setfill('0') << std::setw(2) << timeinfo.tm_min << ":"
       << std::setfill('0') << std::setw(2) << timeinfo.tm_sec;

    return ss.str();
}

// 获取当前日期字符串
std::string getCurrentDate()
{
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    struct tm timeinfo;
    localtime_r(&time, &timeinfo);

    std::stringstream ss;
    ss << (timeinfo.tm_year + 1900) << "-"
       << std::setfill('0') << std::setw(2) << (timeinfo.tm_mon + 1) << "-"
       << std::setfill('0') << std::setw(2) << timeinfo.tm_mday;

    return ss.str();
}

int main()
{
    // 注册信号处理函数
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    try
    {
        // 初始化DHT11传感器
        // 注意：根据实际连接修改GPIO芯片名称和引脚号
        DHT11 dht11("gpiochip0", 17); // 假设DHT11连接到GPIO17

        // 初始化OLED显示屏
        // 注意：根据实际连接修改I2C设备路径和地址
        OLEDDisplay oled("/dev/i2c-1", 0x3C); // 假设OLED连接到I2C-1总线，地址为0x3C
        oled.initialize();

        std::cout << "温湿度监测系统已启动" << std::endl;
        std::cout << "按Ctrl+C退出" << std::endl;

        // 主循环
        while (running)
        {
            // 读取温湿度数据
            auto reading = dht11.read();

            // 获取当前时间
            std::string timeStr = getCurrentTime();
            std::string dateStr = getCurrentDate();

            // 清屏
            oled.clear();

            // 显示日期和时间
            oled.drawString(0, 0, dateStr, false);
            oled.drawString(64, 0, timeStr, false);

            // 显示温湿度数据
            std::stringstream tempSS, humSS;

            if (reading.valid)
            {
                tempSS << "Temp: " << std::fixed << std::setprecision(1) << reading.temperature << " C";
                humSS << "Humi: " << std::fixed << std::setprecision(1) << reading.humidity << " %";

                // 输出到控制台
                std::cout << timeStr << " - 温度: " << reading.temperature
                          << "°C, 湿度: " << reading.humidity << "%" << std::endl;
            }
            else
            {
                tempSS << "Temp: Error";
                humSS << "Humi: Error";

                // 输出到控制台
                std::cout << timeStr << " - 读取传感器失败" << std::endl;
            }

            // 在OLED上显示温湿度
            oled.drawString(0, 20, tempSS.str(), false);
            oled.drawString(0, 30, humSS.str(), false);

            // 绘制边框
            oled.drawRect(0, 0, 128, 16, false); // 时间日期区域
            oled.drawLine(0, 16, 127, 16);       // 分隔线

            // 更新显示
            oled.display();

            // 等待2秒
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }

        // 清屏并显示退出消息
        oled.clear();
        oled.drawString(0, 20, "System Shutdown", false);
        oled.display();
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // 关闭显示
        oled.clear();
        oled.display();

        std::cout << "系统已安全退出" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}