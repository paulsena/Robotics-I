/***************************************************************************
Paul Senatillaka
Ramon Ramirez
Lab 2
Bumper Code
***************************************************************************/
const short lMotor = 3;
const short rMotor = 0;
const short lSensor = 15;
const short rSensor = 14;
const float turnTime = .5;
const float backupTime = 1;
const short velocity = 300;

//Helper Move Functions
void moveLeft(short vel) {
	printf("Left\n");
	mav(lMotor, -vel);
	mav(rMotor, vel);
	sleep(turnTime);
}
void moveRight(short vel) {
	printf("Right\n");
	mav(lMotor, vel);
	mav(rMotor, -vel);
	sleep(turnTime);
}

void moveForward(short vel) {
	printf("Forward\n");
	mav(lMotor, vel);
	mav(rMotor, vel);
}

int main()
{
	
	while(1) {
	
		//If left bumper is hit (or if both sensors are hit)
		if(digital(lSensor)) {
			//Backup
			bk(lMotor); bk(rMotor);
			sleep(backupTime);
			//Move Right
			moveRight(velocity);
			sleep(turnTime);
		}
		//If just right bumper is hit
		else if(digital(rSensor)) {
			//Backup
			bk(lMotor); bk(rMotor);
			sleep(backupTime);
			//Move Left
			moveLeft(velocity);
			sleep(turnTime);
		}
		//No Sensor is Hit
		else {
			//Move Forward
			moveForward(velocity);
		}
		
	}
}
