#include "hwlib.hpp"
#include "i2c_mpu6050.hpp"

int give_score(int value){
	int counter = 0;
	int maximumValue = 32000;
	for(int i = value; i < maximumValue; i += 320){
		if(i < maximumValue){
			counter ++;
		}
	}
	return counter-80;
}

int roll = 0, pitch = 0;
int accelX = 0, accelY = 0, accelZ = 0;
int gAccelX = 0, gAccelY = 0, gAccelZ = 0;
int highestGX = 0, highestGY = 0, highestGZ =0;
int highestAccelX = 0, highestAccelY = 0, highestAccelZ = 0;
int lowestAccelX = 0, lowestAccelY = 0, lowestAccelZ = 0;
int state = 0, score_x = 0, score_y = 0, score_z = 0;
int prev_score_x = 0, prev_score_y = 0, prev_score_z = 0;


void reset_variables(){
	highestAccelX = 0, highestAccelY = 0, highestAccelZ = 0;
	lowestAccelX = 0, lowestAccelY = 0, lowestAccelZ = 0;
	highestGX = 0, highestGY = 0, highestGZ = 0;
	prev_score_x = 0, prev_score_y = 0, prev_score_z = 0;
	score_x = 0, score_y = 0, score_z = 0;
}

int main(){
	uint8_t MPU_addr = 0x68;
	hwlib::wait_ms(500);
	//disable watchdog.
	WDT->WDT_MR = WDT_MR_WDDIS;
	namespace target = hwlib::target;
	
	//creating buttons to switch between display modes.
	auto button_forward = target::pin_in(target::pins::d5);
	auto button_backward = target::pin_in(target::pins::d4);
	
	//creating scl & sda pins.
	auto scl = target::pin_oc(target::pins::d9);
	auto sda = target::pin_oc(target::pins::d8);
	//Creating bus with scl, sda pins.
	auto bus = hwlib::i2c_bus_bit_banged_scl_sda(scl, sda);
	
	//creating oled, font and display to visualize data.
	auto oled = hwlib::glcd_oled( bus, 0x3c ); 
	auto font = hwlib::font_default_8x8();
	auto display = hwlib::window_ostream( oled, font );
	display << "\t0900" << "\f Initializing..." << "\t0902" <<  "\n Calibrating.." << hwlib::flush;
	
	//creating mpu6050 object. assigning address & bus.
	auto mpu = i2c_mpu6050(bus, MPU_addr);
	mpu.display_calibrate_values();
	mpu.display_values();
	while(1){
		//creating simple menu to switch between displays with 2 buttons.
		if(!button_forward.get()){
			state ++;
			hwlib::wait_ms(200);
		}if(!button_backward.get()){
			if(!state == 0){
				state --;
			}
			hwlib::wait_ms(200);
		}
		if(state ==0){
			roll = mpu.read_roll();
			pitch = mpu.read_pitch();
			display << "\f roll: " << "\t0900" <<roll << "\n \n pitch: "<<"\t0902" <<  pitch << "\t1307" << state << "/5" << hwlib::flush;
		}else if(state == 1){
			accelX = mpu.read_accelX();
			accelY = mpu.read_accelY();
			accelZ = mpu.read_accelZ();
			display << "\f accel x: " << "\t0900" << accelX << "\n accel y: " << "\t0901 " << accelY <<
			"\n accel z: " << "\t0902" << accelZ  << "\t1307" << state << "/5" << hwlib::flush;
		}else if(state == 2){
			accelX = mpu.read_accelX();
			accelY = mpu.read_accelY();
			accelZ = mpu.read_accelZ();
			//if statements to save the highest measured raw accelerometer value.
			if(accelX > highestAccelX){
				highestAccelX = accelX;
			}
			if(accelY > highestAccelY){
				highestAccelY = accelY;
			}
			if(accelZ > highestAccelZ){
				highestAccelZ = accelZ;
			}
			display << "\t0901" << "\f Highest accel value: " << "\n accel x: " << "\t0901" << highestAccelX << "\n accel y: " << "\t0902 " << highestAccelY <<
			"\n accel z: " << "\t0903" << highestAccelZ << "\t1307" << state << "/5" << hwlib::flush;
		}else if(state == 3){
			accelX = mpu.read_accelX();
			accelY = mpu.read_accelY();
			accelZ = mpu.read_accelZ();
			//if statements to save the lowest measured raw accelerometer value.
			if(accelX < lowestAccelX){
				lowestAccelX = accelX;
			}
			if(accelY < lowestAccelY){
				lowestAccelY = accelY;
			}
			if(accelZ < lowestAccelZ){
				lowestAccelZ = accelZ;
			}
			
			display << "\t0901" << "\f Lowest accel value: " << "\n accel x: " << "\t0901" << lowestAccelX << "\n accel y: " << "\t0902 " << lowestAccelY <<
			"\n accel z: " << "\t0903" << lowestAccelZ << "\t1307" << state << "/5" << hwlib::flush;
		}else if(state == 4){
			//if statement to save the highest measured filtered accelerometer value in G.
			if(gAccelX > highestGX){
				highestGX = gAccelX;
			}
			if(gAccelY > highestGY){
				highestGY = gAccelY;
			}
			if(gAccelZ > highestGZ){
				highestGZ = gAccelZ;
			}
			mpu.read_values(gAccelX, gAccelY, gAccelZ);
			display << "\t0901" << "\f accel in G " << "\n accel x: " << "\t0901" << highestGX << "\n accel y: " << "\t0902 " << highestGY <<
			"\n accel z: " << "\t0903" << highestGZ << "\t1307" << state << "/5" << hwlib::flush;
		}else if(state == 5){
			accelX = mpu.read_accelX();
			accelY = mpu.read_accelY();
			accelZ = mpu.read_accelZ();
			score_x = give_score(accelX);
			score_y = give_score(accelY);
			score_z = give_score(accelZ);
			if(score_x > prev_score_x)
				prev_score_x = score_x;
			if(score_y > prev_score_y)
				prev_score_y = score_y;
			if(score_z > prev_score_z)
				prev_score_z = score_z;
			display << "\t0901" << "\f Scores: " << "\n accel x: " << "\t0901" << prev_score_x << "\n accel y: " << "\t0902 " << prev_score_y <<
			"\n accel z: " << "\t0903" << prev_score_z << "\t1307" << state << "/5" << hwlib::flush;
		}else if(state == 6){
			display << "\t0901" << "\f Reset values?" << hwlib::flush;
		}else if(state == 7){
			reset_variables();
			state = 2;
		}
	}
}