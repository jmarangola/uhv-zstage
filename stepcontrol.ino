#include <Stepper.h>
/** @ John Marangola
 *  Controls TB6560 stepper driver on arduino uno or equivalent microprocessor.
 *  Tailored for Nema 23 stepper motor
 *  Just a basic module to control the function of a stepper including a ramping function and general motion function
 *  Must be compiled and ran in ing the Arduino development environment
 *  **/
 
const int MICROSTEP_DENOMINATOR = 16;
const int stepsPerRevolution = 200 * MICROSTEP_DENOMINATOR; 

Stepper myStepper(200, 11, 12); // more condensed than using constants manually

int stepCount = 0;  // number of steps the motor has taken


void ramp_up(int initial, int rampRate, int ramp_interval, int maxSpeed, boolean up){ // function ramps motor speed up/down to demonstrate performance capabilities and prolong motor lifetime up=true for ramping up, false for down
int  block_time = int(ramp_interval * 1000);
int t1; 
int t0;
int currentSpeed;
int speedIndex = 0;
  while (true){
    t0 = millis();
    t1 = -1; // Initial and unreachable value, delta t cannot ever be negative
    currentSpeed = initial + int(up) * (speedIndex * rampRate);
    if (currentSpeed >= maxSpeed) {
      break;
    }
    while (t1 - t0 < block_time || t1 == -1){
      myStepper.setSpeed(currentSpeed);
      myStepper.step(stepsPerRevolution / 100);
      t1 = millis();
      }
    speedIndex++;
   }
}

void move_revolutions(int RPM, int revolutions){
    myStepper.setSpeed(RPM);
   myStepper.step(revolutions);
}

int input_status(){
  // stubbed
}

void setup() {
   Serial.begin(9600);
  //ramp_up(300, 100, 1, 1600, true);
}
void loop(){
   myStepper.setSpeed(2000);
   myStepper.step(stepsPerRevolution / 100);
}

// manual PWM loop in the most fundemental way possible -- low level approach to demonstrate what is going on below the hood of abstraction
/**void drive_stepper_manual_PWM(int revs, boolean clockwise, int delay_, int DIR, int STEP){ 
  digitalWrite(DIR,HIGH);
  for(int i = 0; i < revs * stepsPerRevolution; i++) {
    digitalWrite(STEP,HIGH); 
    delayMicroseconds(delay_); 
    digitalWrite(STEP,LOW); 
    delayMicroseconds(delay_); 

  }
  
}**/
