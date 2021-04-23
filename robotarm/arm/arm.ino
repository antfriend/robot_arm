#include "HCPCA9685.h"
//#include "HCPCA9685.cpp"
#include <Console.h>
 
#define  I2CAdd 0x40
HCPCA9685 HCPCA9685(I2CAdd);

#define dirPin 2
#define stepPin 3
#define stepsPerRevolution 100
#define stepSpeed 1000

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

int wrist_clockwise = -6;
int wrist_counterclock = 394;

int _wrist_position = 194;
int _wrist_step = 100;

int elbow_straight_up = 180;
int elbow_park = -40;
int knee_straight_up = 200;
int knee_park = -40;

int iStart = 1;
int iEnd = 100;

void setup() {
  HCPCA9685.Init(SERVO_MODE);
  HCPCA9685.Sleep(false);

  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  //delay(3000);
  //from_extended_to_park(10);
  wrist_flick(500);
  // park();
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN,HIGH);
  Serial.begin(9600);
}

void rotateClock(){
    // Set the spinning direction clockwise:
    digitalWrite(dirPin, LOW);
    for (int i = 0; i < stepsPerRevolution; i++) {
    // These four lines result in 1 step:
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(stepSpeed);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(stepSpeed);
  }
  //digitalWrite(stepPin, HIGH);
}

void rotateCounterClock(){
    // Set the spinning direction counterclockwise:
    digitalWrite(dirPin, HIGH);
    for (int i = 0; i < stepsPerRevolution; i++) {
    // These four lines result in 1 step:
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(stepSpeed);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(stepSpeed);
  }
  //digitalWrite(stepPin, HIGH);
}

void loop() {
  
  digitalWrite(LED_BUILTIN,LOW);
  if (Serial.available() > 0) {
    digitalWrite(LED_BUILTIN,HIGH);
    char c = Serial.read(); // read the next char received
    
    switch (c) {
      case 'p':
      case 'P':
        from_extended_to_park(15);
        break;
      case 's':
      case 'S':
        from_park_to_extended(30);
        break;
      case '^':
        open_claw();
        break;
      case 'v':
        close_claw();
        break;
      case '<':
        wrist_to_counterclockwise(_wrist_step);
        Serial.print(_wrist_position);
        break;
      case '>':
        wrist_to_clockwise(_wrist_step);
        Serial.print(_wrist_position);
        break;
      case 'R':
      case 'r':
        rest();
        break;
      case 'A':
      case 'a':
        activate();
        break;
      case 'K':
      case 'k':
        rotateClock();
        break;
      case 'L':
      case 'l':
        rotateCounterClock();
        break;
      default:
        //routine_1();
        Serial.print(" what? ");
        break;
    }
    
    Serial.print(c);
    if (c == '\n') {
      //Serial.print(" [enter] ");
      delay(100);
    }
  } else {
    delay(10);
  }
}

void rest(){
  HCPCA9685.Sleep(true);
}

void activate(){
  HCPCA9685.Sleep(false);
  routine_1();
}

void routine_1(){
    delay(500);
    open_claw();
    wrist_to_counterclockwise(wrist_counterclock);
    delay(500);
    from_park_to_extended(5);
    delay(100);
    nibble();
    delay(500);
    close_claw();
    wrist_to_clockwise(wrist_clockwise);
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

void wrist_to_clockwise(int howFar){
  _wrist_position = _wrist_position - howFar;
  if(_wrist_position < wrist_clockwise){
    _wrist_position = wrist_clockwise;
  }
  HCPCA9685.Servo(wrist, _wrist_position);
  delay(10);
}

void wrist_to_counterclockwise(int howFar){
  _wrist_position = _wrist_position + howFar;
  if(_wrist_position > wrist_counterclock){
    _wrist_position = wrist_counterclock;
  }
  HCPCA9685.Servo(wrist, _wrist_position);
  delay(10);
}

void wrist_flick(int milisecs){
  wrist_to_clockwise(_wrist_step);
  delay(milisecs);
  wrist_to_counterclockwise(_wrist_step);
  delay(milisecs);
  wrist_to_clockwise(_wrist_step);
  delay(milisecs);
  wrist_to_counterclockwise(_wrist_step);
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
  delay(50);
  HCPCA9685.Servo(knee, -40);
  delay(50);
  HCPCA9685.Servo(elbow, -40);
  delay(50);
  HCPCA9685.Servo(wrist, 380);
  delay(50);
  open_claw();
  delay(200);
  close_claw();
}
