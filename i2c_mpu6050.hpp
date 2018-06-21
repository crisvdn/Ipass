#ifndef I2C_MPU6050_HPP
#define I2C_MPU6050_HPP

#include "hwlib.hpp"

class i2c_mpu6050{
private:
	hwlib::i2c_bus_bit_banged_scl_sda & bus;
	uint8_t chipAddr;
public:
	i2c_mpu6050(hwlib::i2c_bus_bit_banged_scl_sda & bus, uint16_t chipAddr):
	bus ( bus ),
	chipAddr ( chipAddr )
	{
	set_register(0x6B, 0x00); //Set sleep bit 0;
	}
	void set_register(const uint8_t &regAddr, const uint8_t & data);
	uint16_t read_values(const uint8_t &regAddr);
};


#endif
