#include "i2c_mpu6050.hpp"
#include "math.h"
//Private functions

void i2c_mpu6050::initialize(){
	hwlib::cout << "Initializing...\n";
	set_register(0x6B, 0x00); //Set sleep bit to 0. This will activate the chip.
	set_register(0x1C, 0x00); //Set Accel configuration to +-8g Acceleration sensitivity.
	set_register(0x1B, 0x00); //set Gyro configuration to 250 degrees/s
	set_register(0x1A, 0x02); //Set config mode Digital Low Pass Filter to 2.
}

//ACCELEROMETER SENSITIVITY Full Scale Range
//AFS_SEL=0 	±2	16,384 	LSB/g
//AFS_SEL=1 	±4	8,192	LSB/g
//AFS_SEL=2 	±8	4,096 	LSB/g
//AFS_SEL=3 	±16	2,048 	LSB/g

//GYROSCOPE SENSITIVITY	Full-Scale Range 
//FS_SEL=0 131	±250 º/s	LSB/(º/s)
//FS_SEL=1 65.5	±500 º/s	LSB/(º/s)
//FS_SEL=2 32.8	±1000 º/s	LSB/(º/s)
//FS_SEL=3 16.4	±2000 		LSB/(º/s)

void i2c_mpu6050::header_values(){
	hwlib::cout << hwlib::setw(1) <<  "Acc X: "  <<
	hwlib::setw(8)<< " Acc Y: " <<	hwlib::setw(13)<< " Acc Z: "<<
	hwlib::setw(16)<< " Temperature: " <<hwlib::setw(17) <<" X axis: "<<
	hwlib::setw(20) <<" Y axis: "<<	hwlib::setw(23) <<" Z axis: " << '\n';
}

void i2c_mpu6050::calibrate(){
	hwlib::cout << "Calibrating device... Don't move the MPU-6050! \n";
	int16_t AcX, AcY, AcZ, GyX, GyY, GyZ;
	hwlib::wait_ms(100);
	for(unsigned int i=0; i < 100; i++){
		AcX = read_values(0x3B);
		AcY = read_values(0x3D);
		AcZ = read_values(0x3F);
		GyX = read_values(0x43);
		GyY = read_values(0x45);
		GyZ = read_values(0x47);
	}
	for(unsigned int i=0; i < 10; i++){
		AcX = read_values(0x3B);
		acXoffset += (AcX);
		AcY = read_values(0x3D);
		acYoffset += (AcY);
		AcZ = read_values(0x3F);
		acZoffset += (AcZ);
		GyX = read_values(0x43);
		GyXoffset += (GyX);
		GyY = read_values(0x45);
		GyYoffset += (GyY);
		GyZ = read_values(0x47);
		GyZoffset += (GyZ);
		hwlib::cout <<hwlib::dec << hwlib::showbase << "AcZ: " << AcZ << '\n';
	}
	acXoffset = (acXoffset / 10);
	acYoffset = (acYoffset / 10);
	acZoffset = (acZoffset / 10);
	GyXoffset = (GyXoffset / 10);
	GyYoffset = (GyYoffset / 10);
	GyZoffset = (GyZoffset / 10);
	hwlib::cout <<hwlib::dec << hwlib::showbase << "acXoffset: " << acXoffset << '\n';
	hwlib::cout <<hwlib::dec << hwlib::showbase << "acYoffset: " << acYoffset << '\n';
	hwlib::cout <<hwlib::dec << hwlib::showbase << "acZoffset: " << acZoffset << '\n';
	hwlib::cout <<hwlib::dec << hwlib::showbase << "GyXoffset: " << GyXoffset << '\n';
	hwlib::cout <<hwlib::dec << hwlib::showbase << "GyYoffset: " << GyYoffset << '\n';
	hwlib::cout <<hwlib::dec << hwlib::showbase << "GyZoffset: " << GyZoffset << '\n';
	hwlib::cout << "Calibration done!\n";
}

//Public functions

void i2c_mpu6050::display_raw_values(){
	hwlib::cout << "Displaying raw values.\n";
	int16_t AcX, AcY, AcZ, Ty, GyX, GyY, GyZ = 0x00;
	header_values();
	while(1){
		AcX = read_values(0x3B);
		AcY = read_values(0x3D);
		AcZ = read_values(0x3F);
		Ty = read_values(0x41);
		GyX = read_values(0x43);
		GyY = read_values(0x45);
		GyZ = read_values(0x47);
		hwlib::cout << hwlib::dec <<hwlib::setw(1) << AcX << ' '; 
		hwlib::cout << hwlib::dec <<hwlib::setw(8) << AcY << ' '; 
		hwlib::cout << hwlib::dec <<hwlib::setw(13) << AcZ << ' ';
		hwlib::cout << hwlib::dec <<hwlib::setw(16) << (Ty) / 340 + 36 << ' ';
		hwlib::cout << hwlib::dec <<hwlib::setw(17) << GyX << ' ';
		hwlib::cout << hwlib::dec <<hwlib::setw(20) << GyY << ' ';
		hwlib::cout << hwlib::dec <<hwlib::setw(23) << GyZ << '\n';
	}
}

