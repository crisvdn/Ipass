#include "hwlib.hpp"
#include "i2c_mpu6050.hpp"
int roll = 0, pitch = 0;

int main(){
	uint8_t MPU_addr = 0x68;
	hwlib::wait_ms(500);
	WDT->WDT_MR = WDT_MR_WDDIS;
	namespace target = hwlib::target;
	auto scl = target::pin_oc(target::pins::d9);
	auto sda = target::pin_oc(target::pins::d8);
	//Creating bus with scl, sda pins.
	auto bus = hwlib::i2c_bus_bit_banged_scl_sda(scl, sda);
	
	//creating oled, font and display to visualize data.
	auto oled = hwlib::glcd_oled( bus, 0x3c ); 
	auto font = hwlib::font_default_8x8();
	auto display = hwlib::window_ostream( oled, font );
	
	//creating mpu6050 object. assigning address & bus.
	auto mpu = i2c_mpu6050(bus, MPU_addr); 
	mpu.show_calibrate_values();
	//gyro.display_roll_pitch();
	//mpu.get_gyro_scale();
	mpu.display_values();
//	while(1){
//		roll = gyro.read_roll();
//		pitch = gyro.read_pitch();
//		display << "\f roll: " << "\t0900" <<roll << "\n pitch: "<<"\t0901" <<  pitch << hwlib::flush;
//	}
}