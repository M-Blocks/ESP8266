  int get_battery_voltage()
  {
    long begin_function = millis();
    while(Serial.available()){Serial.read();} // empty serial buffer just in case...
    delay(3);
    Serial.println("vbat");
    delay(23);
    char prev_char = ' ';
    String temp_string = "";
    int battery_counter = 1;
    while (Serial.available() > 0 && (millis()-begin_function) < 60) // while there are things in the serial buffer...
    {
        char c = Serial.read();
        if(c == ' ' && prev_char == ':')
          {
              for(int i = 0; i < 4; i++)
                  {
                  char a = Serial.read();
                  delayMicroseconds(250);
                  if(isDigit(a)){temp_string += a;}
                  } 
              if(battery_counter == 1){vbat[1] = temp_string.toInt();}
              else if(battery_counter == 2){vbat[2] = temp_string.toInt();}
              else if(battery_counter == 3){vbat[3] = temp_string.toInt();}
              else if(battery_counter == 4){vbat[4] = temp_string.toInt();}
              battery_counter++;
              temp_string = ""; 
          }
        delayMicroseconds(200);
        prev_char = c;
    }
  vbat[0] = (vbat[1]+vbat[2]+vbat[3]+vbat[4])/4;
  return (vbat[0]);
  
//  Serial.print("This part of this took: ");
//  Serial.println(millis() - begin_function);
}

int get_vin()
{
  long begin_function = millis();
  while(Serial.available()){Serial.read();} // empty serial buffer just in case...
  delay(5);
  Serial.println("vin");
  delay(23);
  char prev_char      = ' ';
  String temp_string  = "";
  int temp_vin        = 0;
  while (Serial.available() > 0 && (millis()-begin_function) < 60) // while there are things in the serial buffer...
  {
        char c = Serial.read();
        if(c == ' ' && prev_char == ':')
          {
              for(int i = 0; i < 4; i++)
                  {
                  char a = Serial.read();
                  delayMicroseconds(250);
                  if(isDigit(a)){temp_string += a;}
                  } 
              temp_vin = temp_string.toInt();
        
          }
        delayMicroseconds(250);
        prev_char = c;
  }
  return(temp_vin);
//  Serial.print("This part of this took: ");
//  Serial.println(millis() - begin_function);
}

int read_accel_central(){ 
// Function typically takes about 30 milliseconds to complete
// This function attempts to update these variables
// AcX_c, AcY_c, AcZ_c
// Yes this is a giant clusterfuck of convoluted logic... but it seems to work
// this is the format of the string returned from the NRF51422:
// imugravityi: 7800 534 31818
  long begin_function = millis(); // record the moment when the program starts
  while(Serial.available()){Serial.read();} //This is meant to flush any existing serial characters
  delay(5);
  Serial.println("imugravityi"); // Prints imugravityi to the NRF51422
  delay(20); // Wait a little while for the the Nrf51422 to process this command
  String temp_string = ""; // Setup a string to begin 
  int loop_count = 1; // yes... this is the only way I know how to do this
  char c = 'c';
  while (Serial.available() > 0 && (millis()-begin_function) < 50) // while there are things in the serial buffer...
  {
        if(c != ' '){c = Serial.read();}
        if(c == ' ') // 
            {
                delayMicroseconds(150);
                c = Serial.read();
                while(c != ' ' && Serial.available() > 0)
                  {
                  temp_string += c;
                  delayMicroseconds(150);
                  c = Serial.read();
                  }
                if(loop_count == 1){AcX_c = temp_string.toInt()/2;}
                else if(loop_count == 2){AcY_c = temp_string.toInt()/2;}
                else if(loop_count == 3){AcZ_c = temp_string.toInt()/2;}
                loop_count++;
                temp_string = ""; // reset the temporary string
             } 
   }
  return (abs(AcX_c)+abs(AcY_c)+abs(AcZ_c));
}

int read_ambient(int face)//(int face)
{
  int reading = 0;
  digitalWrite(Switch, HIGH);
  if(face > 6 || face < 1) {return(-1);}
  Wire.beginTransmission(face); 
  Wire.write(byte(0x10)); // this is the register where the Ambient values are stored
  Wire.endTransmission();
  Wire.requestFrom(face, 2);
  if (2 <= Wire.available()) //ambientLight  = twiBuf[0] << 2;
  {
    reading =  Wire.read()<<1;     //  ambientLight |= twiBuf[1] >> 6;
    reading |=  Wire.read()>>7;
    return reading;
  }
  else{return(-1);}
}

void wake_accel(){
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
}

long read_accel(){
  digitalWrite(Switch, HIGH);
  wake_accel();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,6,1);
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
//  Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
//  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
//  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
//  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  return(abs(AcX)+abs(AcY)+abs(AcZ));
}

long read_gyro_and_accel(){
  digitalWrite(Switch, HIGH);
  wake_accel();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,1);
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  return((abs(GyX)+abs(GyY)+abs(GyZ)+abs(AcX) + abs(AcY) + abs(AcZ))/10);
}

long read_gyro(){
  digitalWrite(Switch, HIGH);
  wake_accel();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x41);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,8,1);
//  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
//  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
//  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  return(abs(GyX)+abs(GyY)+abs(GyZ));
}

void fbrxen(bool state) // Turns on Ambient light sensor
{
  if(DEBUG){Serial.println("Beginning FBRXEN");}
  digitalWrite(Switch, HIGH); // Takecontrol of I2C BUS
  delay(1);
  for (int face = 1; face < 7; face++) {
  Wire.beginTransmission(face);
  Wire.write(byte(0x43));
  if(state) {Wire.write(byte(0x01));}
  else      {Wire.write(byte(0x00));}
  Wire.endTransmission(); 
  }
}

int readMagnet()
{
  if(DEBUG){Serial.println("Beginning ReadMagnet");}
  Wire.beginTransmission(0x40);
  Wire.write(byte(255));
  Wire.endTransmission();
  Wire.requestFrom(0x40, 2);

  int reading = 0;
  if (2 <= Wire.available())
  {
    reading = Wire.read();
    reading = reading << 6;
    reading |= Wire.read();
  }
  return reading;
}
