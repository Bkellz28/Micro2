#include<Wire.h>

int VRx = A0;
int VRy = A1;

int buzzer = 12;

int xInput = 0;
int yInput = 0;
int SW_state = 0;
int mapX = 0;
int mapY = 0;
char up = 'w', down = 's', left = 'a' , right = 'd';

int dataIn = 0;

const int mcAddr = 0x68;  // I2C address of the MPU-6050 (microprocessorAddress)
int16_t gyroX,gyroY,gyroZ;

void setup() {
  Wire.begin();
  Wire.beginTransmission(mcAddr);
  Wire.write(0x6B);   //PWR_MGMT_1 register
  Wire.write(0);      //set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  
  Serial.begin(9600);
  
  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  pinMode(buzzer, OUTPUT);
}

void loop() {

  //Read joystick for joystick inputs
  xInput = analogRead(VRx);
  yInput = analogRead(VRy);
  mapX = map(xInput, 0, 1023, -512, 512);
  mapY = map(yInput, 0, 1023, -512, 512);

  //Read gyroscope for tilt input
  Wire.beginTransmission(mcAddr); 
  Wire.write(0x43);     //starting with register 0x43
  Wire.endTransmission(false);
  Wire.requestFrom(mcAddr, 6, true); //request 6 registers
  gyroX = Wire.read() << 8 | Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  gyroY = Wire.read() << 8 | Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  gyroZ = Wire.read() << 8 | Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  //Send instructions to game based on inputs by printing to serial monitor
  //OR joystick and gyro input vars so either device is accepted
  if((mapX > 300) || (gyroX < -4000)){
    Serial.print(right);
    delay(100);
  }
  else if((mapX < -300) || (gyroX > 4000)){
    Serial.print(left);
    delay(100);
  }
  else if((mapY > 300) || (gyroY > 4000)){
    Serial.print(up);
    delay(100);
  }
  else if((mapY < -300) || (gyroY < -4000)){
    Serial.print(down);
    delay(100);
  }

  //read data from game to check for apple eaten flag
  dataIn = Serial.read();
  Serial.println(dataIn);

  if(dataIn == 'A'){ //activate buzzer if data read is apple eaten flag
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
  }
    
}
