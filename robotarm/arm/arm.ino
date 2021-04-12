#include "HCPCA9685.h"
#include <Console.h>
 
#define  I2CAdd 0x40
HCPCA9685 HCPCA9685(I2CAdd);

const int claw = 0;
const int wrist = 1;
const int elbow = 2;
const int knee = 4;

const int base_left = 5;
const int base_right = 7;

int servo_gripper_min_pos = 50;//10
int servo_gripper_max_pos = 70;//120

int base_left_straight_up = 130;
int base_left_park = -40;
int base_right_straight_up = 150;
int base_right_park = 340;

int claw_open = 380;
int claw_closed = 50;
int wrist_clockwise = 180;
int wrist_counterclock = 380;
int elbow_straight_up = 180;
int elbow_park = -40;
int knee_straight_up = 200;
int knee_park = -40;

int iStart = 1;
int iEnd = 100;

void setup() {
  HCPCA9685.Init(SERVO_MODE);
  HCPCA9685.Sleep(false);
   delay(5000);
   from_extended_to_park(10);
   routine_1();
  // park();
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN,HIGH);
  Serial.begin(9600);
}

void loop() {
  
  digitalWrite(LED_BUILTIN,LOW);
  if (Serial.available() > 0) {
    digitalWrite(LED_BUILTIN,HIGH);
    char c = Serial.read(); // read the next char received
    
    switch (c) {
      case 'p':
        from_extended_to_park(15);
        break;
      case 's':
        from_park_to_extended(30);
        break;
      default:
        routine_1();
        break;
    }
    
//    if(c == 'p'){
//      from_extended_to_park(15);
//    }
//    if(c == 's'){
//      from_park_to_extended(30);
//    }
//    if(c == 'r'){
//      routine_1();
//    }
    Serial.print(c);
    if (c == '\n') {
      //Serial.print(" [enter] ");
      delay(100);
    }
  } else {
    delay(10);
  }
}

void routine_1(){
    delay(500);
    open_claw();
    HCPCA9685.Servo(wrist, wrist_counterclock);
    delay(500);
    from_park_to_extended(5);
    delay(100);
    nibble();
    delay(500);
    close_claw();
    HCPCA9685.Servo(wrist, wrist_clockwise); 
    delay(500);   
    from_extended_to_park(20);
    wrist_flick(500);
  }

void from_park_to_extended(int milisecs){
  for(int step = iStart; step < 101; step++)
  {
    HCPCA9685.Servo(base_left, map(step, iStart, iEnd, base_left_park, base_left_straight_up));
    HCPCA9685.Servo(base_right, map(step, iStart, iEnd, base_right_park, base_right_straight_up));  
    
    HCPCA9685.Servo(knee, map(step, iStart, iEnd, knee_park, knee_straight_up)); 
    HCPCA9685.Servo(elbow, map(step, iStart, iEnd, elbow_park, elbow_straight_up)); 
    HCPCA9685.Servo(wrist, map(step, iStart, iEnd, wrist_counterclock, wrist_clockwise)); 
    delay(milisecs);
   }
}

void wrist_flick(int milisecs){
  HCPCA9685.Servo(wrist, wrist_clockwise);
  delay(milisecs);
  HCPCA9685.Servo(wrist, wrist_counterclock);
  delay(milisecs);
  HCPCA9685.Servo(wrist, wrist_clockwise);
  delay(milisecs);
  HCPCA9685.Servo(wrist, wrist_counterclock);
  delay(milisecs);
}

void nibble(){
   close_claw();
   delay(100);
   open_claw();
   delay(100);
   close_claw();
   delay(100);
   open_claw();
   delay(100);
}

void from_extended_to_park(int milisecs){
  for(int step = iStart ; step < 101; step++)
  {
    HCPCA9685.Servo(base_left, map(step, iStart, iEnd, base_left_straight_up, base_left_park));
    HCPCA9685.Servo(base_right, map(step, iStart, iEnd, base_right_straight_up, base_right_park));  
    
    HCPCA9685.Servo(knee, map(step, iStart, iEnd, knee_straight_up, knee_park)); 
    HCPCA9685.Servo(elbow, map(step, iStart, iEnd, elbow_straight_up, elbow_park)); 
    HCPCA9685.Servo(wrist, map(step, iStart, iEnd, wrist_clockwise, wrist_counterclock)); 
    delay(milisecs);
   }
}

void open_claw(){
  HCPCA9685.Servo(claw, claw_open);
}

void close_claw(){
  HCPCA9685.Servo(claw, claw_closed);
}

void park_knee(){
  HCPCA9685.Servo(knee, knee_park);
}
  
void park_base(){
  HCPCA9685.Servo(base_left, base_left_park);
  HCPCA9685.Servo(base_right, base_right_park);
}

void park(){
  park_base();
  delay(100);
  HCPCA9685.Servo(knee, -40);
  delay(100);
  HCPCA9685.Servo(elbow, -40);
  delay(100);
  HCPCA9685.Servo(wrist, 380);
  delay(100);
  open_claw();
  delay(500);
  close_claw();
}
