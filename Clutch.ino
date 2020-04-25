// Dual Clutch Controller 
// Arduino Leonardo or Arduino Micro.
//
// Wataru Hanada
// 2020-02-02 - Original Version
// 2020-04-26 - Add Bite Point Mode
//------------------------------------------------------------

#include "Joystick.h"
#include "State.h"

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

const bool isDebug = false;
const long a1_min = 280;
const long a1_max = 222;
const long a2_min = 265;
const long a2_max = 200;

StateContext state_context_;
InputData in_;
OutData out_;

void setup() {
  if(isDebug) {
    Serial.begin(9600);
  }
  Joystick.setBrakeRange(a1_min, a1_max); // Left Paddle A1
  Joystick.setThrottleRange(a2_min, a2_max); // Right Paddle A2
  Joystick.begin();

  in_.Init(a1_min, a1_max, a2_min, a2_max, false);

  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(13, OUTPUT);
}

void loop() {

  // Turn indicator light on.
  // digitalWrite(13, 1);

  auto a1 = analogRead(A1);
  auto a2 = analogRead(A2);

  if(isDebug) {
    Serial.println(a1);
    Serial.println(a2);
  }

  in_.Update(a1, a2);
  state_context_.Update(&in_, &out_);

  Joystick.setBrake(out_.A1);

  if(isDebug) {
    Serial.println(out_.A1);
  }
}
