/*********************************
* Paul Senatillaka
* Ramon Ramirez
* Egg Hunt

**State machine**
States: Collect -> FindGoal -> Dump
**********************************/


#include "helper.h";

//Declerations
enum state = { COLLECT, FINDGOAL, DUMP };

int main()
{
	//Init Variables
	
	//Image Capture
	track_update();
	
	//Initial Vision Planning
	
	
	while(!black_button()) {
		
		//**** Collection State *****
		//Sub Loop
		
			//Drive
		
			//Egg Detect
	
			//Obstacle Avoidance - Walls/Robots
		
		//End Loop
		
		//**** Goal Finding State *****
		//Sub Loop
			//Drive
		
			//Goal + Side Detection
		
			//Obstacle Avoidance - Walls/Robots
		
		//End Loop
		
		//**** Dumping State *****
			//Activate Pulley


	}	
	
}
