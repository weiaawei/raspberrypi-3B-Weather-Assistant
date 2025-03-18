#include "../include/dht11.h"

DHT11::DHT11(const std::string &chip_name, unsigned int pin) : pin(pin), chip(nullptr), line(nullptr)
{
    try
    {
        initialize();
    }
    catch (const std::exception &e)
    {
        cleanup();
        throw;
    }
}

DHT11::~DHT11()
{
    cleanup();
}

void DHT11::initialize()
{
    // 打开GPIO芯片
    chip = gpiod_chip_open_by_name(chip_name.c_str());
    if (!chip)
    {
        throw std::runtime_error("无法打开GPIO芯片: " + chip_name);
    }

    // 获取GPIO线
    line = gpiod_chip_get_line(chip, pin);
    if (!line)
    {
        throw std::runtime_error("无法获取GPIO线: " + std::to_string(pin));
    }
}

void DHT11::cleanup()
{
    if (line)
    {
        gpiod_line_release(line);
        line = nullptr;
    }

    if (chip)
    {
        gpiod_chip_close(chip);
        chip = nullptr;
    }
}

bool DHT11::wait_for(bool level, std::chrono::microseconds timeout)
{
    auto start = std::chrono::high_resolution_clock::now();
    int expected_value = level ? 1 : 0;

    while (true)
    {
        // 配置为输入模式
        if (gpiod_line_request_input(line, "DHT11") < 0)
        {
            return false;
        }

        // 读取当前值
        int value = gpiod_line_get_value(line);

        // 释放线路
        gpiod_line_release(line);

        if (value == expected_value)
        {
            return true;
        }

        auto now = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - start);

        if (elapsed > timeout)
        {
            return false;
        }

        // 短暂休眠以减少CPU使用
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }
}

DHT11::Reading DHT11::read()
{
    Reading result = {0, 0, false};
    uint8_t data[5] = {0};

    // 发送开始信号
    // 配置为输出模式
    if (gpiod_line_request_output(line, "DHT11", 1) < 0)
    {
        return result;
    }

    // 拉低至少18ms
    gpiod_line_set_value(line, 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    // 拉高
    gpiod_line_set_value(line, 1);
    std::this_thread::sleep_for(std::chrono::microseconds(40));

    // 释放线路
    gpiod_line_release(line);

    // 等待DHT11响应
    if (!wait_for(false, std::chrono::microseconds(90)))
    {
        return result;
    }

    if (!wait_for(true, std::chrono::microseconds(90)))
    {
        return result;
    }

    // 读取40位数据
    for (int i = 0; i < 40; i++)
    {
        if (!wait_for(false, std::chrono::microseconds(60)))
        {
            return result;
        }

        auto start = std::chrono::high_resolution_clock::now();

        if (!wait_for(true, std::chrono::microseconds(80)))
        {
            return result;
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        // 如果高电平持续时间大于约28微秒，则为1，否则为0
        uint8_t bit = (duration.count() > 28) ? 1 : 0;

        // 将位存储到相应的字节中
        data[i / 8] <<= 1;
        data[i / 8] |= bit;
    }

    // 验证校验和
    if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF))
    {
        result.humidity = data[0] + data[1] * 0.1f;
        result.temperature = data[2] + data[3] * 0.1f;
        result.valid = true;
    }

    return result;
}