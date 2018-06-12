#define BLYNK_PRINT Serial
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <SimpleTimer.h>
#include <SoftwareSerial.h>
#define ESP8266_BAUD 9600

// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO false

// Blynk connection variables
int heartbeatValue = 0;
bool connectedToBlynk = false;

// Blynk connection configs
char auth[] = "a9366cf2c7ca444f965db8bd9ac50a76";
char ssid[] = "tardis";
char pass[] = "Blake123";

// ESP8266 config
SoftwareSerial EspSerial(2, 3); // RX, TX //ESP8266
ESP8266 wifi(&EspSerial);

SimpleTimer timer;

void setup() {
  Serial.begin(9600);
  Serial.println("CYTRON_TOUCH_PA1_UNO");
  
  EspSerial.begin(ESP8266_BAUD);

  // TODO: Delays for what?
  delay(10);
  delay(1000);

  // TODO: Pin 4 is what? Move to global constant
  pinMode(4, INPUT);
  Blynk.begin(auth, wifi, ssid, pass);

  timer.setInterval(5000L, checkConnection);
  timer.setInterval(1000L, heartbeat);

  // TODO: Add GPS method to timer
  // timerx.setInterval(5000L, GPSFunction);

  Serial.println("Board setup is complete");
}

void loop() {
  Blynk.run();
  timer.run();
}

void checkConnection() {
  connectedToBlynk = Blynk.connected();
  if (!connectedToBlynk) {
    Serial.println("Not connected to Blynk server");
  }
  else {
    Serial.println("Connected to Blynk server");
  }
}

void heartbeat() {
  heartbeatValue++;
  Blynk.virtualWrite(V1, heartbeatValue);
  Serial.print("Sending heartbeat: ");
  Serial.println(heartbeatValue);
}

