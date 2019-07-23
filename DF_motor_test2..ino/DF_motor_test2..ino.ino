
#include <PID_v1.h>
int i = 0;
int rpm;
unsigned long time = 0;
bool direction = HIGH;
int speed;
double Setpoint, Input, Output;  // Setpoint is going to be the desired speed. Input will be encoder reading. Output will be motor command
double Kp = 0.5; 
double Ki = 3;
double Kd = 0;

//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint,Kp,Ki,Kd, REVERSE);

void setup() {
  Serial.begin(115200);
  pinMode(10, OUTPUT); //PWM PIN 10  with PWM wire
  pinMode(11, OUTPUT);//direction control PIN 11 with direction wire
  digitalWrite(10, direction);
  Setpoint = 100;
  Input = 100;
  myPID.SetMode(AUTOMATIC);

} 

void loop() {
  if (Serial.available())  {
    speed = Serial.parseInt();
    speed = 255 - speed;
    delay(200); 
  }
  for(int j = 0;j<8;j++)  {
    i += pulseIn(9, HIGH, 500000);         //SIGNAL OUTPUT PIN 9 with  white line,cycle = 2*i,1s = 1000000us，Signal cycle pulse number：27*2
  }
  i = i >> 3;
  rpm = 111111 / i;   //speed   r/min  (60*1000000/(45*6*2*i))
  i = 0;
//  Serial.print(rpm);                      
//  Serial.println("  r/min");

  Setpoint = 255-speed;
  Input = rpm;
  myPID.Compute();                       // calculate the right motor control to make rpm equal desired speed
  if (Output > 220) {Output = 220;}
  if (Output < 20) {Output = 20;}
  Serial.print("Setpoint, Input, Output: ");
  Serial.print(Setpoint);
  Serial.print("  ");
  Serial.print(Input);
  Serial.print("  ");
  Serial.println(Output);
  analogWrite(11, Output);
}
