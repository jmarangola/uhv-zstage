
#include <Stepper.h>
/** @ John Marangola
 *  Controls TB6560 stepper driver on arduino uno or equivalent microprocessor.
 *  Tailored for Nema 23 stepper motor
 *  Just a basic module to control the function of a stepper including a ramping function and general motion function
 *  Must be compiled and ran in ing the Arduino development environment
 *  pins : 2, 3 are deploy/retract
 *  STP:
 *  DIR:
 *  pos_x:
 *  pos_y:
 *  pos_z:
 *  vin: + (ps)
 *  gnd : - (ps)
 *  **/
 #include <AccelStepper.h>

// Stepper Parameters:
const int MICROSTEP_DENOMINATOR = 16;
int STEPS_PER_REV = 200;
const int RESOLUTION = STEPS_PER_REV * MICROSTEP_DENOMINATOR;
const int MAXIMUM_ACCELERATION = 100000.0;
const int MAXIMUM_SPEED = 5000.0;

bool invert = false;
bool blocking = false;

// limits
const int UPPER_LIMIT = 7;
const int LOWER_LIMIT = 8;

// Input pins (PU)
const int RETRACT = 2;
const int DEPLOY = 3;
const int XP = 4;
const int YP = 5;
const int ZP = 6;
const int ZERO = 7;

//2 Pin Stepper control
const int STP = 11;
const int DIR = 12;

// Position indices:
const int POS[3] = {1000, 1000, 1000};
bool recalibrate = false;
int state = 0;

AccelStepper ZStage(AccelStepper::FULL2WIRE, 11, 12);

/*
// Manual stair ramping of acceleration
void ramp_up(int initial, int rampRate, int ramp_interval, int maximumSpeed, boolean up){ // function ramps motor speed up/down to demonstrate performance capabilities and prolong motor lifetime up=true for ramping up, false for down
  int  block_time = int(ramp_interval * 1000);
  int t1; 
  int t0;
  int currentSpeed;
  int speedIndex = 0;
  while (true){
    t0 = millis();
    t1 = -1; // Initial and unreachable value, delta t cannot ever be negative
    currentSpeed = initial + int(up) * (speedIndex * rampRate);
    if (currentSpeed >= maximumSpeed) 
      break;
    while (t1 - t0 < block_time || t1 == -1){
      ZStage.setSpeed(currentSpeed);
      ZStage.step(STEPS_PER_REV / 100);
      t1 = millis();
      }
    speedIndex++;
  }*/
   
void setup(){ // initial params
  Serial.begin(9600);
  ZStage.setMaxSpeed(MAXIMUM_SPEED);
  ZStage.setAcceleration(MAXIMUM_ACCELERATION);
  
  // internally pullup upper/lower limits from floating
  pinMode(LOWER_LIMIT, INPUT_PULLUP); 
  pinMode(UPPER_LIMIT, INPUT_PULLUP);
  pinMode(RETRACT, INPUT_PULLUP);
  pinMode(DEPLOY, INPUT_PULLUP);
  pinMode(XP, INPUT_PULLUP);
  pinMode(YP, INPUT_PULLUP);
  pinMode(ZP, INPUT_PULLUP);
  pinMode(ZERO, INPUT_PULLUP);
  

  ZStage.setPinsInverted(false, false, false);
  ZStage.setCurrentPosition(0);

} 
bool isAtLimit(){
  return (digitalRead(LOWER_LIMIT) == LOW || digitalRead(UPPER_LIMIT) == LOW);
}

void loop(){
 
   if (digitalRead(RETRACT) == LOW){ // Retract
    //Serial.println("RETRACT");
    //Check if not at limit or at upper limit
      //if (!isAtLimit() || digitalRead(UPPER_LIMIT) == LOW){}
         
         ZStage.setSpeed(5000);
         ZStage.run();
   }
   else if (digitalRead(DEPLOY) == LOW){ // Deploy
      //Serial.println("Deploy");
        // Check if not at limit or at lower limit 
        //if (!isAtLimit() || digitalRead(LOWER_LIMIT) == LOW) {
          //ZStage.runSpeed(MAXIMUM_SPEED);
       // }
        
         ZStage.setSpeed(-5000);
         ZStage.run();
   }
   else if (!digitalRead(DEPLOY) == LOW && !digitalRead(XP) == LOW){
    Serial.println(ZStage.currentPosition());
   }
   
   else if (digitalRead(XP) == LOW){
   
      Serial.println("MOVE TO X");
      ZStage.moveTo(POS[0]);
      Serial.println(ZStage.currentPosition());
      ZStage.runToPosition();
      Serial.println(ZStage.targetPosition());
      Serial.println(ZStage.currentPosition());
   }
   
   else if (digitalRead(YP) == LOW){
    // Move to y
      //ZStage.runToNewPosition(POS[1]);
   }
   
   else if (digitalRead(ZP) == LOW){
      // Move to Z
      //ZStage.runToNewPosition(POS[2]);
   }
   
   else if (digitalRead(ZERO) == LOW || recalibrate){
      // Calibrate zero to lower limit
      if (isAtLimit()){
        recalibrate = false;
        Serial.println("Re-calibrating position...");
        Serial.print("Stepping with ");
        Serial.print(RESOLUTION);
        Serial.println("microsteps per revolution");
       // ZStage.setCurrentPosition(0);
        Serial.println("Current position: ");
        Serial.println(ZStage.currentPosition());
    }
     else {
       
     } 
    
   }
   
}
