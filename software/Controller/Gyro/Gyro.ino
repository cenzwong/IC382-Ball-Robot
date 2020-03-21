#include <Wire.h>
#include <math.h>
int I2C_Address = 0xA7 >> 1; // ADXL345 的 I2C 地址
void setReg(int reg, int data);
int getData(int reg);

int X0, X1, Y0, Y1;
float X, Y, Xa, Ya, Angle, xy, R;
char xstatus[256];

const int X_pin = 6;
const int Y_pin = 7;

//R is the magnitude ranging from 100 to 255
double Get_R(float a, float b){
  if(a > -100 && a <100){a =0;}
  if(b > -100 && b <100){b =0;}
  return sqrt (a*a + b*b); 
  }

//this function returns angle in degree 90 degree forward, 0 degree right side
double Get_Angle(float a, float b){
  double rad = atan2(Xa,Ya);

  if ((rad > -3.14) && (rad < 0))
  {
     rad = rad*100;
     rad = map(rad, -314, 0, 314, 628); 
     analogWrite(8, X);                 
     rad = rad/100;
  }
  return (rad*4068/71);
  }
 

void setup() {
  Serial.begin(115200);
  Serial.println("Start : \n-------------------------------------------------------------");
  Wire.begin();  //初始化 I2C
  setReg(0x2D, 0xA); // (打開電源, 設定輸出資料速度為 100 Hz)
}


void loop() {

//gyro mode
  X0 = getData(0x32); // 取得 X 軸 低位元資料
  X1 = getData(0x33); // 取得 X 軸 高位元資料
  X = ((X1 << 8)  + X0 );
  Xa = X;
 
  Y0 = getData(0x34); // 取得 Y 軸 低位元資料
  Y1 = getData(0x35); // 取得 Y 軸 高位元資料
  Y = -((Y1 << 8)  + Y0 );
  Ya = Y;

  
  if((X>=-100)&&(X<=100)){X=0;}
  
  if((X>=-260)&&(X<=-100)){
  X = map(X, -100, -260, 0, 255);
  analogWrite(8, X);}
  

  if((X>=100)&&(X<=260)){
  X = map(X, 100, 260, 0, 255);
  analogWrite(8, X);}

  if((Y>=-100)&&(Y<=100)){Y=0;}
  
  if((Y>=-260)&&(Y<=-100)){
  Y = map(Y, -100, -260, 0, 255);
  analogWrite(9, Y);}
  

  if((Y>=100)&&(Y<=260)){
  Y = map(Y, 100, 260, 0, 255);
  analogWrite(9, Y);}
 
    
  Serial.print("X= ");
  Serial.print(X);
  Serial.print("    Y= ");
  Serial.print(Y);
  Serial.print("    Xa= ");
  Serial.print(Xa);
  Serial.print("    Ya= ");
  Serial.print(Ya);
  Serial.print("    R=  ");
  Serial.print(Get_R(Xa,Ya));
  Serial.print("    Angle=");
  Serial.println(Get_Angle(Xa,Ya));
  delay(250);
  }

/* setReg(reg,data)：寫入佔存器
   參數：
       reg → 指定佔存器位址
       data → 要寫入的資料
*/
void setReg(int reg, int data) {
  Wire.beginTransmission(I2C_Address);
  Wire.write(reg); // 指定佔存器
  Wire.write(data); // 寫入資料
  Wire.endTransmission();
}

/* getData(reg)：取得佔存器裡的資料
   參數：reg → 佔存器位址
*/
int getData(int reg) {
  Wire.beginTransmission(I2C_Address);
  Wire.write(reg);
  Wire.endTransmission();

  Wire.requestFrom(I2C_Address, 1);

  if (Wire.available() <= 1) {
    return Wire.read();
  }
}
