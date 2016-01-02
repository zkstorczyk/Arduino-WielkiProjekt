#include <OneWire.h>
#include <DallasTemperature.h>


#define ONE_WIRE_BUS 2
int photocellInput = -1;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress insideThermometer;


void setup(void)
{
 Serial.begin(9600);
  sensors.begin();
  sensors.getAddress(insideThermometer, 0);
  sensors.setResolution(insideThermometer, 9); // rozdzielczosc termometru do 9bit
}

void printTemperature(DeviceAddress deviceAddress)
{
 sensors.requestTemperatures();
  Serial.print("Temperatura ");
  
  Serial.print(sensors.getTempC(deviceAddress));
  Serial.print(" C");
   Serial.println(" ");
   photocellInput = (analogRead(0));
  Serial.print(" Oczyt foto rezystora= ");
  Serial.println(photocellInput);
}

void loop(void)
{ 
  
   
  printTemperature(insideThermometer);
  delay(1000);
}

