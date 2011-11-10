/***************************************************************************
Paul Senatillaka
Ramon Ramirez
Lab 6
Vision Tracking - Pneumatics Firing Shovel Robot
Description: Tracks a specific colored blob, using planar vision. When it gets close,
it activates a pneumatic flipper (controlled by 3 small pistons), to hit and flip the
blob in the air. The robot also has a pump mechanism to refill the pneumatic tanks.
                  
Note: Camera Pixel Dimensions: 159 x 119
***************************************************************************/

#include "Helper.h"

int main()
{

	//Init Variables
	int color = RED;
	int lastSeen = right;
	int pumpTime = 0;
	
	//Enable Servos
	enable_servos();
	sleep(.2);
	beep();
	
	while(1) {
		
		//Check if we have time left on our Pump Counter
		if(pumpTime > 0) {
			//Turn Pump On
			set_servo_position(servoPump, pumpOn);
		}
		else {
			//Turn Pump Off
			set_servo_position(servoPump, pumpOff);
		}
		
		//Image Capture
		track_update();
		
		//Check if We See a blob
		if(track_count(color) > 0 ) {
			
			//Find out where blobs are
			//Left
			if( track_x(color, 0) < 80) {
				move(diagLeft, 500);
				lastSeen = left;
			}
			//Right
			else if( track_x(color, 0) > 120 ) {
				move(diagRight, 500);
				lastSeen = right;
			}
			//Middle
			else {
				move(forward, 500);
			}
			
			//Check if the Blob is close
			if( track_bbox_top(color, 0) > 80) {
				
				//Motors Off
				ao();
				//Extend Arm (Triggers Pneumatics)
				set_servo_position(servoSwitch, 44);
				sleep(1);
				//Retract Arm (Triggers Pneumatics)
				set_servo_position(servoSwitch, 1024);
				sleep(1);
				
				//We need to refill tanks
				pumpTime += 100;
			}
			
			//****Screen Debug Info****
			//printf("red blobs: %d\n", track_count(color));
			printf("Blob X Cord: %d\n", track_x(color,0));
			printf("Blob Y Cord: %d\n", track_y(color,0));
			//printf("Blob Box Height: %d\n", track_bbox_height(color,0));
			//printf("Blob Box Width: %d\n", track_bbox_width(color,0));
			printf("track_size: %d\n", track_size(color, 0));
			printf( "top y: %d\n\n", track_bbox_top(color, 0));
					
		}
		//Else We didn't find a blob
		else{
			
			printf( "Don't see the color blob.\n" );
			// Search for ball using last known direction
			move(lastSeen, 500); 
			
		}
		
		//Decrement PumpTime
		if(pumpTime > 0) pumpTime--;
		
	}
	
}
