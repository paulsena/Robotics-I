/***************************************************************************
Paul Senatillaka
Ramon Ramirez
Lab 3
Light Following Robot
***************************************************************************/

//Sensor Ports
const int lMotor = 0;
const int rMotor = 3;
const int lSensor = 6;
const int rSensor = 7;
const int lBumper = 14;
const int rBumper = 15;

//Movement Variables
const float turnTime = .2;
const float backupTime = 1;
const int maxVelocity = 800;
const int threshold = 10;

//Light Sensor Variables
int ambientLightLevel;


//Helper Move Functions, Only used for Bumper Code
void moveLeft(int vel) {
	mav(lMotor, -vel);
	mav(rMotor, vel);
	sleep(turnTime);
}

void moveRight(int vel) {
	mav(lMotor, vel);
	mav(rMotor, -vel);
	sleep(turnTime);
}

void moveForward(int vel) {
	mav(lMotor, vel);
	mav(rMotor, vel);
}

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
		moveRight(maxVelocity/2);
		sleep(turnTime);
		moveForward(maxVelocity);
		sleep(backupTime/2);

	}
	//If just right bumper is hit
	else if(digital(rBumper)) {
		printf("Right Bumper Hit!\n");
		//Backup
		moveForward(-maxVelocity);
		sleep(backupTime);
		//Move Left
		moveLeft(maxVelocity/2);
		sleep(turnTime);
		moveForward(maxVelocity);
		sleep(backupTime/2);
	}
	//No Sensor is Hit
	else {
		//Move Forward
		//moveForward(velocity);
	}
}

//Dynamic calibration of ambient light variables for different environments
void calibrateLightSensors() {
	
	//Calibrate Robot Sensor Values
	printf("Please A to set the Ambient Light Value \n");
	while(!a_button()) {
	}
	ambientLightLevel = (analog(lSensor) + analog(rSensor)) / 2;
	printf("Ambient Light Level Set to: %d \n", ambientLightLevel);
	
	sleep(1);
	printf("Please A start the Robot! \n");
	while(!a_button()) {
	}
}

//Normalization function
//Translates light readings into velocity values
int lightToVelocity(int lightSensor) {
	
	//Based off our max speed, get the multiplier to apply to our 256 range
	int velMultiplier = maxVelocity / 255;
	
	if ( analog(lightSensor) < ambientLightLevel - threshold && analog(lightSensor) >= 5) {
		
		return maxVelocity - (velMultiplier * analog(lightSensor));
	}
	
	return 0;
}


int main()
{
	//Sets the sensors with w/o a resistor properly. IE: Our Light sensors
	set_each_analog_state(0,0,0,0,0,0,0,0);
	sleep(.5);
	
	//Dynamically calibrate our min max light values
	calibrateLightSensors();
	
	//Main loop
	while(1) {
	
		//Set left and right motor velocities based off sensor readings
		int rMotorVel = lightToVelocity(lSensor);
		int lMotorVel = lightToVelocity(rSensor);
		
		mav(lMotor, lMotorVel);
		printf("Left motor vel: %d",lMotorVel);
		
		mav(rMotor, rMotorVel);
		printf("Right motor vel: %d \n",rMotorVel);
				
		sleep(.5);
		
		bumperChecknMove();
	
	}
	
}
