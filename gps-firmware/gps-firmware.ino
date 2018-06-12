/* Comment this out to disable prints and save space */
// Libraries for Blynk and ESP8266
#define BLYNK_PRINT Serial
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <SimpleTimer.h>
#include <SoftwareSerial.h>
#define ESP8266_BAUD 9600
#define GPSECHO  false // Set to 'true' if you want to debug and listen to the raw GPS sentences. 



// Variables for Blynk, ESP8266, Blynk Connection and Hearthbeat
int heartbeat ;
String readString;
bool Connected2Blynk = false;


char auth[] = "a9366cf2c7ca444f965db8bd9ac50a76";
char ssid[] = "tardis";
char pass[] = "Blake123";






SoftwareSerial EspSerial(2, 3); // RX, TX //ESP8266

ESP8266 wifi(&EspSerial);


SimpleTimer timerx;


void setup()
{
  Serial.println("CYTRON_TOUCH_PA1_UNO");
  Serial.begin(9600);
  EspSerial.begin(ESP8266_BAUD);


  delay(10);
  delay(1000);


  pinMode(4, INPUT);      // sets the digital pin 4 as input
  Blynk.begin(auth, wifi, ssid, pass);

  timerx.setInterval(5000L, CheckConnection);
  timerx.setInterval(1000L, Heartbeat);
//  timerx.setInterval(5000L, GPSFunction);
}












void CheckConnection() {
  Connected2Blynk = Blynk.connected();
  if (!Connected2Blynk) {
    Serial.println("Not connected to Blynk server");
  }
  else {
    Serial.println("Connected to Blynk server");

  }
}


void Heartbeat()
{
  // randNumber1 = random(100);
  float rev = 2;
 heartbeat =  heartbeat +1;
  Blynk.virtualWrite(V1, heartbeat);
  Serial.print("Sending Heart Beath ->");
  Serial.println(heartbeat);
 
}




void loop()
{
  Blynk.run();
  timerx.run();
 

}


