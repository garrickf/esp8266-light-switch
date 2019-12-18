#include <Arduino.h> // Should be version 2.3.X
#include <ESP8266WiFi.h>
#include <weenyMo.h>
#include <Servo.h>

// This function will be called when Alexa hears "switch [on | off] 'vee three'" 
// ESP8266 builtin LED is "backwards" i.e. active LOW, hence Alexa's ON=1 needs reversing

#define WIFI_SSID "Experience PEACE"
#define WIFI_PASS "contemplation.stanford.edu"
#define SERIAL_BAUDRATE 74880

#define SERVO_DATA_PIN D1

Servo servo;
bool operated = true;
bool isOn = false;

void servoPressOn() {
  servo.write(90 + 55); // 25-30 When glued down.
  delay(200);
  servo.write(90);
}

void servoPressOff() {
  servo.write(90 - 55);
  delay(200);
  servo.write(90);
}

void onVoiceCommand(bool turnOn){ 
  Serial.printf("onVoiceCommand %d\n", turnOn);
  digitalWrite(BUILTIN_LED, LOW);
  isOn = turnOn;
  operated = false;
  digitalWrite(BUILTIN_LED, HIGH);
} 

// Forward declaration from weenyMo.h
bool getAlexaState(){
  Serial.printf("getAlexaState %d\n", isOn);
  return isOn;
}

// The only constructor: const char* name, function<void(bool)> onCommand
// choose the name wisely: Alexa has very selective hearing!
//
weenyMo device("Kitchen", onVoiceCommand);
           
void setup() {
  // Servo setup
  servo.attach(SERVO_DATA_PIN);
  servo.write(90);
  
  Serial.begin(SERIAL_BAUDRATE);
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);
  
  // Wifi setup
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  WiFi.waitForConnectResult();
  
  device.gotIPAddress();
}

void loop() {
  // Use getAlexaState to decide whether or not to write to the servo in the event loop
  // (outside of the async server)
  if (operated) return;;
  if (isOn) {
    servoPressOn();
  } else {
    servoPressOff();
  }
  operated = true;
  delay(1000);
}
