lo#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include <OneWire.h>
#include <WiFi.h>
#include <SPI.h>
#include <PubSubClient.h>
#include <string.h>
// tracking sensor and buzzer pin declaration
const int Sensor = 17;
const int buzzer = 25 ;
const int buttonPin = 26;
int buttonState = 0;
int hum;
int temp;
// WiFi network name and password: please modify according to
//your own wifi connection specifics
const char * networkName = "kenesolor";
const char * networkPswd = "janinajanina";
PubSubClient client;
WiFiClient wf;
//function for connection to network
void connectToWiFi(const char * ssid, const char * pwd)
{
  Serial.println("Connecting to WiFi network: " + String(ssid));
  WiFi.begin(ssid, pwd);
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect()
 {
 // Loop until we're reconnected
 while (!client.connected()) {
 Serial.print("Attempting MQTT connection...");
 // Attempt to connect
 client.setClient(wf);
  client.setServer("10.0.0.19",1883);
 if (client.connect("esp32dev"))
  {
  Serial.println("connected");
  // ... and subscribe to topic
  client.subscribe("Say");
 }
 else
  {
  Serial.print("failed, rc=");
  Serial.print(client.state());
  Serial.println(" try again in 1 seconds");
  // Wait 1 seconds before retrying
  delay(1000);
  }
 }
}
//end of function of network

//* code of temperature and humidity sensor until *//
int DHpin = 16;
byte dat [5];
byte read_data () {
  byte data;
  for (int i = 0; i < 8; i ++) {
    if (digitalRead (DHpin) == LOW) {
      while (digitalRead (DHpin) == LOW); // wait for 50us
      delayMicroseconds (30); // determine the duration of the high level to determine the data is '0 'or '1'
      if (digitalRead (DHpin) == HIGH)
        data |= (1 << (7-i)); // high front and low in the post
      while (digitalRead (DHpin) == HIGH); // data '1 ', wait for the next one receiver
     }
  }
return data;
}

void start_test () {
  digitalWrite (DHpin, LOW); // bus down, send start signal
  delay (30); // delay greater than 18ms, so DHT11 start signal can be detected

  digitalWrite (DHpin, HIGH);
  delayMicroseconds (40); // Wait for DHT11 response

  pinMode (DHpin, INPUT);
  while (digitalRead (DHpin) == HIGH);
  delayMicroseconds (80); // DHT11 response, pulled the bus 80us
  if (digitalRead (DHpin) == LOW);
  delayMicroseconds (80); // DHT11 80us after the bus pulled to start sending data

  for (int i = 0; i < 4; i ++) // receive temperature and humidity data, the parity bit is not considered
    dat[i] = read_data ();

  pinMode (DHpin, OUTPUT);
  digitalWrite (DHpin, HIGH); // send data once after releasing the bus, wait for the host to open the next Start signal
} //* temperature and humidity sensor code ends





void setup ()
{
  Serial.begin(9600);
  pinMode (Sensor, INPUT) ; // Initializing Sensorpin
  pinMode (buzzer, OUTPUT) ;
  pinMode (DHpin, OUTPUT);
  pinMode(buttonPin, INPUT);
	connectToWiFi(networkName, networkPswd);
  client.setClient(wf);
  //please specify your own broker specific IP
  client.setServer("10.0.0.19",1883);
}


void loop ()
{
  unsigned char i, j ;
  bool val = digitalRead (Sensor) ; //trckr

  if (val == HIGH)
  {
    Serial.println("LineTracker is outside the line "); //trckr

  for (i = 0; i <80; i++) //  a frequency sound
  {
    digitalWrite (buzzer, HIGH) ;// sound
    delay (1) ;// Delay 1ms
    digitalWrite (buzzer, LOW) ;// no sound
    delay (1) ;// delay ms
  }
  for (i = 0; i <100; i++) //  another frequency sound
  {
    digitalWrite (buzzer, HIGH) ;//  sound
    delay (2) ;// delay 2ms
    digitalWrite (buzzer, LOW) ;// no sound
    delay (2) ;// delay 2ms
  }
//
// read the state of the pushbutton value:
buttonState = digitalRead(buttonPin);

// check if the pushbutton is pressed.
// if it is, the buttonState is HIGH:
if (buttonState == HIGH)
{
//do nothing
}
else
{
  //code for t & h sensor
  start_test ();
Serial.print ("Current humdity =");
hum = dat[0];
Serial.print (dat [0], DEC); // display the humidity-bit integer;
Serial.print ('.');
Serial.print (dat [1], DEC); // display the humidity decimal places;
//  float aInt =(float)dat [0]+((float)Fract/100) ;
Serial.println ('%');
Serial.print ("Current temperature =");
temp = dat[2];
Serial.print (dat [2], DEC); // display the temperature of integer bits;
Serial.print ('.');
Serial.print (dat [3], DEC); // display the temperature of decimal places;
Serial.println ('C');


//this fragment is for TTS(Text to Speech)
//TTS for humidity
client.publish("Say","'Current humidity is'");
//int hum = (int)dat[0];
char strn5[15];
sprintf(strn5, "%d", hum);
char strn6[15]= "' ";
strcat(strn6,strn5);
strcat(strn6," '");
Serial.println (strn6);
client.publish("Say",strn6);


client.publish("Say","'Current Temperature is'");
char strn[15];
sprintf(strn, "%d", temp);
char strn2[15]= "' ";
strcat(strn2,strn);
strcat(strn2," '");
Serial.println (strn2);
client.publish("Say",strn2);
}
//


}

  else
  {
    Serial.println("Linetracker is in the line ");

//
// read the state of the pushbutton value:
buttonState = digitalRead(buttonPin);

// check if the pushbutton is pressed.
// if it is, the buttonState is HIGH:
if (buttonState == HIGH)
{
//do nothing
}
else
{
  //code for t & h sensor
  start_test ();
Serial.print ("Current humdity =");
 hum = dat[0];
Serial.print (dat [0], DEC); // display the humidity-bit integer;
Serial.print ('.');
Serial.print (dat [1], DEC); // display the humidity decimal places;
Serial.println ('%');
Serial.print ("Current temperature =");
 temp = dat[2];
Serial.print (dat [2], DEC); // display the temperature of integer bits;
Serial.print ('.');
Serial.print (dat [3], DEC); // display the temperature of decimal places;
Serial.println ('C');

//this fragment is for TTS(Text to Speech)
//TTS for humidity
client.publish("Say","'Current humidity is'");
//int hum = (int)dat[0];
char strn5[15];
sprintf(strn5, "%d", hum);
char strn6[15]= "' ";
strcat(strn6,strn5);
strcat(strn6," '");
Serial.println (strn6);
client.publish("Say",strn6);


client.publish("Say","'Current Temperature is'");
char strn[15];
sprintf(strn, "%d", temp);
char strn2[15]= "' ";
strcat(strn2,strn);
strcat(strn2," '");
Serial.println (strn2);
client.publish("Say",strn2);

}
// end of TTS over MQTT segment
  }

  Serial.println("------------------------------------");
  //for reconnection , sometimes the code must be uploaded multiple times if MQTT connection fails
  //or in case the code is already loaded, simply the esp32 on the cane may be restarted attempting
  //a MQTT connection
if (!client.connected())
{
reconnect();
}
//
  delay(500);
}
