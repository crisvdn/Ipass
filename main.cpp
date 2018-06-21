#include "hwlib.hpp"
#include "i2c_mpu6050.hpp"

uint16_t showCaseData, sleepBit = 0x00;
//const uint8_t emptyValue = 0x00;

int main(){
	uint8_t MPU_addr = 0x68;
	hwlib::wait_ms(500);
	WDT->WDT_MR = WDT_MR_WDDIS;
	namespace target = hwlib::target;
	auto scl = target::pin_oc(target::pins::scl);
	auto sda = target::pin_oc(target::pins::sda);
	auto bus = hwlib::i2c_bus_bit_banged_scl_sda(scl, sda);
	auto gyro = i2c_mpu6050(bus, MPU_addr);
	gyro.display_raw_values();
	//showCaseData = gyro.read_word(0x1C);
	//sleepBit = gyro.read_word(0x6B);
	//hwlib::cout <<"showing data: "<< hwlib::dec << showCaseData << '\n';
	//hwlib::cout <<"showing sleepbit: "<< hwlib::dec << sleepBit << '\n';
}