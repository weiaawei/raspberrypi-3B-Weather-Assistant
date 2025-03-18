# ��ݮ����ʪ�ȼ��ϵͳ

����һ��������ݮ��3B����ʪ�ȼ��ϵͳ��ʹ��DHT11��������ȡ��ʪ�����ݣ�����OLED��ʾ������ʾ��ʪ�Ⱥ͵�ǰʱ�䡣

## Ӳ��Ҫ��

- ��ݮ��3B����°汾
- DHT11��ʪ�ȴ�����
- SSD1306 OLED��ʾ��(I2C�ӿ�)
- ������

## ����˵��

### DHT11������

- VCC: ���ӵ���ݮ��3.3V��5V��Դ����
- GND: ���ӵ���ݮ��GND����
- DATA: ���ӵ���ݮ��GPIO17(BCM���)

### OLED��ʾ��(I2C)

- VCC: ���ӵ���ݮ��3.3V��Դ����
- GND: ���ӵ���ݮ��GND����
- SCL: ���ӵ���ݮ��SCL(GPIO3, BCM���)
- SDA: ���ӵ���ݮ��SDA(GPIO2, BCM���)

## �������

- libgpiod (����GPIO����)
- libi2c-dev (����I2Cͨ��)

## ��װ����

```bash
sudo apt-get update
sudo apt-get install -y libgpiod-dev libi2c-dev cmake build-essential
```

## ����Ͱ�װ

```bash
# ��������Ŀ¼
mkdir build
cd build

# ���úͱ���
cmake ..
make

# ��װ(��ѡ)
sudo make install
```

## ����

```bash
# ����Ѱ�װ
home_temp_monitor

# ����ֱ�Ӵӹ���Ŀ¼����
./home_temp_monitor
```

## ����

�����Ҫ�޸�GPIO���Ż�I2C�豸����༭`src/main.cpp`�ļ��е���Ӧ���ã�

```cpp
// ��ʼ��DHT11������
// �޸�GPIOоƬ���ƺ����ź�
DHT11 dht11("gpiochip0", 17); // DHT11���ӵ�GPIO17

// ��ʼ��OLED��ʾ��
// �޸�I2C�豸·���͵�ַ
OLEDDisplay oled("/dev/i2c-1", 0x3C); // OLED���ӵ�I2C-1���ߣ���ַΪ0x3C
```

## ����

- ��ȡDHT11���������¶Ⱥ�ʪ������
- ��OLED��ʾ������ʾ��ǰ���ڡ�ʱ�䡢�¶Ⱥ�ʪ��
- ÿ2�����һ������
- ֧��ͨ��Ctrl+C�����˳�

## �����ų�

1. ȷ��I2C��GPIO�ӿ������ã�
   ```bash
   sudo raspi-config
   # ѡ��"Interface Options" -> "I2C" -> "Yes"
   # ѡ��"Interface Options" -> "GPIO" -> "Yes"
   ```

2. ���I2C�豸��
   ```bash
   i2cdetect -y 1
   ```

3. ���GPIOȨ�ޣ�
   ```bash
   sudo usermod -a -G gpio,i2c $USER
   # ��Ҫ���µ�¼��Ч
   ```

## ���֤

MIT 