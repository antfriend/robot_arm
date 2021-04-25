#include "HCPCA9685.h"
//#include "HCPCA9685.cpp"
#include <Console.h>
 
#define  I2CAdd 0x40
HCPCA9685 HCPCA9685(I2CAdd);

#define dirPin 2
#define stepPin 3
#define stepsPerRevolution 5
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

int elbow_straight_up = 180;
int elbow_park = -40;
int knee_straight_up = 200;
int knee_park = -40;

int _elbow_forward_limit = elbow_park;
int _elbow_back_limit = 360;
int _knee_forward_limit = 360;
int _knee_back_limit = knee_park;
int _base_forward_limit = 100;
int _base_back_limit = 1;

int _wrist_position = wrist_clockwise;
int _wrist_step = 100;

int _elbow_position = elbow_park;
int _elbow_step = 20;
int _knee_position = knee_park;
int _knee_step = 10;

int _base_position = _base_forward_limit;
int _base_step = 2;

int iStart = 1;
int iEnd = 100;

void setup() {
  HCPCA9685.Init(SERVO_MODE);
  HCPCA9685.Sleep(false);

  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  wrist_flick(500);
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
        from_here_to_park(15);
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
        //Serial.print(_wrist_position);
        break;
      case '>':
        wrist_to_clockwise(_wrist_step);
        //Serial.print(_wrist_position);
        break;

      case 'q':
        elbow_forward(_elbow_step);
        //Serial.print(_elbow_position);
        break;
      case 'w':
        elbow_back(_elbow_step);
        //Serial.print(_elbow_position);
        break;

      case 'e':
        knee_forward(_knee_step);
        Serial.print(_knee_position);
        break;
      case 't':
        knee_back(_knee_step);
        Serial.print(_knee_position);
        break;

      case 'u':
        base_forward(_base_step);
        Serial.print(_base_position);
        break;
      case 'd':
        base_back(_base_step);
        Serial.print(_base_position);
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
    from_here_to_park(15);
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
  _wrist_position = wrist_clockwise;
  _elbow_position = elbow_straight_up;
  _knee_position = knee_straight_up;
  _base_position = 40;
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

void from_here_to_park(int milisecs){

  int original_base_position = _base_position;
  for(int step = iStart ; step < 101; step++)
  {
    _base_position = map(step, iStart, iEnd, original_base_position, _base_forward_limit);
    base_map(); 
    HCPCA9685.Servo(knee, map(step, iStart, iEnd, _knee_position, knee_park)); 
    HCPCA9685.Servo(elbow, map(step, iStart, iEnd, _elbow_position, elbow_park)); 
    HCPCA9685.Servo(wrist, map(step, iStart, iEnd, _wrist_position, wrist_counterclock)); 
    delay(milisecs);
   }
   park();
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
   park();
}

void open_claw(){
  HCPCA9685.Servo(claw, claw_open);
}

void close_claw(){
  HCPCA9685.Servo(claw, claw_closed);
}


void elbow_forward(int howFar){
  _elbow_position = _elbow_position - howFar;
  if(_elbow_position < _elbow_forward_limit){
    _elbow_position = _elbow_forward_limit;
  }
  HCPCA9685.Servo(elbow, _elbow_position);
  delay(10);
}

void elbow_back(int howFar){
  _elbow_position = _elbow_position + howFar;
  if(_elbow_position > _elbow_back_limit){
    _elbow_position = _elbow_back_limit;
  }
  HCPCA9685.Servo(elbow, _elbow_position);
  delay(10);
}

void knee_forward(int howFar){
  _knee_position = _knee_position + howFar;
  if(_knee_position > _knee_forward_limit){
    _knee_position = _knee_forward_limit;
  }
  HCPCA9685.Servo(knee, _knee_position);
  delay(10);
}

void knee_back(int howFar){
  _knee_position = _knee_position - howFar;
  if(_knee_position < _knee_back_limit){
    _knee_position = _knee_back_limit;
  }
  HCPCA9685.Servo(knee, _knee_position);
  delay(10);
}

void base_forward(int howFar){
  _base_position = _base_position + howFar;
  if(_base_position > _base_forward_limit){
    _base_position = _base_forward_limit;
  }
  base_map();
  delay(10);
}

void base_back(int howFar){
  _base_position = _base_position - howFar;
  if(_base_position < _base_back_limit){
    _base_position = _base_back_limit;
  }
  base_map();
  delay(10);
}

void base_map(){
    /* /map _base_position to both servos
  int base_left_straight_up = 130;
  int base_left_park = -40;

  int base_right_straight_up = 150;
  int base_right_park = 340;
  */
  HCPCA9685.Servo(base_left, map(_base_position, _base_back_limit, _base_forward_limit, 230, base_left_park));
  HCPCA9685.Servo(base_right, map(_base_position, _base_back_limit, _base_forward_limit, 50, base_right_park));  
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
  HCPCA9685.Servo(knee, knee_park);
  delay(50);
  HCPCA9685.Servo(elbow, elbow_park);
  delay(50);
  HCPCA9685.Servo(wrist, wrist_clockwise);
  delay(50);
  open_claw();
  delay(200);
  close_claw();
  _wrist_position = wrist_clockwise;
  _elbow_position = elbow_park;
  _knee_position = knee_park;
  _base_position = _base_forward_limit;
}
