#include<Wire.h>

int VRx = A0;
int VRy = A1;

int buzzer = 8;

int xPosition = 0;
int yPosition = 0;
int SW_state = 0;
int mapX = 0;
int mapY = 0;
char up = 'w', down = 's', left = 'a' , right = 'd';

int incomingByte = 0;

const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t GyX,GyY,GyZ;

void setup() {
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);   //PWR_MGMT_1 register
  Wire.write(0);      //set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  
  Serial.begin(9600);
  
  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  pinMode(buzzer, OUTPUT);
}

void loop() {

  //Read joystick for joystick values
  xPosition = analogRead(VRx);
  yPosition = analogRead(VRy);
  mapX = map(xPosition, 0, 1023, -512, 512);
  mapY = map(yPosition, 0, 1023, -512, 512);

  //Read gyroscope for values
  Wire.beginTransmission(MPU_addr); 
  Wire.write(0x43);     //starting with register 0x43
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 6, true); //request 6 registers
  GyX = Wire.read() << 8 | Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY = Wire.read() << 8 | Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ = Wire.read() << 8 | Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  //For if statements, OR joystick values with gyro values so whichever one is detected, the program takes
  //converting code to readable info to python
  //printing values to serial monitor
  if((mapX > 200) || (GyX < -4000)){
    Serial.print(right);
    delay(100);
  }
  else if((mapX < -200) || (GyX > 4000)){
    Serial.print(left);
    delay(100);
  }
  else if((mapY > 200) || (GyY > 4000)){
    Serial.print(up);
    delay(100);
  }
  else if((mapY < -200) || (GyY < -4000)){
    Serial.print(down);
    delay(100);
  }

  //read flag from python if apple is eaten
  if(Serial.available() > 0){
    incomingByte = Serial.read();
    Serial.println(incomingByte);

    if(incomingByte == 'A'){
      digitalWrite(buzzer, HIGH);
      delay(100);
      digitalWrite(buzzer, LOW);
    }
    
  }
  
}
