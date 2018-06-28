# Ipass

This is a library for the MPU-6050.
This library interfaces with the MPU-6050 Gyroscope and Accelerometer. It provides functionality to output the data easily to the screen
or return it so you can do anything with it. Any customisation for user-preferred settings can be done by supporting functions in this libary.

This library supports only I2C communication. So any versions like the MPU-6000 are not supported(SPI).

![alt text](https://i.imgur.com/NcQi8zY.png)

Link to the datasheet:
https://www.invensense.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf

Link to the register map:
https://www.invensense.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf

Code example to use this library.


```
#include "hwlib.hpp"
#include "i2c_mpu6050.hpp"

int main(){
      uint8_t MPU_addr = 0x68;
      hwlib::wait_ms(500);
      //disable watchdog.
      WDT->WDT_MR = WDT_MR_WDDIS;
      namespace target = hwlib::target;
  
      //creating scl & sda pins.
      auto scl = target::pin_oc(target::pins::d9);
      auto sda = target::pin_oc(target::pins::d8);
      //creating mpu6050 object. assigning address & bus.
      auto mpu = i2c_mpu6050(bus, MPU_addr);
      mpu.display_calibrate_values();
      mpu.test();
      //output roll & pitch to screen.
      mpu.display_roll_pitch();
}
```
