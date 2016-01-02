#include <PS2X_lib.h>  //for v1.6
#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>
#include <Servo.h>  
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
#define HG7881_B_IA 6 // D10 --> Motor B Input A --> MOTOR B +
#define HG7881_B_IB 10 // D11 --> Motor B Input B --> MOTOR B -


 #define HG7881_A_IA 5 // D10 --> Motor B Input A --> MOTOR B +
#define HG7881_A_IB 9 // D11 --> Motor B Input B --> MOTOR B -
 
// functional connections
#define MOTOR_B_PWM HG7881_B_IA // Motor B PWM Speed
#define MOTOR_B_DIR HG7881_B_IB // Motor B Direction

#define MOTOR_A_PWM HG7881_A_IA // Motor B PWM Speed
#define MOTOR_A_DIR HG7881_A_IB // Motor B Direction
//const int AIA = 5;  // (pwm) pin 9 connected to pin A-IA
//const int AIB = 9;  // (pwm) pin 5 connected to pin A-IB
//const int BIA = 6; // (pwm) pin 10 connected to pin B-IA
//const int BIB = 10;  // (pwm) pin 6 connected to pin B-IB
//byte speed = 70;  // change this (0-255) to control the speed of the motors
#define PWM_SLOW 50  // arbitrary slow speed PWM duty cycle
#define PWM_FAST 100 // arbitrary fast speed PWM duty cycle
#define DIR_DELAY 1000 // brief delay for abrupt motor changes


Servo myservo;  
int pos = 0; 

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

// Listen on default port 5555, the webserver on the Yun
// will forward there all the HTTP requests for us.
YunServer server;

void setup(){
    //Silniki

pinMode( MOTOR_A_DIR, OUTPUT );
  pinMode( MOTOR_A_PWM, OUTPUT );
  digitalWrite( MOTOR_A_DIR, LOW );
  digitalWrite( MOTOR_A_PWM, LOW );

    
   pinMode( MOTOR_B_DIR, OUTPUT );
  pinMode( MOTOR_B_PWM, OUTPUT );
  digitalWrite( MOTOR_B_DIR, LOW );
  digitalWrite( MOTOR_B_PWM, LOW );
    // PS2
    
    //CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************
    error = ps2x.config_gamepad(13,11,3,12, true, true);   //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
    if(error == 0){
       
    }
   
    
    //Serial.print(ps2x.Analog(1), HEX);
    type = ps2x.readType();
    switch(type) {
        case 0:
        
        break;
        case 1:
        
        break;
        case 2:
        
        break;
    }




     Bridge.begin();
  

  // Listen for incoming connection only from localhost
  // (no one from the external network could connect)
  server.listenOnLocalhost();
  server.begin();

  myservo.attach(0);  // attaches the servo on pin 9 to the servo object
}
void loop(){



YunClient client = server.accept();


 if (client) {
    // Process request
    process(client);

    // Close connection and free resources.
    client.stop();
  }






  
   
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
       
        vibrate = ps2x.Analog(PSAB_BLUE);        //this will set the large motor vibrate speed based on
        //how hard you press the blue (X) button
       
        if(ps2x.ButtonPressed(PSB_RED)) {            //will be TRUE if button was JUST pressed
        digitalWrite( MOTOR_A_DIR, LOW );
  digitalWrite( MOTOR_A_PWM, LOW );
  
  digitalWrite( MOTOR_B_DIR, LOW );
  digitalWrite( MOTOR_B_PWM, LOW );
        }
       
      /*  if(ps2x.Button(PSB_L3) || ps2x.Button(PSB_R3)) // print stick values if either is TRUE
        {
            Serial.print("Stick Values:");
            Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX
            Serial.print(",");
            Serial.print(ps2x.Analog(PSS_LX), DEC);
            Serial.print(",");
            Serial.print(ps2x.Analog(PSS_RY), DEC);
            Serial.print(",");
            Serial.println(ps2x.Analog(PSS_RX), DEC);
        }*/
        L3=ps2x.Analog(PSS_LY);
        R3= ps2x.Analog(PSS_RY);
        /*
        Serial.print("drozki ");
        Serial.print(L3);
        Serial.print(" , ");
        Serial.println(R3);*/
         
        if(L3<=10){
          
         // Serial.print(ps2x.Analog(PSS_LY), DEC);
           digitalWrite( MOTOR_B_DIR, LOW );
        digitalWrite( MOTOR_B_PWM, LOW );
        delay( DIR_DELAY );
            digitalWrite( MOTOR_B_DIR, LOW ); // direction = reverse
            analogWrite( MOTOR_B_PWM, PWM_SLOW ); // PWM speed = slow
           
            
            }else if (L3 >=240) {
              digitalWrite( MOTOR_B_DIR, LOW );
        digitalWrite( MOTOR_B_PWM, LOW );
        delay( DIR_DELAY );
              
           digitalWrite( MOTOR_B_DIR, HIGH ); // direction = forward
        analogWrite( MOTOR_B_PWM, 255-PWM_SLOW ); // PWM speed = slow
            } else{
              
            digitalWrite( MOTOR_B_DIR, LOW );
        digitalWrite( MOTOR_B_PWM, LOW );
        }
        if(R3<=10){
          digitalWrite( MOTOR_A_DIR, LOW );
        digitalWrite( MOTOR_A_PWM, LOW );
        delay( DIR_DELAY );
            
             digitalWrite( MOTOR_A_DIR, LOW ); // direction = reverse
             analogWrite( MOTOR_A_PWM, PWM_SLOW ); // PWM speed = slow
            }else if (R3 >=240) {
              digitalWrite( MOTOR_A_DIR, LOW );
        digitalWrite( MOTOR_A_PWM, LOW );
        delay( DIR_DELAY );
               
               digitalWrite( MOTOR_A_DIR, HIGH ); // direction = forward
                analogWrite( MOTOR_A_PWM, 255-PWM_SLOW ); // PWM speed = slow
          
            } else{
               
               digitalWrite( MOTOR_A_DIR, LOW );
              digitalWrite( MOTOR_A_PWM, LOW );
           
        }
       
        
    }
   
}








