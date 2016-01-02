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
const int AIA = 5;  // (pwm) pin 9 connected to pin A-IA
const int AIB = 9;  // (pwm) pin 5 connected to pin A-IB
const int BIA = 6; // (pwm) pin 10 connected to pin B-IA
const int BIB = 10;  // (pwm) pin 6 connected to pin B-IB
byte speed = 70;  // change this (0-255) to control the speed of the motors

Servo myservo;  // create servo object to control a servo 
                // twelve servo objects can be created on most boards#
 
int pos = 0;    // variable to store the servo position 

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

// Listen on default port 5555, the webserver on the Yun
// will forward there all the HTTP requests for us.
YunServer server;
void setup(){
   Serial.begin(9600);
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
    

  // Bridge startup
  for (int i=0; i<=13;i++){
  pinMode(i,OUTPUT);
  }
  
  Bridge.begin();
  

  // Listen for incoming connection only from localhost
  // (no one from the external network could connect)
  server.listenOnLocalhost();
  server.begin();
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
 //  Serial.println("wpisz od 0 do 180");
}
void loop(){
  YunClient client = server.accept();
/*
  // There is a new client?
  if (client) {
    // Process request
    process(client);

    // Close connection and free resources.
    client.stop();
  }
   */
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
        if(ps2x.ButtonPressed(PSB_RED)) {            //will be TRUE if button was JUST pressed
          analogWrite(AIA, 0);
    analogWrite(BIA, 0);
    analogWrite(AIB, 0);
    analogWrite(BIB, 0);
        Serial.println("Circle just pressed");}
        if(ps2x.ButtonReleased(PSB_PINK))             //will be TRUE if button was JUST released
        Serial.println("Square just released");
        if(ps2x.NewButtonState(PSB_BLUE))            //will be TRUE if button was JUST pressed OR released
        Serial.println("X just changed");
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
         // Serial.print(" Lewy + " );
         // Serial.print(ps2x.Analog(PSS_LY), DEC);
            analogWrite(BIA, speed);
            analogWrite(BIB, 0);
            }else if (L3 >=240) {
            //  Serial.print(" Lewy - " );
            analogWrite(BIA, 0);
            analogWrite(BIB, speed);
            } else{
            //  Serial.print(" Lewy 0 " );
            analogWrite(BIA, 0);
            analogWrite(BIB, 0);
        }
        if(R3<=10){
          //  Serial.print(" Prawy + " );
            analogWrite(AIA, speed);
            analogWrite(AIB, 0);
            }else if (R3 >=240) {
           //    Serial.print(" Prawy - " );
            analogWrite(AIA, 0);
            analogWrite(AIB, speed);
            } else{
           //    Serial.print(" Prawy 0 " );
            analogWrite(AIA, 0);
            analogWrite(AIB, 0);
        }
        // Serial.println(" +++++ " );
        delay(50);
    }
    delay(50);
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



