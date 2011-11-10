/***************************************************************************
Paul Senatillaka
Ramon Ramirez
Lab 2
Line Following Code
***************************************************************************/
/* #defines and constants go below.*/
const short lMotor = 3;
const short rMotor = 0;
const short velocity = 300;
const short lineThreshold = 850;
const float turnTime = .1;
const float forwardTime = .05;

/*Function Declarations*/
void moveForward(short);
void moveRight(short);
void moveLeft(short);
short checkSensorBlack(short motorPort);

/*Function Definitions*/
int main()
{
	int moveFowardCntr = 0;
	moveForward(velocity);
	
	while(  1 ) {

		//Just Left Hit Black
		if(checkSensorBlack(lMotor) && !checkSensorBlack(rMotor)) {
			moveLeft(velocity);
			moveFowardCntr = 0;
			printf("Left Hit\n");
		}
		//Just Right Hit Black
		else if(checkSensorBlack(rMotor) && !checkSensorBlack(lMotor)) { 
			moveRight(velocity);
			moveFowardCntr = 0;
			printf("Right Hit\n");
		}
		//Both Hit Black
		else if(checkSensorBlack(lMotor) && checkSensorBlack(rMotor)) {  
			moveForward(velocity);
			moveFowardCntr = 0;
			printf("Both Black, Moving Foward\n");
		}
		//Both Hit White
		else {  
			//Move Foward for only a few iterations
			if (moveFowardCntr <= 4) {
				moveForward(velocity);
				printf("Both White, Moving Forward\n");
			}
			//Move Right to look for line, since either sensor has not seen black in a few iterations
			else { 
				moveRight(velocity);
				printf("Finding Line\n");
			}
			moveFowardCntr++;
		}
		
		
	}
}

//Helper Functions
void moveLeft(short vel) {
	mav(lMotor, -vel/2.5);
	mav(rMotor, vel/1.5);
	sleep(turnTime);
}
void moveRight(short vel) {
	mav(lMotor, vel/1.5);
	mav(rMotor, -vel/2.5);
	sleep(turnTime);
}

void moveForward(short vel) {
	mav(lMotor, vel);
	mav(rMotor, vel);
	sleep(forwardTime);
}

short checkSensorBlack(short motorPort) {
	return analog10(motorPort) > lineThreshold;
}
