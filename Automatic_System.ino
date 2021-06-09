   #include <SoftwareSerial.h>
SoftwareSerial GPSModule(3,2);

int sensor_pin = A0; 
int output_value ;
String MySendString;
int pumpPin = 4;

void setup() {
  pinMode(pumpPin,OUTPUT);
   digitalWrite(pumpPin,0);
  Serial.begin(9600);
  delay(2000); 
  Serial.println("Reading From the Sensor ...");
  GPSModule.begin(9600);
  delay(5000);
  Serial.println("Initializing HTTP... ");
  //delay(10000);
  httpInit();
}

void loop()
{
  output_value= analogRead(sensor_pin);
  output_value = map(output_value,1020,0,0,100);
  Serial.print("Mositure : ");
  Serial.print(output_value);
  Serial.println("%");
  delay(3000);

  if(output_value > 50)
  {
    //Turn off pump
    digitalWrite(pumpPin,0);
    Serial.println("Pump off");
  }
  else
  {
    //Turn on pump
    digitalWrite(pumpPin,1);
    Serial.println("Pump On");
  }
  int pumpStatus = digitalRead(pumpPin);
  Serial.println(pumpStatus);
  MySendString = String(output_value) + "," + String(pumpStatus);
  sendData(MySendString);
}

void httpInit()
{
   /********************/
  // Initialize GSM to enter GPRS mode for internet conection
  GPSModule.println("AT+HTTPINIT");
  GPSModule.print("\n");
  updateSerial();
  delay(1000);

  GPSModule.println("AT+CGATT=1");
  delay(2000);
  updateSerial();

  GPSModule.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  delay(2000);
  updateSerial();

  // Remember to change to the network provider you are using
  GPSModule.println("AT+SAPBR=3,1,\"APN\",\"SAFARICOM\"");
  delay(2000);
  updateSerial();

  GPSModule.println("AT+SAPBR=1,1");
  delay(2000);
  updateSerial();
 
  GPSModule.println("AT+SAPBR=2,1");
  delay(2000);
  updateSerial();
  /********************/  
}

void sendData(String data)
{
  // Append the sensor value to the server URL
  GPSModule.print("AT+HTTPPARA=\"URL\",\"http://heriplex.com/irrig/getval.php?g=");
  GPSModule.print(data);
  GPSModule.println("\"");
  delay(1000);
  updateSerial();
  // Now POST the data to the cloud
  GPSModule.println("AT+HTTPACTION=1");
  delay(2000);
  //Serial1.println("AT+CGATT=0");
  //delay(3000);
}

void updateSerial()
{
  while(GPSModule.available()) 
  {
    Serial.write(GPSModule.read());//Forward what Software Serial received to Serial Port
  }
}
