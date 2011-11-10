/*********************************
* Paul Senatillaka
* Ramon Ramirez
* Egg Hunt

**State machine**
States: Collect -> FindGoal -> Dump
                  
Note: Camera Pixel Dimensions: 159 x 119

Todo:
-Fix finding home from opponents side
-Add code for front closer
-Stay away from light when not going home
-Better search when no eggs seen. Currently just spins.
-When spinning, read side dist sensor to decide which way to turn

-Readjust 180 turn. Actually don't use discrete turns/positioning.
-Add side calibration
**********************************/
#define DEBUG

#include "helper.h";

//**** Variables ****
//Corrosponds to Top Hat Digital Reading
//White Side = 1, Dark Side = 0
const int homeColor = 0; 
const int awayColor = 1; 
const enum COLOR ourEggColor = PURPLE;
const int numBallsCollect = 3;
//*******************


int main()
{
	
	//Init Variables
	float startTime = seconds();
	enum STATE state = DUMP;
	int currentEggColor = ourEggColor;

	enum DIRECTION lastSeen = forward;
	int ballCount = 0;
	int dmpCnt = 0;
	
	//Enable our servos
	enable_servos();
	set_servo_position(fGate, GateClose);
	
	//Set our Distance Sensors to Float	
	set_each_analog_state(0,0,1,1,0,0,0,0);
	
	//Clears our track motor position.
	//Note: Pusher should always be set in the back position on start
	clear_motor_position_counter(trackMotor);
	
	//Possible Initial Vision Planning
	
	while(!black_button()) {
			
		
		sideDistCheckMove();
		
		bumperChecknMove();
		
		
		switch(state) {
			
		//**** Collection State *****
		//***************************
		case COLLECT:
			
			//Image Capture
			track_update();
		
			//Check for Bonus Eggs
			if ( track_count(BONUS) > 0 ) {
				currentEggColor = BONUS;
			}
			else {
				currentEggColor = ourEggColor;
			}		
			
			//Egg Detect
			if(track_count(currentEggColor) > 0 && track_confidence(currentEggColor, 0) > 40 ) {
				
				#ifdef DEBUG
					printf("See our egg %d \n", track_x( currentEggColor, 0));
				#endif
				
				//Find out where blobs are
				//Right
				if( track_x(currentEggColor, 0) > 115) {
					move(diagRight, 800);
					lastSeen = right;
				}
				//Left
				else if( track_x(currentEggColor, 0) < 85 ) {
					move(diagLeft, 800);
					lastSeen = left;
				}
				//Forward
				else {
					move(forward, 800);
										
				}
				
				//Check if ball is about to go in. 
				if( track_bbox_top(currentEggColor, 0) > 100) {
					
					//Open Front Gate
					set_servo_position(fGate, GateOpen);
						
					sleep(.5);
					ballCount++;
					beep();
					
					set_servo_position(fGate, GateClose);
					
					#ifdef DEBUG
					printf("Ball In: %d\n", ballCount);
					#endif
				}
				
			}
				
			//We didn't find a blob
			else {
				
				#ifdef DEBUG
					printf("No Egg \n");
				#endif
				
				//Move towards last seen direction
				move(lastSeen, maxVelocity);
				sleep(.5);
				
			}

			//TODO:
			//Obstacle Avoidance - Walls/Robots
			
			if (ballCount >= numBallsCollect) {
				state = FINDGOAL;
				ballCount = 0;
			}
			
			break;

		//***** Goal Finding State ******
		//*******************************
		case FINDGOAL:
		
			#ifdef DEBUG
				printf("STATE: FindGoal \n");
			#endif
		
			//Spin until either light sensor hits threshold
			while ( analog(lLight) > lightThreshHigh && analog(rLight) > lightThreshHigh ) {
				//look for the brightest light
				move(pointLeft, maxVelocity);
			}	
					
			//Read Ground Value
			//If we are on our Home Side
			if ( digital(groundSensor) == homeColor) {
				
				//Drive towards Light
				move(forward, maxVelocity);
				sleep(1);
				
				
				//Recheck our ground value to make sure we're moving towards correct goal
				if ( digital(groundSensor) == homeColor ) {
					//Advance to our next state
					state = DRIVEGOAL;
				}
				//We're heading towards the wrong goal
				else {
					#ifdef DEBUG
					printf("We found and moved towards light, but on wrong side \n");
					#endif
					//Spin 180
					move(oneEighty, maxVelocity);
					
					move(forward, maxVelocity);
					sleep(3);
				}
				
				
			}
			//We're in Oponent's Zone
			else {
				
				//Rotate away from Light
				move(oneEighty, maxVelocity);
				
				//Drive away from Light
				move(forward, maxVelocity);
				sleep(5);
				
				//Recheck our ground value to make sure we're moving away from opponents goal
				if ( digital(groundSensor) == homeColor ) {
					//Advance to our next state
					state = DRIVEGOAL;
				}
				
				
			}
		
		
			//Drive
			
			//Goal + Side Detection
			
			//Obstacle Avoidance - Walls/Robots
			
			
			
			break;
			
		//***** Drive towards Goal State ******
		//*************************************
		case DRIVEGOAL:
			
			#ifdef DEBUG
				printf("STATE: DRIVEGOAL \n");
			#endif
			
			//Good, we're still driving towards our Goal
			if ( digital(groundSensor) == homeColor) {
				
				int leftValue = analog(lLight);
				int rightValue = analog(rLight);	
				
				//Left Sensor Brighter
				if ( leftValue < rightValue - 20 ) {
					move(pointLeft, maxVelocity);
				}
				//Right Sensor Brighter
				else if ( rightValue < leftValue - 20) {
					move(pointRight, maxVelocity);
				}
				//Move Forwards
				else {
					if (leftValue < lightThreshLow || rightValue < lightThreshLow ) {
						ao();
						state = DUMP;
					}
					else {
						move(forward, maxVelocity);
					}
				}
				
			}
			//We need to turn around, heading towards wrong light source
			else {
				#ifdef DEBUG
					printf("Wrong Side, Turn around \n");
				#endif
				//TODO: Calibrate is this turn is a 180
				move(oneEighty, maxVelocity);
				move(forward, maxVelocity);
				sleep(3);
				state = FINDGOAL;
			}
			
			
			break;
		
			
		//**** Dumping State *****
		//************************
		case DUMP:
			
		#ifdef DEBUG
			printf("STATE: Dump\n");
		#endif
		
		ao();
		
		//If track isn't in back position, move it
		if(!digital(trackLimit)) {
				mav (trackMotor, -800);
				while (!digital(trackLimit)) {};
				mav (trackMotor, 0);
				clear_motor_position_counter(trackMotor);	
		}
		
		//Open Front Gate
		set_servo_position(fGate, GateOpen);
		
		//Push out Twice
		for (dmpCnt = 0; dmpCnt<2; dmpCnt++) {
			//Lets Dump
			move_to_position (trackMotor, 900, 2350);
			sleep (2.5);
			mav (trackMotor, -900);
			while (!digital(trackLimit)) {};
			mav (trackMotor, 0);
			clear_motor_position_counter(trackMotor);
		}
		
		//Sweep Front Gate
		set_servo_position(fGate, GateRightSweep);
		sleep(.5);
		set_servo_position(fGate, GateLeftSweep);
		sleep(.5);
		set_servo_position(fGate, GateClose);
		
		//Dump finished, move back and turn
		move(back, maxVelocity);
		sleep(1);
		move(oneEighty, maxVelocity);
		
		
		state = COLLECT;
		break;

		}	
	}
}
