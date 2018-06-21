#ifndef I2C_MPU6050_HPP
#define I2C_MPU6050_HPP

#include "hwlib.hpp"

class i2c_mpu6050{
private:
	hwlib::i2c_bus_bit_banged_scl_sda & bus;
	uint8_t chipAddr;
	
	void initialize();
	void header_values();
public:
	i2c_mpu6050(hwlib::i2c_bus_bit_banged_scl_sda & bus, uint16_t chipAddr):
	bus ( bus ),
	chipAddr ( chipAddr )
	{
	initialize();
	}
	
	void set_register(const uint8_t &regAddr, const uint8_t & data);
	
	uint16_t read_values(const uint8_t &regAddr);
	uint8_t read_word(const uint8_t &regAddr);
	
	void display_raw_values();
	void display_values();
};


#endif
