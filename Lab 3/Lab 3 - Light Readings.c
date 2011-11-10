/***************************************************************************
Paul Senatillaka
Ramon Ramirez
Lab 3
Light Sensor Reading
***************************************************************************/

int main()
{

	while(1) {
		set_each_analog_state(0,0,0,0,0,0,0,0);
		printf("Left sensor %d\n", analog(7));
		sleep(.5);
	}
	
	
	
} 

/*Function definitions go below.*/
