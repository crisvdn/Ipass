#ifndef I2C_MPU6050_HPP
#define I2C_MPU6050_HPP

#include "hwlib.hpp"

// register addresses and hexadecimal values for the MPU-6050.
enum commands : uint8_t {
	GYRO_CONFIG = 0x1B, 	 
	ACCEL_CONFIG = 0x1C,	
	PWR_MGMT_1 = 0x6B, 	
	FIFO_EN = 0x23,
	CONFIG = 0x1A,
	WHO_AM_I = 0x75,
	ACCEL_XOUT_H = 0x3B, //bit 15-8
	ACCEL_XOUT_L = 0x3C, //bit 7-0 
	ACCEL_YOUT_H = 0x3D,
	ACCEL_YOUT_L = 0x3E,		
	ACCEL_ZOUT_H = 0x3F,
	ACCEL_ZOUT_L = 0x40,
	TEMP_OUT_H   = 0x41,
	TEMP_OUT_L   = 0x42,
	GYRO_XOUT_H  = 0x43,
	GYRO_XOUT_L  = 0x44,
	GYRO_YOUT_H  = 0x45,
	GYRO_YOUT_L  = 0x46,
	GYRO_ZOUT_H  = 0x47,
	GYRO_ZOUT_L  = 0x48,
	AFS_SEL_0 = 0x00,
	AFS_SEL_1 = 0x08,
	AFS_SEL_2 = 0x10,
	AFS_SEL_3 = 0x18,
	FS_SEL_0 = 0x00,
	FS_SEL_1 = 0x08,
	FS_SEL_2 = 0x10,
	FS_SEL_3 = 0x18
};

class i2c_mpu6050{
private:
	hwlib::i2c_bus_bit_banged_scl_sda & bus;
	uint8_t chipAddr;
	int32_t acXoffset = 0x00, acYoffset = 0x00, acZoffset = 0x00;
	int32_t GyXoffset = 0x00, GyYoffset = 0x00, GyZoffset = 0x00;
	double accScale = 0x00;
	double gyroScale = 0x00;
	double pi = 3.141592;
	
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
	void set_accel_gyro_scale(uint8_t FS_SEL, uint8_t AFS_SEL);
	
	void set_register(const uint8_t &regAddr, const uint8_t & data);
	
	int16_t read_bytes(const uint8_t &regAddr);
	int16_t read_word(const uint8_t &regAddr);
	
	void display_raw_values();
	void display_values();
	void display_roll_pitch(); //rp is short for roll & pitch.
	void display_calibrate_values();
	
	int read_roll();
	int read_pitch();
	int read_gyroX();
	int read_gyroY();
	int read_gyroZ();
	
	void set_calibrate_values(const int16_t & accelXoffset, const int16_t & accelYoffset,const int16_t & accelZoffset,
	const int16_t & GyroXoffset, const int16_t & GyroYoffset, const int16_t & GyroZoffset);
	void calibrate();
	void start_measure();
};


#endif		//I2C_MPU6050_HPP