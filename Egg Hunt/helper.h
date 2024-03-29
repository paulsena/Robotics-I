/***************************************************************************
Paul Senatillaka
Ramon Ramirez
Helper Function Library
***************************************************************************/

//****Sensor Ports Settings****
//Motor Ports
#define lMotor 3
#define rMotor 0
#define trackMotor 1
//Digital Ports
#define groundSensor 8
#define trackLimit 9
#define lBumper 15
#define rBumper 14
//Analog Ports
#define lLight 1
#define rLight 0
#define lDistSensor 3
#define rDistSensor 2
//Servo Ports
#define fGate 0
//*****************************

//Light Thresholds
#define lightThreshHigh 120
#define lightThreshLow 7

//Dist Thresholds
#define distThreshold 850

//Gate Servo Settings
#define GateOpen 670
#define GateClose 1425
#define GateRightSweep 1900
#define GateLeftSweep 848

//Movement Variables
const int maxVelocity = 1000;

//Direction variables
enum DIRECTION {pointRight, right, diagRight, forward, diagLeft, left, pointLeft, oneEighty, back};

//Color variables
enum COLOR {EXTRA, YELLOW, BONUS, PURPLE};

//Declerations
enum STATE {COLLECT, FINDGOAL, DRIVEGOAL, DUMP };


/*
* move()
* @Description: Moves the robot depending on 1 of 5 position states given
* @param: int position - Position state. 1 through 5
* @param: int velocity - Velocity of moving
*/
void move(enum DIRECTION moveDirection, int velocity) {
	
	switch(moveDirection) {
		
		//Point Turn Right
		case 0:
			mav(lMotor, velocity * .5); 
			mav(rMotor, -velocity * .5);  
			#ifdef DEBUG
				printf("Point Turn Right \n");
			#endif
			break;
		//Right
		case 1:
			mav(lMotor, velocity * .5); 
			mav(rMotor, 0);  
			#ifdef DEBUG
				printf("Right \n");
			#endif
			break;
		//Diagonal Right
		case 2:
			mav(lMotor, velocity);
			mav(rMotor, velocity * .25);
			#ifdef DEBUG
				printf("Diag Right \n");
			#endif	
			break;
		//Center
		case 3:
			mav(lMotor, velocity);  
			mav(rMotor, velocity);
			#ifdef DEBUG
				printf("Center \n");
			#endif
			break;		
		//Diagonal Left
		case 4:
			mav(lMotor, velocity * .25);
			mav(rMotor, velocity);
			#ifdef DEBUG
				printf("Diag Left \n");
			#endif
			break;		
		//Left
		case 5:
			mav(lMotor, 0);
			mav(rMotor, velocity * .5);
			#ifdef DEBUG
				printf("Left \n");
			#endif
			break;		
		//Point Turn Left
		case 6:
			mav(lMotor, -velocity * .5);
			mav(rMotor, velocity * .5);
			#ifdef DEBUG
				printf("Point Turn Left \n");
			#endif
			break;
		//180 Turns Left
		case oneEighty:
				
			//Find open side before spinning
			if ( analog10(lDistSensor) > analog10(rDistSensor) ) {
				//turn left
				mav(lMotor, -1024 );
				mav(rMotor, 1024 );
			}
			else {
				//turn right
				mav(lMotor, 1024 );
				mav(rMotor, -1024 );
			}

			sleep(1.9);
			mav(lMotor, 0);
			mav(rMotor, 0);
			break;
		//Back
		case back:
			mav(lMotor, -velocity);
			mav(rMotor, -velocity);
			break;
	}

}

void stopDriving() {
	mav(lMotor, 0);
	mav(rMotor, 0);
}

//If bumpers are hit, moves robot
void bumperChecknMove() {
	/****** Bumper Logic *******/
	//If left bumper is hit (or if both sensors are hit)
	if(digital(lBumper)) {
		printf("Left or Both Bumpers Hit!\n");
		//Backup
		move(back, maxVelocity);
		sleep(.7);
		//Move Right
		move(pointRight, maxVelocity);
		sleep(.7);
		stopDriving();

	}
	//If just right bumper is hit
	else if(digital(rBumper)) {
		printf("Right Bumper Hit!\n");
		//Backup
		move(back, maxVelocity);
		sleep(.7);
		//Move Left
		move(pointLeft, maxVelocity);
		sleep(.7);
		stopDriving();
	}
	//No Sensor is Hit
	else {

	}
}

void sideDistCheckMove() {
	
	if ( analog10(lDistSensor) > distThreshold ) {
		#ifdef DEBUG
			printf("Left too close \n");
		#endif
		move(diagRight, maxVelocity);
		sleep(1);	

	}
	else if ( analog10(rDistSensor) > distThreshold ) { 
		#ifdef DEBUG
			printf("Right too close \n");
		#endif
		move(diagLeft, maxVelocity);
		sleep(1);	
		
	}
	
}

