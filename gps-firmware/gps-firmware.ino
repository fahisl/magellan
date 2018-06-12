#define BLYNK_PRINT Serial
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <SimpleTimer.h>
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#define ESP8266_BAUD 9600

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

// GPS config
SoftwareSerial gpsSerial(8, 7);
Adafruit_GPS GPS(&gpsSerial);
#define GPSECHO false  // true = debug prints
boolean usingInterrupt = true;
uint32_t gpsTimer = millis();

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

  // Start GPS
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PGCMD_ANTENNA);
  useInterrupt();

  // This is from the GPS example but doesn't indicate why
  delay(1000);

  // Start Blynk keep alive
  timer.setInterval(5000L, checkConnection);
  timer.setInterval(1000L, heartbeat);

  Serial.println("Magellan GPS firmware setup is complete");
}

void loop() {
  Blynk.run();
  timer.run();
  checkNewGPSSentence();
}

SIGNAL(TIMER0_COMPA_vect) {
  // Interrupt is called once a millisecond, looks for any new GPS data, and stores it
  
  char c = GPS.read();

#ifdef UDR0
  if (GPSECHO)
    if (c) UDR0 = c;  
#endif
}

void useInterrupt() {
  if (usingInterrupt) {
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
  } else {
    TIMSK0 &= ~_BV(OCIE0A);
  }
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

void resetGPSTimer() {
  gpsTimer = millis();
}

void checkNewGPSSentence() {
  // Try to parse new sentence. Skip normally on parsing failure.
  if (GPS.newNMEAreceived()) {

    // Skip the rest if the sentence cannot be parsed
    if (!GPS.parse(GPS.lastNMEA())) return;
  }

  // If we are at the desired interval, update our GPS data
  if (gpsTimer > millis()) {
    resetGPSTimer();
  }

  // TODO: move 1000ms to a config constant
  if (millis() - gpsTimer > 1000) {
    resetGPSTimer();
    writeGPSDataToBlynk();
  }
}

void writeGPSDataToBlynk() {
  // Skip the rest if no Blynk connection
  if (!Blynk.connected()) return;

  // TODO: fix virtual pins and add the data we need
  Blynk.virtualWrite(V5, (int)GPS.fix);
  
  // Skip the rest if no fix
  if (!GPS.fix) return;
  
  // Write out each data value to Blynk
  Blynk.virtualWrite(V2, GPS.latitudeDegrees);
  Blynk.virtualWrite(V3, GPS.longitudeDegrees);
  Blynk.virtualWrite(V4, GPS.speed);
}

