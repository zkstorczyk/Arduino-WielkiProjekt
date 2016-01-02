#include <PS2X_lib.h>  //for v1.6

	//Kontroler PS2
	PS2X ps2x; // create PS2 Controller Class

	//right now, the library does NOT support hot pluggable controllers, meaning 
	//you must always either restart your Arduino after you conect the controller, 
	//or call config_gamepad(pins) again after connecting the controller.
	int error = 0; 
	byte type = 0;
	byte vibrate = 0;

	int R3=0;
	int L3=0;

	//error = ps2x.config_gamepad(13,11,3,12, true, true);   //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error



	//Silniki
	const int AIA = 5;  // (pwm) pin 9 connected to pin A-IA 
	const int AIB = 9;  // (pwm) pin 5 connected to pin A-IB 
	const int BIA = 6; // (pwm) pin 10 connected to pin B-IA  
	const int BIB = 10;  // (pwm) pin 6 connected to pin B-IB 

	byte speed = 50;  // change this (0-255) to control the speed of the motors 

	void setup(){
	//Silniki
	pinMode(AIA, OUTPUT); // set pins to output
	pinMode(AIB, OUTPUT);
	pinMode(BIA, OUTPUT);
	pinMode(BIB, OUTPUT);
	analogWrite(AIA, 0);
	analogWrite(BIA, 0);
	analogWrite(AIB, 0);
	analogWrite(BIB, 0);
	// PS2
	Serial.begin(9600);

	//CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************

	error = ps2x.config_gamepad(13,11,3,12, true, true);   //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error

	if(error == 0){
	Serial.println("Found Controller, configured successful");
	Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
	Serial.println("holding L1 or R1 will print out the analog stick values.");
	Serial.println("Go to www.billporter.info for updates and to report bugs.");
	}

	else if(error == 1)
	Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");

	else if(error == 2)
	Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

	else if(error == 3)
	Serial.println("Controller refusing to enter Pressures mode, may not support it. ");

	//Serial.print(ps2x.Analog(1), HEX);

	type = ps2x.readType(); 
	switch(type) {
	case 0:
	Serial.println("Unknown Controller type");
	break;
	case 1:
	Serial.println("DualShock Controller Found");
	break;
	case 2:
	Serial.println("GuitarHero Controller Found");
	break;
	}

	}


	void loop(){
	/* You must Read Gamepad to get new values
	Read GamePad and set vibration values
	ps2x.read_gamepad(small motor on/off, larger motor strenght from 0-255)
	if you don't enable the rumble, use ps2x.read_gamepad(); with no values

	you should call this at least once a second
	*/



	if(error == 1) //skip loop if no controller found
	return; 

	if(type == 2){ //Guitar Hero Controller


	}

	else { //DualShock Controller

	ps2x.read_gamepad(false, vibrate);          //read controller and set large motor to spin at 'vibrate' speed

	if(ps2x.Button(PSB_START))                   //will be TRUE as long as button is pressed
	Serial.println("Start is being held");
	if(ps2x.Button(PSB_SELECT))
	Serial.println("Select is being held");


	if(ps2x.Button(PSB_PAD_UP)) {         //will be TRUE as long as button is pressed
	Serial.print("Up held this hard: ");
	Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);
	}
	if(ps2x.Button(PSB_PAD_RIGHT)){
	Serial.print("Right held this hard: ");
	Serial.println(ps2x.Analog(PSAB_PAD_RIGHT), DEC);
	}
	if(ps2x.Button(PSB_PAD_LEFT)){
	Serial.print("LEFT held this hard: ");
	Serial.println(ps2x.Analog(PSAB_PAD_LEFT), DEC);
	}
	if(ps2x.Button(PSB_PAD_DOWN)){
	Serial.print("DOWN held this hard: ");
	Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);
	}   


	vibrate = ps2x.Analog(PSAB_BLUE);        //this will set the large motor vibrate speed based on 
	//how hard you press the blue (X) button    

	if (ps2x.NewButtonState())               //will be TRUE if any button changes state (on to off, or off to on)
	{




	/*


	if(ps2x.Button(PSB_L1)){
	analogWrite(BIA, speed);
	analogWrite(BIB, 0);          
	Serial.println("L1 pressed");}
	else{
	analogWrite(BIA, 0);
	analogWrite(BIB, 0); 
	}

	if(ps2x.Button(PSB_L2)){
	analogWrite(BIA, 0);
	analogWrite(BIB, speed);          
	Serial.println("L1 pressed");}
	else{
	analogWrite(BIA, 0);
	analogWrite(BIB, 0); 
	}



	if(ps2x.Button(PSB_R2)){
	analogWrite(AIA, speed);
	analogWrite(AIB, 0);          
	Serial.println("R2 pressed");}
	else{
	analogWrite(AIA, 0);
	analogWrite(AIB, 0); 
	}


	if(ps2x.Button(PSB_R1)){
	analogWrite(AIA, speed);
	analogWrite(AIB, 0);          
	Serial.println("R1 pressed");}
	else{
	analogWrite(AIA, 0);
	analogWrite(AIB, 0); 
	}*/
	}   


	if(ps2x.ButtonPressed(PSB_RED))             //will be TRUE if button was JUST pressed
	Serial.println("Circle just pressed");

	if(ps2x.ButtonReleased(PSB_PINK))             //will be TRUE if button was JUST released
	Serial.println("Square just released");     

	if(ps2x.NewButtonState(PSB_BLUE))            //will be TRUE if button was JUST pressed OR released
	Serial.println("X just changed");    


	if(ps2x.Button(PSB_L3) || ps2x.Button(PSB_R3)) // print stick values if either is TRUE
	{
	Serial.print("Stick Values:");
	Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX  
	Serial.print(",");
	Serial.print(ps2x.Analog(PSS_LX), DEC); 
	Serial.print(",");
	Serial.print(ps2x.Analog(PSS_RY), DEC); 
	Serial.print(",");
	Serial.println(ps2x.Analog(PSS_RX), DEC); 
	} 


	L3=ps2x.Analog(PSS_LY);
	R3= ps2x.Analog(PSS_RY);    
	/*
	Serial.print("drozki ");
	Serial.print(L3);
	Serial.print(" , ");
	Serial.println(R3);*/

	if(L3<=10){
			analogWrite(BIA, speed);
			analogWrite(BIB, 0);  
			}else if (L3 >=240) {
		analogWrite(BIA, 0);
		analogWrite(BIB, speed);  
		} else{
		analogWrite(BIA, 0);
		analogWrite(BIB, 0); 
		}



		if(R3<=10){
				analogWrite(AIA, speed);
				analogWrite(AIB, 0);  
				}else if (R3 >=240) {
			analogWrite(AIA, 0);
			analogWrite(AIB, speed);  

			} else{
			analogWrite(AIA, 0);
			analogWrite(AIB, 0); 
			}




			delay(50);






			}





			delay(50);
			}






			