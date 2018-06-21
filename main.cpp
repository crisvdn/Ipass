#include "hwlib.hpp"
#include "i2c_mpu6050.hpp"

uint16_t AcX, AcY, AcZ, Ty = 0x00;
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
	hwlib::cout << "Acc X: "  << " Acc Y: " << " Acc Z: "<< " Temperature: " << hwlib::setw(16) <<"X axis: "<< hwlib::setw(20) <<" Y axis: "<< hwlib::setw(24) <<" Z axis: " << '\n';
	while(1){
		hwlib::wait_ms(500);
		//gyro.set_register(0x6B);
		AcX = gyro.read_values(0x3B);
		AcY = gyro.read_values(0x3D);
		AcZ = gyro.read_values(0x3E);
		Ty = gyro.read_values(0x41);
		hwlib::cout << hwlib::dec <<hwlib::setw(1) << AcX << ' '; 
		hwlib::cout << hwlib::dec <<hwlib::setw(5) << AcY << ' '; 
		hwlib::cout << hwlib::dec <<hwlib::setw(9) << AcZ << ' ';
		hwlib::cout << hwlib::dec <<hwlib::setw(14) << (Ty / 340) + 36 << '\n';
	}
}