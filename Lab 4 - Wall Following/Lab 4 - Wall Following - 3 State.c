/***************************************************************************
Paul Senatillaka
Ramon Ramirez
Lab 4
Wall Following Robot - 3 States
***************************************************************************/
//Sensor Ports
const int lMotor = 0;
const int rMotor = 3;
const int fDistSensor = 0;
const int rDistSensor = 1;
const int lBumper = 14;
const int rBumper = 15;

//Movement Variables
const float turnTime = .2;
const float backupTime = 1;
const int maxVelocity = 1000;
const int distThreshold = 300;

//External Helper file for Movement and Bumper Functions
#include "Helper.h";

int main()
{
	
	//variable declarations go here.
	set_each_analog_state(1,1,0,0,0,0,0,0);// This line sets analog ports to be pullup (0) or floating (1)
	sleep(.5);
	
	while (1) {
		
		int rDistance = analog10(rDistSensor);
		int fDistance = analog10(fDistSensor);
		
		printf("Right Distance: %d \n" , rDistance);
		printf("Front Distance: %d \n" , fDistance);
		
		//Something in Front Failsafe Check
		if( fDistance > distThreshold ) {
			//Point turn Left
			mav(lMotor, -maxVelocity);
			mav(rMotor, maxVelocity);
			sleep(turnTime*2);
		}
		//State 1: Too Close. Threshold to Max Proximity
		else if( rDistance > distThreshold ) {
			printf("Too Close\n");
			moveLeft(maxVelocity);
		}
		//State 2: Just Right.  Threshold-100 to Threshold
		else if(rDistance > distThreshold - 100) {
			printf("Just Right\n");
			moveForward(maxVelocity);
		}
		//State 2: Too Far.  Min Proximity to Threshold-100
		else {
			printf("Too Far\n");
			moveRight(maxVelocity);
		}
	
		bumperChecknMove();
		
		
 	}
	
}
