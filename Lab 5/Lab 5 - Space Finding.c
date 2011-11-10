/***************************************************************************
Paul Senatillaka
Ramon Ramirez
Lab 5
Space Finding Robot
***************************************************************************/
//Sensor Ports Settings
const int lMotor = 0;
const int rMotor = 3;
const int lBumper = 14;
const int rBumper = 15;
const int sonarSensor = 0;
const int servo = 0;

//Movement Variables
const float turnTime = .5;
const float backupTime = 1;
const int maxVelocity = 400;

//External Helper file for Movement and Bumper Functions
#include "Helper.h";


/*
* findOpenSpace()
* @Description: Scans at 5 servo positions ranging from 20 to 2028 (So we aren't at max servo bounds). Readings at: 2028, 1526, 1024, 522, 20
* @Returns: Servo Position at whih max distance was found (Possible values from 1 to 5).
*/
 int findOpenSpace() {

	int i = 0;
	int servoPos = 20;
	int servoPosMax = 0;
	int sonarReading = 0;
	int sonarReadingMax = 0;
	  
	//Find Max Distance Loop
	for (i=1; i<=5; i++) {
		
		bumperChecknMove();
	
		//Move Servo to Position
		set_servo_position(servo, servoPos);
		if (i==1)
			sleep(.6);
		else 
			sleep(.3);
		//Capture Sonar Reading
		sonarReading = analog(sonarSensor);
		printf ("Sonar Reading: %d  Servo Position: %d\n", sonarReading, servoPos);
		
		//Find Sonar Max Reading and What Position It was at
		if ( sonarReading > sonarReadingMax) {
			sonarReadingMax = sonarReading; 
			servoPosMax = i;
		}
				
		//Increment the servo Position.
		servoPos += 502;
	}	
	
	disable_servos();
	
	return servoPosMax;
}

/*
* move()
* @Description: Moves the robot depending on 1 of 5 position states given
* @param: int position - Position state. 1 through 5
* @param: int velocity - Velocity of moving
*/
void move(int position, int velocity) {
	
	switch(position) {
		//Right
		case 1:
			mav(lMotor, velocity/1.5); //267
			mav(rMotor, velocity/5);  //80
			break;
		//Diagonal Right
		case 2:
			mav(lMotor, velocity);  //400
			mav(rMotor, velocity/1.5);  //267
			break;		
		//Center
		case 3:
			mav(lMotor, velocity);  
			mav(rMotor, velocity);
			break;		
		//Diagonal Left
		case 4:
			mav(lMotor, velocity/1.5);
			mav(rMotor, velocity);
			break;		
		//Left
		default:
			mav(lMotor, velocity/5);
			mav(rMotor, velocity/1.5);
			break;		
	}

}

int main()
{

	//variable declarations go here.
	set_each_analog_state(1,0,0,0,0,0,0,0);// This line sets analog ports to be pullup (0) or floating (1)
	enable_servos();
	sleep(.5);
	
	int movePosition = 3;
	int lastState = 0;
	
	while (1) {

		//Max Distance Found, Now Move there
		movePosition = findOpenSpace();
		
		printf("Max Distance Position: %d\n", movePosition); 
		
		move(movePosition, maxVelocity);
		
		//sleep(turnTime);
		
		bumperChecknMove();
		
 	}
}
