#include "i2c_mpu6050.hpp"

//Private functions

void i2c_mpu6050::initialize(){
	hwlib::cout << "Initializing...\n";
	set_register(0x6B, 0x00); //Set sleep bit to 0. This will activate the chip.
	set_register(0x1C, 0xF0); //Set Accel configuration to +-4g Acceleration sensitivity.
	set_register(0x1B, 0x00); //set Gyro configuration to 250 degrees/s
}

void i2c_mpu6050::header_values(){
	hwlib::cout << hwlib::setw(1) <<  "Acc X: "  << hwlib::setw(8)<< " Acc Y: " <<
	hwlib::setw(13)<< " Acc Z: "<<hwlib::setw(16)<< " Temperature: " <<
	hwlib::setw(17) <<"X axis: "<< hwlib::setw(20) <<" Y axis: "<<
	hwlib::setw(23) <<" Z axis: " << '\n';
}

void i2c_mpu6050::calibrate(){
	hwlib::cout << "Calibrating device... Don't move the MPU-6050! \n";
	int32_t AcX, AcY, AcZ, GyX, GyY, GyZ;
	hwlib::wait_ms(500);
	for(unsigned int i=0; i < 200; i++){
		AcX = read_values(0x3B);
		acXoffset += (AcX / 8192);
		AcY = read_values(0x3D);
		acYoffset += (AcY / 8192);
		AcZ = read_values(0x3E);
		acZoffset += (AcZ / 8192);
		GyX = read_values(0x43);
		GyXoffset += (GyX / 131);
		GyY = read_values(0x45);
		GyYoffset += (GyY / 131);
		GyZ = read_values(0x47);
		GyZoffset += (GyZ / 131);
		hwlib::cout<<hwlib::dec << "AcZ: " << AcZ << '\n';
	}
	hwlib::cout<<hwlib::dec << "GyY: " << GyY << '\n';
	hwlib::cout<<hwlib::dec << "GyY: " << (GyY / 10) << '\n';
	hwlib::cout<<hwlib::dec << "acZoffset: " << acZoffset << '\n';
	acXoffset = (acXoffset / 200);
	acYoffset = (acYoffset / 200);
	acZoffset = (acZoffset / 200);
	GyXoffset = (GyXoffset / 200);
	GyYoffset = (GyYoffset / 200);
	GyZoffset = (GyZoffset / 200);
	hwlib::cout <<hwlib::dec << hwlib::showbase << "acZoffset: " << acZoffset << '\n';
	hwlib::cout <<hwlib::dec << hwlib::showbase <<"acYoffset: " << acYoffset << '\n';
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
		AcZ = read_values(0x3E);
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
	AcZ = read_values(0x3E);
	Ty = read_values(0x41);
	GyX = read_values(0x43);
	GyY = read_values(0x45);
	GyZ = read_values(0x47);
	accelX = (AcX - acXoffset) / 8192;
	accelY = (AcY - acYoffset) / 8192;
	accelZ = ((AcZ - acZoffset) / 8192);
	gyroX = (GyX - GyXoffset) / 131;
	gyroY = (GyX - GyYoffset) / 131;
	gyroZ = (GyX - GyZoffset) / 131;
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
	uint16_t AcX, AcY, AcZ, Ty, GyX, GyY, GyZ = 0x00;
	while(1){
		AcX = read_values(0x3B);
		AcY = read_values(0x3D);
		AcZ = read_values(0x3E);
		Ty = read_values(0x41);
		GyX = read_values(0x43);
		GyY = read_values(0x45);
		GyZ = read_values(0x47);
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

void i2c_mpu6050::set_calibrate_values(const int16_t & acXoffset, const int16_t & acYoffset, 
	const int16_t & acZoffset, const int16_t & GyXoffset,
	const int16_t & GyYoffset, const int16_t & GyZoffset){
		i2c_mpu6050::acXoffset = acXoffset;
		i2c_mpu6050::acYoffset = acYoffset;
		i2c_mpu6050::acZoffset = acZoffset;
		i2c_mpu6050::GyXoffset = GyXoffset;
		i2c_mpu6050::GyYoffset = GyYoffset;
		i2c_mpu6050::GyZoffset = GyZoffset;
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