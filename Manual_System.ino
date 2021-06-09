#include <SoftwareSerial.h>
SoftwareSerial serial(3, 2); // RX digital pin 3, TX digital pin 2.


void setup()
{
  pinMode(4,OUTPUT);
  digitalWrite(4,0);
  Serial.begin(9600);
  Serial.println("******INITIALIZING SYSTEM******");
  serial.begin(9600); //Baud rate of the GSM/GPRS Module
  serial.println("AT+CMGF=1\r");
  updateSerial();
  delay(100);
  //serial.println("AT+CMGR=1\r");//Read SMS at location 1.
  serial.println("AT+CNMI=2,2,0,0,0\r"); //To read newly arrived SMS.
  updateSerial();
  delay(2000);

 Serial.println("***SYSTEM INITIALIZATION COMPLETE***\n\n\n");
 Serial.println("******WAITING FOR COMMANDS FROM THE USER******");
  
}

void loop()
{
  if (serial.available() > 0)
  {
    int c = serial.read();
    if (c == 'R' )
    {
      //Turn on pump
      digitalWrite(4,1);
      Serial.println("Pump On");
    }
    if (c == 'F' )
    {
      //Turn off pump
      digitalWrite(4,0);
      Serial.println("Pump off");
    }

    //Serial.write(c);
  }
}

void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    serial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(serial.available()) 
  {
    Serial.write(serial.read());//Forward what Software Serial received to Serial Port
  }
}