void process(YunClient client) {
  // read the command
  String command = client.readStringUntil('/');

  // is "digital" command?
  if (command == "digital") {
    digitalCommand(client);
  }

 // is "analog" command?
  if (command == "analog") {
    analogCommand(client);
  }

  // is "mode" command?
  if (command == "mode") {
    modeCommand(client);
  }
   if (command == "servo") {
    servoCommand(client);
  }
   if (command == "help") {
    helpCommand(client);
  }
   if (command == "dc") {
    silnikCommand(client);
  }
}

void digitalCommand(YunClient client) {
  int pin, value;
 client.println(F("Witaj w swiatelkach"));
  // Read pin number
  pin = client.parseInt();

  // If the next character is a '/' it means we have an URL
  // with a value like: "/digital/13/1"
  if (client.read() == '/') {
    value = client.parseInt();
    if (value = 0)
    digitalWrite(pin, LOW);
    if(value = 1)
     digitalWrite(pin, HIGH);
  } 
  else {
    value = digitalRead(pin);
  }

  // Send feedback to client
  client.print(F("Pin D"));
  client.print(pin);
  client.print(F(" ustawiony na "));
  client.println(value);

  // Update datastore key with the current pin value
  String key = "D";
  key += pin;
  Bridge.put(key, String(value));
}

void analogCommand(YunClient client) {
  int pin, value;

  // Read pin number
  pin = client.parseInt();

  // If the next character is a '/' it means we have an URL
  // with a value like: "/analog/5/120"
  if (client.read() == '/') {
    // Read value and execute command
    value = client.parseInt();
    analogWrite(pin, value);

    // Send feedback to client
    client.print(F("Pin D"));
    client.print(pin);
    client.print(F(" set to analog "));
    client.println(value);

    // Update datastore key with the current pin value
    String key = "D";
    key += pin;
    Bridge.put(key, String(value));
  }
  else {
    // Read analog pin
    value = analogRead(pin);

    // Send feedback to client
    client.print(F("Pin A"));
    client.print(pin);
    client.print(F(" reads analog "));
    client.println(value);

    // Update datastore key with the current pin value
    String key = "A";
    key += pin;
    Bridge.put(key, String(value));
  }
}

void modeCommand(YunClient client) {
  int pin;

  // Read pin number
  pin = client.parseInt();

  // If the next character is not a '/' we have a malformed URL
  if (client.read() != '/') {
    client.println(F("error"));
    return;
  }

  String mode = client.readStringUntil('\r');

  if (mode == "input") {
    pinMode(pin, INPUT);
    // Send feedback to client
    client.print(F("Pin D"));
    client.print(pin);
    client.print(F(" configured as INPUT!"));
    return;
  }

  if (mode == "output") {
    pinMode(pin, OUTPUT);
    // Send feedback to client
    client.print(F("Pin D"));
    client.print(pin);
    client.print(F(" configured as OUTPUT!"));
    return;
  }

  client.print(F("error: invalid mode "));
  client.print(mode);
}


void servoCommand(YunClient client) {
  int pozycja;

  // Read pin number
  pozycja = client.parseInt();
myservo.write(pozycja);              // tell servo to go to position in variable 'pos' 
    delay(15);
  // If the next character is a '/' it means we have an URL
  // with a value like: "/digital/13/1"
 /* if (client.read() == '/') {
    value = client.parseInt();
    myservo.write(value);              // tell servo to go to position in variable 'pos' 
    delay(15);
  } 
  else {
    value = digitalRead(pin);
  }
*/
  // Send feedback to client
  client.println(F("Witaj w obsludze serwo mechanizmu !!! "));
  client.print(F("Serwo ustawione na "));
  client.print(pozycja);
 client.print(F(" stopnia"));
/*
  // Update datastore key with the current pin value
  String key = "D";
  key += pin;
  Bridge.put(key, String(value));*/
}







void helpCommand(YunClient client) {
  int pin, value;

  // Read pin number
  pin = client.parseInt();
  client.println(F("Kochaj moje placki a Tyburek to dostanie na środe"));
  client.println(F("Witaj w intenetowym sterowani arduino"));
        client.println(F(""));
     client.println(F("aby ustawiac swiatelka wipsz:"));
     client.println(F("http://adresDoArduino/arduino/digital/13/0"));
      client.println(F("/digital/13/0 wytlumaczenie digital to pord cyfrowy         13 to nr portu ktory chesz nastawić               a 0 to stan portu zamias niego mozesz wpisac 1 albo 0"));

      
  client.println(F(""));
       client.println(F("aby ustawiac kamerke na serwie wipsz:"));
     client.println(F("http://adresDoArduino/arduino/servo/90"));
      client.println(F("  http://adresDoArduino/arduino/servo/90  servo to czym sterujesz                a 90 to kąt jaki cie intresuje możesz wpisać od 0 do 180"));
}




void silnikCommand(YunClient client) {
  
 
}