void i2c_mpu6050::display_values(){
	hwlib::cout << "Displaying accelerometer values in G/s and gyro axis values in degrees/sec .\n";
	int16_t AcX, AcY, AcZ, Ty, GyX, GyY, GyZ = 0x00;
	int16_t accelX, accelY, accelZ, gyroX, gyroY, gyroZ = 0x00;
	header_values();
	while(1){
	AcX = read_values(0x3B);
	AcY = read_values(0x3D);
	AcZ = read_values(0x3F);
	Ty = read_values(0x41);
	GyX = read_values(0x43);
	GyY = read_values(0x45);
	GyZ = read_values(0x47);
	accelX = ((AcX - acXoffset) / 16384);
	accelY = ((AcY - acYoffset) / 16384);
	accelZ = ((AcZ - acZoffset) / 16384) + 1;
	gyroX = ((GyX - GyXoffset) / 131);
	gyroY = ((GyY - GyYoffset) / 131);
	gyroZ = ((GyZ - GyZoffset) / 131);
	hwlib::cout << hwlib::dec <<hwlib::setw(1) << accelX << ' '; 
	hwlib::cout << hwlib::dec <<hwlib::setw(8) << accelY << ' '; 
	hwlib::cout << hwlib::dec <<hwlib::setw(13) << accelZ << ' ';
	hwlib::cout << hwlib::dec <<hwlib::setw(16) << (Ty / 340) + 36 << ' ';
	hwlib::cout << hwlib::dec <<hwlib::setw(17) << gyroX << ' ';
	hwlib::cout << hwlib::dec <<hwlib::setw(20) << gyroY << ' ';
	hwlib::cout << hwlib::dec <<hwlib::setw(23) << gyroZ << '\n';
	}
}

void i2c_mpu6050::display_rpy(){
	double AcX, AcY, AcZ, Ty, GyX, GyY, GyZ = 0x00;
	double accelX, accelY, accelZ, gyroX, gyroY, gyroZ = 0;
	int16_t roll, pitch = 0;
	
	while(1){
		AcX = read_values(0x3B);
		AcY = read_values(0x3D);
		AcZ = read_values(0x3F);
		Ty = read_values(0x41);
		GyX = read_values(0x43);
		GyY = read_values(0x45);
		GyZ = read_values(0x47);
		accelX = ((AcX - acXoffset) / 16384);
		accelY = ((AcY - acYoffset) / 16384);
		accelZ = ((AcZ - acZoffset) / 16384) + 1;
		gyroX = ((GyX - GyXoffset) / 131);
		gyroY = ((GyY - GyYoffset) / 131);
		gyroZ = ((GyZ - GyZoffset) / 131);
		
		//Calculating roll.
		roll = 180 * atan (accelX/sqrt(accelY*accelY + accelZ*accelZ))/3.141592;
		
		//calculating pitch.
		pitch = 180 * atan (accelY/sqrt(accelX*accelX + accelZ*accelZ))/3.141592;
		
		hwlib::cout << "pitch: " << pitch  << ' ';
		hwlib::cout << " : roll: " << roll << '\n';
		hwlib::wait_ms(20);
	}
}


void i2c_mpu6050::set_register(const uint8_t &regAddr, const uint8_t & data){
	hwlib::cout << "couting data from register: " << data << '\n';
	uint8_t _data[] = {regAddr, data};
	hwlib::cout << "setting register 0x" <<hwlib::hex << regAddr << " with value: " << data << '\n';
	bus.write(chipAddr, _data, 2);
}

int16_t i2c_mpu6050::read_values(const uint8_t &regAddr){
	uint8_t _regData[] = {regAddr};
	uint8_t _data[2];
	bus.write(chipAddr,_regData,1);
	bus.read(chipAddr, _data, 2);
	return ((_data[0] << 8) | _data[1]);
}

int16_t i2c_mpu6050::read_word(const uint8_t &regAddr){
	uint8_t _regData[] = {regAddr};
	uint8_t _data[1];
	bus.write(chipAddr,_regData,1);
	bus.read(chipAddr, _data, 1);
	return _data[0];
}

void i2c_mpu6050::set_calibrate_values(const int16_t & accelXoffset, const int16_t & accelYoffset, 
	const int16_t & accelZoffset, const int16_t & GyroXoffset,
	const int16_t & GyroYoffset, const int16_t & GyroZoffset){
		i2c_mpu6050::acXoffset = accelXoffset;
		i2c_mpu6050::acYoffset = accelYoffset;
		i2c_mpu6050::acZoffset = accelZoffset;
		i2c_mpu6050::GyXoffset = GyroXoffset;
		i2c_mpu6050::GyYoffset = GyroYoffset;
		i2c_mpu6050::GyZoffset = GyroZoffset;
}

void i2c_mpu6050::show_calibrate_values(){
	hwlib::cout << hwlib::setw(1) << "Accel X offset" << hwlib::setw(8) << "Accel Y offset" <<
	hwlib::setw(13) << "Accel Z offset" << hwlib::setw(17) << "Gyro X offset" << hwlib::setw(20) << "Gyro Y offset" <<
	hwlib::setw(23) << "Gyro Z offset\n";
	hwlib::cout << hwlib::dec <<hwlib::setw(1) << acXoffset << ' '; 
	hwlib::cout << hwlib::dec <<hwlib::setw(8) << acYoffset << ' '; 
	hwlib::cout << hwlib::dec <<hwlib::setw(13) << acZoffset << ' ';
	hwlib::cout << hwlib::dec <<hwlib::setw(17) << GyXoffset << ' ';
	hwlib::cout << hwlib::dec <<hwlib::setw(20) << GyYoffset << ' ';
	hwlib::cout << hwlib::dec <<hwlib::setw(23) << GyZoffset << '\n';
}