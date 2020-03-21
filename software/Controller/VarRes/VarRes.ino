#include <SoftwareSerial.h>
#include <math.h>
SoftwareSerial BTserial(10, 11);

int state = 0;
const int mode=1; // (Preset mode=1 --> Pin towards backward)
const int VxPin = 14;  // Vx<-->A0
const int VyPin = 15;  // Vy<-->A2
double Vx = 0;
double Vy = 0;
double deltax=0;
double deltay=0;
double temp=0;
double r=0;
double theta;
int count = 0;
int pwm_parameter=0;
String str1;

void setup() 
{
  Serial.begin(9600);
  BTserial.begin(9600);
  pinMode(VxPin, INPUT);
  pinMode(VyPin, INPUT);
}

void loop() 
{
  Vx = analogRead(VxPin);
  Vy = analogRead(VyPin);
  deltax=-(Vx-506);
  deltay=-(Vy-527);
  temp=square(deltax)+square(deltay);
  r=sqrt(temp);
  if ((deltay<0) and (deltax>=0))
  {
    theta=180-(atan(deltax/deltay)*(-1)*57.29577955); 
  }
  if ((deltay<0) and (deltax<0))
  {
    theta=180+(atan(deltax/deltay)*57.29577955); 
  }
  if ((deltay>=0) and (deltax<0))
  {
    theta=360-(atan(deltax/deltay)*(-1)*57.29577955); 
  }
  if ((deltay>=0) and (deltax>=0))
  {
    theta=atan(deltax/deltay)*57.29577955; 
  }
  BTserial.write(gen(r,theta));
  Serial.print(gen(r,theta));
  Serial.println(' ');
}

byte gen(double r,double theta)
{
  byte sum=0;
  if (((theta<45) and (theta>=0)) or ((theta>=315) and (theta<360)))
  {
    pwm_parameter=r*cos(theta/57.29577955)-100;
    pwm_parameter=trunc(pwm_parameter/54);
    str1=String(128+pwm_parameter*8+pwm_parameter,BIN);
  }
  if ((theta<135) and (theta>=45))
  {
    pwm_parameter=abs(r*sin(theta/57.29577955))-100;
    pwm_parameter=trunc(pwm_parameter/51);
    str1=String(192+pwm_parameter*8+pwm_parameter,BIN);
  }
  if ((theta<225) and (theta>=135))
  {
    pwm_parameter=abs(r*cos(theta/57.29577955))-100;
    pwm_parameter=trunc(pwm_parameter/50);
    str1=String(64+pwm_parameter*8+pwm_parameter,BIN);
  }
  if ((theta<315) and (theta>=225))
  {
    pwm_parameter=abs(r*sin(theta/57.29577955))-100;
    pwm_parameter=trunc(pwm_parameter/52.5);
    str1=String(pwm_parameter*8+pwm_parameter,BIN);
  }
  for (int i=0;i<str1.length();i++)
  {
    if (str1[i]=='1')
    {
      sum=sum+(byte)(power(2,(str1.length()-i-1)));
    }
  }
  if (pwm_parameter>=0){return sum;} else {return 0;};
  
}

int power(int a,int b)
{
  if (b==0) {return 1;}
  else {return a*power(a,b-1);};
}
