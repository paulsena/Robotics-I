/***************************************************************************
Paul Senatillaka
Ramon Ramirez
Helper Function Library
***************************************************************************/
//Helper Move Functions
void moveLeft(int vel) {
	mav(lMotor, vel/2);
	mav(rMotor, vel);
	sleep(turnTime);
};

void moveRight(int vel) {
	mav(lMotor, vel);
	mav(rMotor, vel/2);
	sleep(turnTime);
};

void moveForward(int vel) {
	mav(lMotor, vel);
	mav(rMotor, vel);
};

//If bumpers are hit, moves robot
void bumperChecknMove() {
	/****** Bumper Logic *******/
	//If left bumper is hit (or if both sensors are hit)
	if(digital(lBumper)) {
		printf("Left or Both Bumpers Hit!\n");
		//Backup
		moveForward(-maxVelocity);
		sleep(backupTime);
		//Move Right
		mav(lMotor, maxVelocity);
		mav(rMotor, -maxVelocity);
		sleep(turnTime*2);

	}
	//If just right bumper is hit
	else if(digital(rBumper)) {
		printf("Right Bumper Hit!\n");
		//Backup
		moveForward(-maxVelocity);
		sleep(backupTime);
		//Move Left
		mav(lMotor, -maxVelocity);
		mav(rMotor, maxVelocity);
		sleep(turnTime*2);
	}
	//No Sensor is Hit
	else {
		//Move Forward
		//moveForward(velocity);
	}
};
