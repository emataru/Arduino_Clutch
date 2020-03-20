// Clutch and Handbreake Controller 
// Arduino Leonardo or Arduino Micro.
//
// Wataru Hanada
// 2020-03-20 - Original Version
//------------------------------------------------------------

#include "Joystick.h"

Joystick_ Joystick = Joystick_(
  0x03,                    // reportid
  JOYSTICK_TYPE_GAMEPAD,   // type
  0,                       // button count
  0,                       // hat switch count
  false,                   // x axis enable
  false,                   // y axis enable
  false,                   // z axis enable
  false,                   // right x axis enable
  false,                   // right y axis enable
  false,                   // right z axis enable
  false,                   // rudder enable
  true,                    // throttle enable
  false,                   // accelerator enable
  true,                    // brake enable
  false                    // steering enable
);

const unsigned long gcAnalogDelta = 25;
unsigned long gNextTime = 0;
const bool isDebug = false;

void setup() {
  if(isDebug) {
    Serial.begin(9600);
  }
  Joystick.setBrakeRange(266, 207); // Clutch A1
  Joystick.setThrottleRange(283, 239); // Parking Breake A2
  Joystick.begin();

  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(13, OUTPUT);
}

void loop() {

  // Turn indicator light on.
//  digitalWrite(13, 1);

  auto a1 = analogRead(A1);
  auto a2 = analogRead(A2);

  if(isDebug) {
    Serial.println(a1);
    Serial.println(a2);
  }
  Joystick.setBrake(a1);
  Joystick.setThrottle(a2);
}
