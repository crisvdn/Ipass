#include "i2c_mpu6050.hpp"

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

void i2c_mpu6050::display_raw_values(){
	hwlib::cout << "Displaying raw values.\n";
	uint16_t AcX, AcY, AcZ, Ty, GyX, GyY, GyZ = 0x00;
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
		hwlib::cout << hwlib::dec <<hwlib::setw(16) << (Ty / 340) + 36 << ' ';
		hwlib::cout << hwlib::dec <<hwlib::setw(17) << GyX << ' ';
		hwlib::cout << hwlib::dec <<hwlib::setw(20) << GyY << ' ';
		hwlib::cout << hwlib::dec <<hwlib::setw(23) << GyZ << '\n';
	}
}

void i2c_mpu6050::display_values(){
	hwlib::cout << "Displaying accelerometer values in G/s and gyro axis values in degrees/sec .\n";
	uint16_t AcX, AcY, AcZ, Ty, GyX, GyY, GyZ = 0x00;
	uint16_t accelX, accelY, accelZ, gyroX, gyroY, gyroZ = 0x00;
	header_values();
	while(1){
	AcX = read_values(0x3B);
	AcY = read_values(0x3D);
	AcZ = read_values(0x3E);
	Ty = read_values(0x41);
	GyX = read_values(0x43);
	GyY = read_values(0x45);
	GyZ = read_values(0x47);
	accelX = AcX / 16384;
	accelY = AcY / 16384;
	accelZ = AcZ / 16384;
	gyroX = GyX / 131;
	gyroY = GyX / 131;
	gyroZ = GyX / 131;
	hwlib::cout << hwlib::dec <<hwlib::setw(1) << accelX << ' '; 
	hwlib::cout << hwlib::dec <<hwlib::setw(8) << accelY << ' '; 
	hwlib::cout << hwlib::dec <<hwlib::setw(13) << accelZ << ' ';
	hwlib::cout << hwlib::dec <<hwlib::setw(16) << (Ty / 340) + 36 << ' ';
	hwlib::cout << hwlib::dec <<hwlib::setw(17) << gyroX << ' ';
	hwlib::cout << hwlib::dec <<hwlib::setw(20) << gyroY << ' ';
	hwlib::cout << hwlib::dec <<hwlib::setw(23) << gyroZ << '\n';
	}
}


void i2c_mpu6050::set_register(const uint8_t &regAddr, const uint8_t & data){
	hwlib::cout << "couting data from register: " << data << '\n';
	uint8_t _data[] = {regAddr, data};
	hwlib::cout << "setting register 0x" <<hwlib::hex << regAddr << " with value: " << data << '\n';
	bus.write(chipAddr, _data, 2);
}

uint16_t i2c_mpu6050::read_values(const uint8_t &regAddr){
	uint8_t _regData[] = {regAddr};
	uint8_t _data[2];
	bus.write(chipAddr,_regData,1);
	bus.read(chipAddr, _data, 2);
	return ((_data[0] << 8) | _data[1]);
}

uint8_t i2c_mpu6050::read_word(const uint8_t &regAddr){
	uint8_t _regData[] = {regAddr};
	uint8_t _data[1];
	bus.write(chipAddr,_regData,1);
	bus.read(chipAddr, _data, 1);
	return _data[0];
}