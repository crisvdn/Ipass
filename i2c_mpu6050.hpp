#ifndef I2C_MPU6050_HPP
#define I2C_MPU6050_HPP

#include "hwlib.hpp"

class i2c_mpu6050{
private:
	hwlib::i2c_bus_bit_banged_scl_sda & bus;
	uint8_t chipAddr;
	int32_t acXoffset = 0x00, acYoffset = 0x00, acZoffset = 0x00;
	int32_t GyXoffset = 0x00, GyYoffset = 0x00, GyZoffset = 0x00;
	
	void initialize();
	void header_values();

public:
	i2c_mpu6050(hwlib::i2c_bus_bit_banged_scl_sda & bus, uint16_t chipAddr):
	bus ( bus ),
	chipAddr ( chipAddr )
	{
	initialize();
	calibrate();
	}
	
	void set_register(const uint8_t &regAddr, const uint8_t & data);
	
	int16_t read_values(const uint8_t &regAddr);
	int16_t read_word(const uint8_t &regAddr);
	
	void display_raw_values();
	void display_values();
	void display_rp(); //rpy is short for roll & pitch.
	int read_roll();
	int read_pitch();
	void set_calibrate_values(const int16_t & accelXoffset, const int16_t & accelYoffset,const int16_t & accelZoffset,
	const int16_t & GyroXoffset, const int16_t & GyroYoffset, const int16_t & GyroZoffset);
	void calibrate();
	void show_calibrate_values();
	void start_measure();
};


#endif		//I2C_MPU6050_HPP