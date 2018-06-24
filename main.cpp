#include "hwlib.hpp"
#include "i2c_mpu6050.hpp"

int main(){
	uint8_t MPU_addr = 0x68;
	hwlib::wait_ms(500);
	WDT->WDT_MR = WDT_MR_WDDIS;
	namespace target = hwlib::target;
	auto scl = target::pin_oc(target::pins::d9);
	auto sda = target::pin_oc(target::pins::d8);
	auto bus = hwlib::i2c_bus_bit_banged_scl_sda(scl, sda);
	auto gyro = i2c_mpu6050(bus, MPU_addr);
	gyro.show_calibrate_values();
	//gyro.display_values();
	gyro.display_rpy();
	//gyro.display_raw_values();
}