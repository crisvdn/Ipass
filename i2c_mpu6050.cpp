#include "i2c_mpu6050.hpp"
#include "math.h"

//Private functions

void i2c_mpu6050::initialize(){
	hwlib::cout << "Initializing...\n";
	set_register(PWR_MGMT_1, 0x00); //Set sleep bit to 0. This will activate the chip.
	set_register(ACCEL_CONFIG, AFS_SEL_0); //Set Accel configuration to +-8g Acceleration sensitivity.
	set_register(GYRO_CONFIG, FS_SEL_0); //set Gyro configuration to 250 degrees/s
	set_register(CONFIG, 0x02); //Set config mode Digital Low Pass Filter to 2.
}

//ACCELEROMETER SENSITIVITY Full Scale Range
//AFS_SEL=0 	±2	16,384 	LSB/g
//AFS_SEL=1 	±4	8,192	LSB/g	
//AFS_SEL=2 	±8	4,096 	LSB/g
//AFS_SEL=3 	±16	2,048 	LSB/g
//
//default initialisation
//Accelerometer sensitivity is set to ±8. The raw values need to be divided by 4096(minus the offset) to get the values in linear gravitational accelaration.

//GYROSCOPE 	SENSITIVITY		Full-Scale Range 	
//FS_SEL=0 		131				±250 º/s	LSB/(º/s)
//FS_SEL=1 		65.5			±500 º/s	LSB/(º/s)
//FS_SEL=2 		32.8			±1000 º/s	LSB/(º/s)
//FS_SEL=3 		16.4			±2000 º/s	LSB/(º/s)
//
//default initialisation
//Gyroscope sensitivity is set to 250 degrees per second. The raw values need to be divided by 131(minus the offset) to get the values of angular change in degrees per second.

void i2c_mpu6050::header_values(){
	hwlib::cout << hwlib::setw(1) <<  "Acc X: "  <<
	hwlib::setw(8)<< " Acc Y: " <<	hwlib::setw(13)<< " Acc Z: "<<
	hwlib::setw(16)<< " Temperature: " <<hwlib::setw(17) <<" X axis: "<<
	hwlib::setw(20) <<" Y axis: "<<	hwlib::setw(23) <<" Z axis: " << '\n';
}

//default address is 0x68. If AD0 is pulled high, address is 0x69.
void i2c_mpu6050::check_identity(){
	uint8_t _data = read_word(WHO_AM_I);
	if(_data == 0x68 || _data == 0x69){
		hwlib::cout << "MPU-6050 found. registry address is: 0x" << hwlib::hex << _data << '\n';;
	}else{
		hwlib::cout << "Couldn't verify identity of MPU-6050.";
	}
	hwlib::wait_ms(100);
}

int i2c_mpu6050::get_gyro_scale(){
	uint8_t _data = read_word(GYRO_CONFIG);
	_data = _data >> 3 & 0x03;
	if(_data == 0){
		gyroScale = 131;
	}else if(_data == 1){
		gyroScale = 65.5;
	}else if(_data == 2){
		gyroScale = 32.8;
	}else if(_data == 3){
		gyroScale = 16.4;
	}
	return gyroScale;
}


int i2c_mpu6050::get_accel_scale(){
	uint8_t _data = read_word(ACCEL_CONFIG);
	_data = _data >> 3 & 0x03;
	if(_data == 0){
		accScale = 16384;
	}else if(_data == 1){
		accScale = 8192;
	}else if(_data == 2){
		accScale = 4096;
	}else if(_data == 3){
		accScale = 2048;
	}
	return accScale;
}


void i2c_mpu6050::set_accel_gyro_scale(uint8_t FS_SEL, uint8_t AFS_SEL){
	if(FS_SEL < 4 && FS_SEL >= 0 && AFS_SEL < 4 && AFS_SEL >= 0){
		FS_SEL = FS_SEL << 3;
		AFS_SEL = AFS_SEL << 3;
		set_register(GYRO_CONFIG, FS_SEL);
		set_register(ACCEL_CONFIG, AFS_SEL);
		accScale = get_accel_scale();
		gyroScale = get_gyro_scale();
		hwlib::cout << "accelerometer and gyro scale set.\n";
	}else{
		hwlib::cout << "Incorrect values were entered to set the accel & gyro scale. Accel & gyro scale are not set. \n";
	}
}

