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

void servoPressOn() {
  servo.write(90 + 25);
//  delay(200);
//  servo.write(90);
}

void servoPressOff() {
  servo.write(90 - 25);
//  delay(200);
//  servo.write(90);
}

void onVoiceCommand(bool turnOn){ 
  Serial.printf("onVoiceCommand %d\n", turnOn);
  digitalWrite(BUILTIN_LED, LOW);
  if (turnOn) {
    servoPressOn();
  } else {
    servoPressOff();
  }
  digitalWrite(BUILTIN_LED, HIGH);
} 

// Forward declaration from weenyMo.h
bool getAlexaState(){
  Serial.printf("getAlexaState %d\n",!digitalRead(BUILTIN_LED));
  return !digitalRead(BUILTIN_LED);
}

// The only constructor: const char* name, function<void(bool)> onCommand
// choose the name wisely: Alexa has very selective hearing!
//
weenyMo device("Kitchen", onVoiceCommand);
           
void setup() {
  Serial.begin(SERIAL_BAUDRATE);
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);
  
  // Wifi setup
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  WiFi.waitForConnectResult();
  
  device.gotIPAddress();

  // Servo setup
  servo.attach(SERVO_DATA_PIN);
  servo.write(0);
}

void loop() {
//  servoPressOn();
//  delay(1000);
//  servoPressOff();
//  delay(1000);  
}