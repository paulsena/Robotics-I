/***************************************************************************
Paul Senatillaka
Ramon Ramirez
Helper Function Library
***************************************************************************/
//Sensor Ports Settings
const int lMotor = 0;
const int rMotor = 3;
const int lBumper = 14;
const int rBumper = 15;
const int servoSwitch = 3;
const int servoPump = 1;

//Pump Variables
const int pumpOn = 345;
const int pumpOff = 945;

//Movement Variables
const float turnTime = .5;
const float backupTime = 1;
const int maxVelocity = 400;

//Color variables
const int RED = 0;
const int YELLOW = 1;
const int GREEN = 2;
const int BLUE = 3;

//Direction variables
const int right = 1;
const int diagRight = 2;
const int forward = 3;
const int diagLeft = 4;
const int left = 5;

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
			mav(rMotor, 0);  //80
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
			mav(lMotor, 0);
			mav(rMotor, velocity/1.5);
			break;		
	}

}

//If bumpers are hit, moves robot
void bumperChecknMove() {
	/****** Bumper Logic *******/
	//If left bumper is hit (or if both sensors are hit)
	if(digital(lBumper)) {
		printf("Left or Both Bumpers Hit!\n");
		//Backup
		move(forward, -600);
		sleep(1);
		//Move Right
		mav(lMotor,  300);
		mav(rMotor, -300);
		sleep(.5);
		ao();

	}
	//If just right bumper is hit
	else if(digital(rBumper)) {
		printf("Right Bumper Hit!\n");
		//Backup
		move(forward, -600);
		sleep(1);
		//Move Left
		mav(lMotor, -300);
		mav(rMotor, 300);
		sleep(.5);
		ao();
	}
	//No Sensor is Hit
	else {
		//Move Forward
		//moveForward(velocity);
	}
};
