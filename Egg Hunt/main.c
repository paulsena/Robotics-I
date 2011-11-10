/*********************************
* Paul Senatillaka
* Ramon Ramirez
* Egg Hunt

**State machine**
States: Collect -> FindGoal -> Dump
                  
Note: Camera Pixel Dimensions: 159 x 119

Todo:
-Fix finding home from opponents side
-Better search when no eggs seen. Currently just spins last seen.
-Bonus priority tracking
-Speedup dumping by backing up while resetting pusher

-Readjust 180 turn. Actually don't use discrete turns/positioning.

Changelog:
x-Add code for front closer
x-Add auto side calibration
x-Filters on Blob Size now instead of confidence. Should run faster
x-Stay away from light when not going home
x-When spinning, read side dist sensor to decide which way to turn
x-Bump Eggs in when Dumping
x-Fixed side avoidance

**********************************/
#define DEBUG

#include "helper.h";

//**** Variables ****
const int numBallsCollect = 3;
//*******************


int main()
{
	
	//Init Variables
	enum STATE state = COLLECT;
	int homeColor; 
	int currentEggColor;
	enum COLOR ourEggColor;
	int turnDirection;

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
	
	//Calibration
	printf("Place Robot down on side and Press A! \n");
	while(!a_button()) { }
	
	if (digital(groundSensor) == 0) {
		ourEggColor = PURPLE;
		homeColor = 0;  //Black
		printf("You're on the Black Side!\n");
	}
	else {
		ourEggColor = YELLOW;
		homeColor = 1; //White
		printf("You're on the White Side!\n");
	}
	
	//Set our initial egg color to go after
	currentEggColor = ourEggColor;
	
	printf("Press A to start Robot /n");
	
	//Wait to start program
	while(!a_button()) { }
	
	//AutoKill in 90 secs
	shut_down_in(90);
	
	
	while(!black_button()) {
			
		
		start_process(sideNBumperCheckMove);
		
		
		switch(state) {
			
		//**** Collection State *****
		//***************************
		case COLLECT:
			
			//Image Capture
			track_update();
		
			/*Check for Bonus Eggs
			if ( track_count(BONUS) > 0 ) {
				currentEggColor = BONUS;
			}
			else {
				currentEggColor = ourEggColor;
			}	*/	
			
			//Egg Detect
			if(track_count(currentEggColor) > 0 && track_size(currentEggColor, 0) > 10 ) {
				
				#ifdef DEBUG
					printf("See our egg %d \n", track_x( currentEggColor, 0));
				#endif
				
				//Find out where blobs are
				//Right
				if( track_x(currentEggColor, 0) > 95) {
					move(diagRight, 800);
					lastSeen = diagRight;
				}
				//Left
				else if( track_x(currentEggColor, 0) < 65 ) {
					move(diagLeft, 800);
					lastSeen = diagLeft;
				}
				//Forward
				else {
					move(forward, 800);
										
				}
				
				//Check if ball is about to go in. 
				if( track_bbox_bottom(currentEggColor, 0) > 110) {
					
					//Open Front Gate
					set_servo_position(fGate, GateOpen);
						
					sleep(.5);
					ballCount++;
					beep();
					
					sleep(.75);
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
			}
			
			//Avoid End Goals
			if (analog(lLight) < 9) {
				#ifdef DEBUG
					printf("Avoiding goal");
				#endif
				move(oneEighty, maxVelocity);
			} 
			else if (analog(rLight) < 9) {
				#ifdef DEBUG
					printf("Avoiding goal");
				#endif
				move(oneEighty, maxVelocity);
			}
			
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
		
			//Find open side before spinning
			if ( analog10(lDistSensor) > analog10(rDistSensor) ) {
				turnDirection = pointLeft;
			}
			else {
				turnDirection = pointRight;
			}
		
			//Spin until either light sensor hits threshold
			while ( analog(lLight) > lightThreshHigh && analog(rLight) > lightThreshHigh ) {
				//look for the brightest light
				move(turnDirection, maxVelocity);
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
				if ( leftValue < rightValue - 15 ) {
					move(pointLeft, maxVelocity);
				}
				//Right Sensor Brighter
				else if ( rightValue < leftValue - 15) {
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

		//Lets Dump
		move_to_position (trackMotor, 900, 2300);
		sleep (2.5);
		mav (trackMotor, -900);
		
		while (!digital(trackLimit)) {};
		mav (trackMotor, 0);
		clear_motor_position_counter(trackMotor);
		
		//Bump Eggs with Front Gate
		move(back, maxVelocity);
		sleep(.5);
		set_servo_position(fGate, GateClose);
		sleep(.2);
		move(forward, maxVelocity);
		sleep(.8);
		
		//Dump finished, move back and turn
		move(back, maxVelocity);
		sleep(.5);
		move(oneEighty, maxVelocity);
		
		
		state = COLLECT;
		break;

		}	
	}
}
