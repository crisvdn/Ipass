#ifndef I2C_MPU6050_HPP
#define I2C_MPU6050_HPP

#include "hwlib.hpp"

class i2c_mpu6050{
private:
	hwlib::i2c_bus_bit_banged_scl_sda & bus;
	uint8_t chipAddr;
	int32_t acXoffset = 0x00, acYoffset = 0x00, acZoffset = 0x00;
	int32_t GyXoffset = 0x00, GyYoffset = 0x00, GyZoffset = 0x00;
	double accScale = 0x00;
	double gyroScale = 0x00;
	void initialize();
	void header_values();
	void check_identity();
	//void test();

public:
	i2c_mpu6050(hwlib::i2c_bus_bit_banged_scl_sda & bus, uint8_t chipAddr):
	bus ( bus ),
	chipAddr ( chipAddr )
	{
	initialize();
	check_identity();
	calibrate();
	accScale = get_accel_scale();
	gyroScale = get_gyro_scale();
	}
	
	int get_gyro_scale();
	int get_accel_scale();
	void set_register(const uint8_t &regAddr, const uint8_t & data);
	
	int16_t read_bytes(const uint8_t &regAddr);
	int16_t read_word(const uint8_t &regAddr);
	
	void display_raw_values();
	void display_values();
	void display_roll_pitch(); //rp is short for roll & pitch.
	void display_calibrate_values();
	int read_roll();
	int read_pitch();
	void set_calibrate_values(const int16_t & accelXoffset, const int16_t & accelYoffset,const int16_t & accelZoffset,
	const int16_t & GyroXoffset, const int16_t & GyroYoffset, const int16_t & GyroZoffset);
	void calibrate();
	void start_measure();
};


#endif		//I2C_MPU6050_HPP