//Public functions.

void i2c_mpu6050::calibrate(){
	hwlib::cout << "Calibrating device... Don't move the MPU-6050! \n";
	int16_t AcX, AcY, AcZ, GyX, GyY, GyZ, Ty;
	hwlib::wait_ms(100);
	for(unsigned int i=0; i < 100; i++){
		AcX = read_bytes(ACCEL_XOUT_H);
		AcY = read_bytes(ACCEL_YOUT_H);
		AcZ = read_bytes(ACCEL_ZOUT_H);
		Ty = read_bytes(TEMP_OUT_H);
		GyX = read_bytes(GYRO_XOUT_H);
		GyY = read_bytes(GYRO_YOUT_H);
		GyZ = read_bytes(GYRO_ZOUT_H);
	}
	for(unsigned int i=0; i < 10; i++){
		AcX = read_bytes(ACCEL_XOUT_H);
		acXoffset += (AcX);
		AcY = read_bytes(ACCEL_YOUT_H);
		acYoffset += (AcY);
		AcZ = read_bytes(ACCEL_ZOUT_H);
		acZoffset += (AcZ);
		Ty = read_bytes(TEMP_OUT_H);
		GyX = read_bytes(GYRO_XOUT_H);
		GyXoffset += (GyX);
		GyY = read_bytes(GYRO_YOUT_H);
		GyYoffset += (GyY);
		GyZ = read_bytes(GYRO_ZOUT_H);
		GyZoffset += (GyZ);
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
	hwlib::wait_ms(100);
	hwlib::cout << "Calibration done!\n";
}

void i2c_mpu6050::display_raw_values(){
	hwlib::cout << "Displaying raw values.\n";
	int16_t AcX, AcY, AcZ, Ty, GyX, GyY, GyZ = 0x00;
	header_values();
	while(1){
		AcX = read_bytes(ACCEL_XOUT_H);
		AcY = read_bytes(ACCEL_YOUT_H);
		AcZ = read_bytes(ACCEL_ZOUT_H);
		Ty = read_bytes(TEMP_OUT_H);
		GyX = read_bytes(GYRO_XOUT_H);
		GyY = read_bytes(GYRO_YOUT_H);
		GyZ = read_bytes(GYRO_ZOUT_H);
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
		AcX = read_bytes(ACCEL_XOUT_H);
		AcY = read_bytes(ACCEL_YOUT_H);
		AcZ = read_bytes(ACCEL_ZOUT_H);
		Ty = read_bytes(TEMP_OUT_H);
		GyX = read_bytes(GYRO_XOUT_H);
		GyY = read_bytes(GYRO_YOUT_H);
		GyZ = read_bytes(GYRO_ZOUT_H);
		accelX = ((AcX - acXoffset) / accScale);
		accelY = ((AcY - acYoffset) / accScale);
		accelZ = ((AcZ - (acZoffset-100)) / accScale) + 1;
		gyroX = ((GyX - GyXoffset) / gyroScale);
		gyroY = ((GyY - GyYoffset) / gyroScale);
		gyroZ = ((GyZ - GyZoffset) / gyroScale);
		hwlib::cout << hwlib::dec <<hwlib::setw(1) << accelX << ' '; 
		hwlib::cout << hwlib::dec <<hwlib::setw(8) << accelY << ' '; 
		hwlib::cout << hwlib::dec <<hwlib::setw(13) << accelZ << ' ';
		hwlib::cout << hwlib::dec <<hwlib::setw(16) << (Ty / 340) + 36 << ' ';
		hwlib::cout << hwlib::dec <<hwlib::setw(17) << gyroX << ' ';
		hwlib::cout << hwlib::dec <<hwlib::setw(20) << gyroY << ' ';
		hwlib::cout << hwlib::dec <<hwlib::setw(23) << gyroZ << '\n';
	}
}

void i2c_mpu6050::display_roll_pitch(){
	double AcX, AcY, AcZ, GyX, GyY, GyZ;
	double accelX, accelY, accelZ, gyroX, gyroY, gyroZ;
	int16_t roll = 0, pitch = 0;
	
	while(1){
		//reading raw values.
		AcX = read_bytes(ACCEL_XOUT_H);
		AcY = read_bytes(ACCEL_YOUT_H);
		AcZ = read_bytes(ACCEL_ZOUT_H);
		GyX = read_bytes(GYRO_XOUT_H);
		GyY = read_bytes(GYRO_YOUT_H);
		GyZ = read_bytes(GYRO_ZOUT_H);
		accelX = ((AcX - acXoffset) / accScale);
		accelY = ((AcY - acYoffset) / accScale);
		accelZ = ((AcZ - acZoffset) / accScale) + 1;
		gyroX = ((GyX - GyXoffset) / gyroScale);
		gyroY = ((GyY - GyYoffset) / gyroScale);
		gyroZ = ((GyZ - GyZoffset) / gyroScale);
		//Calculating roll.
		roll = 180 * atan (accelX/sqrt(accelY*accelY + accelZ*accelZ))/pi;
		//calculating pitch.
		pitch = 180 * atan (accelY/sqrt(accelX*accelX + accelZ*accelZ))/pi;
		//Output pitch and roll to screen.
		hwlib::cout << "pitch: " << pitch  << ' ';
		hwlib::cout << " : roll: " << roll << '\n';
		hwlib::wait_ms(20);
	}
}

int i2c_mpu6050::read_roll(){
	double AcX, AcY, AcZ, accelX, accelY, accelZ;
	int roll = 0;
	AcX = read_bytes(ACCEL_XOUT_H);
	AcY = read_bytes(ACCEL_YOUT_H);
	AcZ = read_bytes(ACCEL_ZOUT_H);
	accelX = ((AcX - acXoffset) / accScale);
	accelY = ((AcY - acYoffset) / accScale);
	accelZ = ((AcZ - acZoffset) / accScale) + 1;
	//Calculating roll.
	roll = 180 * atan (accelX/sqrt(accelY*accelY + accelZ*accelZ))/pi;
	return roll;
}

int i2c_mpu6050::read_pitch(){
	double AcX, AcY, AcZ, accelX, accelY, accelZ;
	int pitch = 0;
	AcX = read_bytes(ACCEL_XOUT_H);
	AcY = read_bytes(ACCEL_YOUT_H);
	AcZ = read_bytes(ACCEL_ZOUT_H);
	accelX = ((AcX - acXoffset) / accScale);
	accelY = ((AcY - acYoffset) / accScale);
	accelZ = ((AcZ - acZoffset) / accScale) + 1;
	//Calculating roll.
	pitch = 180 * atan (accelY/sqrt(accelX*accelX + accelZ*accelZ))/pi;
	return pitch;
}

int i2c_mpu6050::read_gyroX(){
	int16_t AcX;
	AcX = read_bytes(ACCEL_XOUT_H);
	return AcX;
}

int i2c_mpu6050::read_gyroY(){
	int16_t AcY;
	AcY = read_bytes(ACCEL_YOUT_H);
	return AcY;
}

int i2c_mpu6050::read_gyroZ(){
	int16_t AcZ;
	AcZ = read_bytes(ACCEL_ZOUT_H);
	return AcZ;
}


void i2c_mpu6050::set_register(const uint8_t &regAddr, const uint8_t & data){
	hwlib::cout << "couting data from register: " << data << '\n';
	uint8_t _data[] = {regAddr, data};
	hwlib::cout << "setting register 0x" <<hwlib::hex << regAddr << " with value: " << data << '\n';
	bus.write(chipAddr, _data, 2);
}

int16_t i2c_mpu6050::read_bytes(const uint8_t &regAddr){
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
		hwlib::cout << "Setting calibrate values by user defined.\n";
		acXoffset = accelXoffset;
		acYoffset = accelYoffset;
		acZoffset = accelZoffset;
		GyXoffset = GyroXoffset;
		GyYoffset = GyroYoffset;
		GyZoffset = GyroZoffset;
}

void i2c_mpu6050::display_calibrate_values(){
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