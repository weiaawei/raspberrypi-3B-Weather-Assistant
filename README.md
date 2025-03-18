# 树莓派温湿度监测系统

这是一个基于树莓派3B的温湿度监测系统，使用DHT11传感器读取温湿度数据，并在OLED显示屏上显示温湿度和当前时间。

## 硬件要求

- 树莓派3B或更新版本
- DHT11温湿度传感器
- SSD1306 OLED显示屏(I2C接口)
- 连接线

## 接线说明

### DHT11传感器

- VCC: 连接到树莓派3.3V或5V电源引脚
- GND: 连接到树莓派GND引脚
- DATA: 连接到树莓派GPIO17(BCM编号)

### OLED显示屏(I2C)

- VCC: 连接到树莓派3.3V电源引脚
- GND: 连接到树莓派GND引脚
- SCL: 连接到树莓派SCL(GPIO3, BCM编号)
- SDA: 连接到树莓派SDA(GPIO2, BCM编号)

## 软件依赖

- libgpiod (用于GPIO控制)
- libi2c-dev (用于I2C通信)

## 安装依赖

```bash
sudo apt-get update
sudo apt-get install -y libgpiod-dev libi2c-dev cmake build-essential
```

## 编译和安装

```bash
# 创建构建目录
mkdir build
cd build

# 配置和编译
cmake ..
make

# 安装(可选)
sudo make install
```

## 运行

```bash
# 如果已安装
home_temp_monitor

# 或者直接从构建目录运行
./home_temp_monitor
```

## 配置

如果需要修改GPIO引脚或I2C设备，请编辑`src/main.cpp`文件中的相应配置：

```cpp
// 初始化DHT11传感器
// 修改GPIO芯片名称和引脚号
DHT11 dht11("gpiochip0", 17); // DHT11连接到GPIO17

// 初始化OLED显示屏
// 修改I2C设备路径和地址
OLEDDisplay oled("/dev/i2c-1", 0x3C); // OLED连接到I2C-1总线，地址为0x3C
```

## 功能

- 读取DHT11传感器的温度和湿度数据
- 在OLED显示屏上显示当前日期、时间、温度和湿度
- 每2秒更新一次数据
- 支持通过Ctrl+C优雅退出

## 故障排除

1. 确保I2C和GPIO接口已启用：
   ```bash
   sudo raspi-config
   # 选择"Interface Options" -> "I2C" -> "Yes"
   # 选择"Interface Options" -> "GPIO" -> "Yes"
   ```

2. 检查I2C设备：
   ```bash
   i2cdetect -y 1
   ```

3. 检查GPIO权限：
   ```bash
   sudo usermod -a -G gpio,i2c $USER
   # 需要重新登录生效
   ```

## 许可证

MIT 