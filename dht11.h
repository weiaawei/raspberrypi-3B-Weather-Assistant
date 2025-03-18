#ifndef DHT11_H
#define DHT11_H

#include <string>
#include <gpiod.h>
#include <chrono>
#include <thread>
#include <stdexcept>
#include <iostream>

class DHT11
{
public:
    struct Reading
    {
        float temperature;
        float humidity;
        bool valid;
    };

    DHT11(const std::string &chip_name, unsigned int pin);
    ~DHT11();

    Reading read();

private:
    gpiod_chip *chip;
    gpiod_line *line;
    unsigned int pin;

    void initialize();
    void cleanup();
    bool wait_for(bool level, std::chrono::microseconds timeout);
};

#endif // DHT11_H