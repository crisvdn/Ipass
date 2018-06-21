#include "i2c_mpu6050.hpp"

void i2c_mpu6050::set_register(const uint8_t &regAddr, const uint8_t & data){
	uint8_t _data[] = {regAddr, data};
	bus.write(chipAddr, _data, 2);
}

uint16_t i2c_mpu6050::read_values(const uint8_t &regAddr){
	uint8_t _regData[] = {regAddr};
	uint8_t _data[2];
	bus.write(chipAddr,_regData,1);
	bus.read(chipAddr, _data, 2);
	return ((_data[0] << 8) | _data[1]);
}
