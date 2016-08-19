// TO TIme a function:
// long funny_bs = millis();
// ... Function...
// ...............
// Serial.print("This functiontook: ");
// Serial.println(millis() - funny_bs);
//
//  Serial.print("AcX_c = ");
//  Serial.print(AcX_c);
//  Serial.print("AcY_c = ");
//  Serial.print(AcY_c);
//  Serial.print("AcZ_c = ");
//  Serial.print(AcZ_c);
void display_connected()
{
  for(int i = 1; i<7; i++)
  {if(connected_faces[i] >= 1){face_rgb(i,0,1,0,1); delay(150); rgboff();}}
}
void update_zero_faces()
{
  int previous_values[7] = {0,0,0,0,0,0,0};
  for(int i = 1; i <7; i++)
  {
  int light_value_ = read_ambient(i);
  if(light_value_ < 1 && light_value_ != -1){zero_faces[i] = 1; previous_values[i] = light_value_;}
  else{zero_faces[i] = 0;}
  if(opposite_face(which_face_is_up(12000)) == i)
  {zero_faces[i] = 0;}
  }
}
void taste_the_rainbow()
{
  for(int i = 1; i<5; i++)
  {
  int rr = random(0,2);
  int gg = random(0,2);
  int bb = random(0,2);
  if(rr == 0 && gg == 0 && bb == 0)
  {rr = 1; bb = 1;}
  rgb(i, random(0,2), rr, gg, bb);
  delay(10+random(1,20));
  }
  
}

int is_i2c_connected()
//
{ 
digitalWrite(Switch, HIGH);
Wire.beginTransmission(0x69);
int error = Wire.endTransmission();
return(error);
}

int which_face_is_forward(int plane) // plane should be either int 1234, 1536 2546 or -1
{
  int face = which_face_is_up(12000);
  if      (face == 2 && plane == 1234  ||  face == 6 && plane == 1536)                                 {forward_face = 1;}
  else if (face == 3 && plane == 1234  ||  face == 5 && plane == 2546)                                 {forward_face = 2;}
  else if (face == 4 && plane == 1234  ||  face == 5 && plane == 1536)                                 {forward_face = 3;}
  else if (face == 1 && plane == 1234  ||  face == 6 && plane == 2546)                                 {forward_face = 4;}
  else if (face == 4 && plane == 2546  ||  face == 1 && plane == 1536)                                 {forward_face = 5;}
  else if (face == 2 && plane == 2546  ||  face == 3 && plane == 1536)                                 {forward_face = 6;}
  else if((face == 5 && plane == 1234) || (face == 1 && plane == 2546) || (face == 2 && plane == 1536)){forward_face = 10;}
  else if((face == 6 && plane == 1234) || (face == 3 && plane == 2546) || (face == 4 && plane == 1536)){forward_face = 11;}
  else{return(-1);}
  return(forward_face);
}

int which_face_is_reverse(){
  return(opposite_face(which_face_is_forward(which_plane_fast())));
}

int which_face_is_down(){
  return(opposite_face(which_face_is_up(12000)));
}

int opposite_face(int face){
  if(face == 1){return(3);}
  else if(face == 2){return(4);}
  else if(face == 3){return(1);}
  else if(face == 4){return(2);}
  else if(face == 5){return(6);}
  else if(face == 6){return(5);}
  else{return(-1);}
}

void go_to_sleep()
{
digitalWrite(Switch, HIGH);
fbrxoff();
delay(10);
digitalWrite(Switch, HIGH);
delay(10);
for(int i = 0; i <5;i++){rgbb();delay(100);rgboff();delay(100);} // This just flashes the lights blue 5 times
delay(10);
while(Serial.available()){Serial.read();} // empty serial buffer just in case...
delay(100);
digitalWrite(Switch, LOW);
delay(100);
for(int i = 0; i < 3;i++){Serial.println("fbsleep");delay(500);}// Tell the NRF51422 to sleep the face-boards 3 seperate times.
delay(500);
digitalWrite(Switch, LOW);
delay(500);
for(int i = 0; i < 3;i++){Serial.println("espoff");delay(150);}// This should turn the lights completely off... Suicide basically
cmd = "chill";
}

int which_plane_slow(){
  int f = which_face_is_up(12000); //
  int c = which_orientation_is_central();
  if(f == -1){delay(100); f = which_face_is_up(11000);} // if cube is moving... wait, try again with lower threshold
  //
  if(f == c && f != -1 && c != -1){which_plane_am_i_in = 1234;}
  else if((f == 5 && c == 2)||(f == 4 && c == 3)||(f == 3 && c == 6)||(f == 2 && c == 1)||(f == 1 && c == 5)||(f == 6 && c == 4)){which_plane_am_i_in = 2546;}
  else if((f == 5 && c == 1)||(f == 3 && c == 4)||(f == 6 && c == 3)||(f == 1 && c == 2)||(f == 4 && c == 6)||(f == 2 && c == 5)){which_plane_am_i_in = 1536;}
  else{which_plane_am_i_in = -1;}
  return(which_plane_am_i_in);
}
int display_ring_purple(int which_plane)
{
  int del = 200;
  if     (which_plane == 1234){face_rgb(1,1,0,1,1); delay(del); face_rgb(2,1,0,1,1); delay(del); face_rgb(3,1,0,1,1); delay(del); face_rgb(4,1,0,1,1); delay(del);rgboff();}
  else if(which_plane == 1536){face_rgb(1,1,0,1,1); delay(del); face_rgb(5,1,0,1,1); delay(del); face_rgb(3,1,0,1,1); delay(del); face_rgb(6,1,0,1,1); delay(del);rgboff();}
  else if(which_plane == 2546){face_rgb(2,1,0,1,1); delay(del); face_rgb(5,1,0,1,1); delay(del); face_rgb(4,1,0,1,1); delay(del); face_rgb(6,1,0,1,1); delay(del);rgboff();}
  else if(which_plane == 52)  {blink_green(3);}
  else if(which_plane == 21)  {blink_blue(3);}
  else if(which_plane == 15)  {blink_red(3);}
  else{rgboff();}
}
int which_plane_fast()
{
  int threshold = 1700;
  int value_1234 = 4000;
  int value_2546 = 12000;
  int value_1536 = 1000;
  int magnet_value = readMagnet();
  if(magnet_value > (value_1234 - threshold) && magnet_value < (value_1234 + threshold)){which_plane_am_i_in = 1234;}
  else if(magnet_value > (value_2546 - threshold) && magnet_value < (value_2546 + threshold)){which_plane_am_i_in = 2546;}
  else if((magnet_value > (value_1536 - threshold) && magnet_value < (value_1536 + threshold)) || magnet_value > 15900){which_plane_am_i_in = 1536;}
  else{which_plane_am_i_in = -1;}
  delay(4);
  return(which_plane_am_i_in);
}

int which_orientation_is_central(){
//AcX_c, AcY_c, AcZ_c // Central IMU values
  int eps = 2800;
  int thr = 11000; // this is a value low eneough to be less than sqrt(16000) 
  int zer = 0; // Anything below this value is considered to be zero
  int z_t = 16000; // this value is meant to catch anything
  read_accel_central();
  if     (abs(AcX_c - (-thr)) < eps && abs(AcY_c - (thr)) < eps  && abs(AcZ_c - (zer)) < eps){central_orientation = 4;}
  else if(abs(AcX_c - (thr)) < eps  && abs(AcY_c - (thr)) < eps  && abs(AcZ_c - (zer)) < eps){central_orientation = 1;}
  else if(abs(AcX_c - (thr)) < eps  && abs(AcY_c - (-thr)) < eps && abs(AcZ_c - (zer)) < eps){central_orientation = 2;}
  else if(abs(AcX_c - (-thr)) < eps && abs(AcY_c - (-thr)) < eps && abs(AcZ_c - (zer)) < eps){central_orientation = 3;}
  else if(abs(AcX_c - (-zer)) < eps && abs(AcY_c - (zer)) < eps  && abs(AcZ_c - (z_t)) < eps){central_orientation = 5;}
  else if(abs(AcX_c - (-zer)) < eps && abs(AcY_c - (zer)) < eps  && abs(AcZ_c - (-z_t)) < eps){central_orientation = 6;}
  else{central_orientation = -1;} // cannot determine orientation... something is wrong
  return(central_orientation);
}

int moving_ammount(){
  read_gyro();
  return(abs(GyX)+abs(GyY)+abs(GyZ));
}

int which_face_is_up(int threshold){
  // threshold should be around 12000
  // This function returns an integer representing
  //the face which is point upwards. Returns -1 if results are inconclusive
  digitalWrite(Switch, HIGH);
  face_that_is_up = -1;
  read_accel(); // returns AcX, AcY AcZ values representing the acceleration
  int sum = (abs(AcX)+abs(AcY)+abs(AcZ));
  if(AcZ < -threshold && sum < 20000) { face_that_is_up = 1; }
  else if (AcZ > threshold && sum < 20000) { face_that_is_up = 3; }
  else if (AcX < -threshold && sum < 20000) { face_that_is_up = 6; }
  else if (AcX > threshold && sum < 20000) { face_that_is_up = 5; }
  else if (AcY < -threshold && sum < 20000) {face_that_is_up = 2; }
  else if (AcY > threshold && sum < 20000) { face_that_is_up = 4; }
  return(face_that_is_up);
}

bool IRLED_OFF(){
  for(int i = 1;i<7;i++){
    IRLED(i,0,0,0,0);
  }
}

bool IRLED(int face, bool led1, bool led2, bool led3, bool led4) {
  if (face < 1){return(false);}
  digitalWrite(Switch, HIGH);
  int i2c1 = 0x20; // Get number
  int i2c2;
  if (face > 6) {return(false);}
  i2c2 = led1 ? 0x01 : 0x00;
  i2c2 |= led2 ? 0x02 : 0x00;
  i2c2 |= led3 ? 0x04 : 0x00;
  i2c2 |= led4 ? 0x08 : 0x00;
  Wire.beginTransmission(face);
  Wire.write(byte(i2c1));
  Wire.write(byte(i2c2));
  Wire.endTransmission();
  return true;
}

bool rgb(int face, bool top, bool r, bool g, bool b){
  //fbrxen();
  digitalWrite(Switch, HIGH);
  int i2c1 = 0x00;
  int i2c2 = 0x00;
  i2c1 = top ? 0x00 : 0x01;
  i2c2 |= r ? 0x01 : 0x00;
  i2c2 |= g ? 0x02 : 0x00;
  i2c2 |= b ? 0x04 : 0x00;
  if (face > 6) {return false;}
  Wire.beginTransmission(face);
  Wire.write(byte(i2c1)); //Register for TOP LED
  Wire.write(byte(i2c2)); // 
  Wire.endTransmission();
  //fbrxoff();
}

int face_rgb(int face, int r, int g, int b, int lights_off)
{
  if(face < 1 || face > 6){return(-1);}
  if(face_that_was_last_lit_up != face){rgboff();}
  digitalWrite(Switch, HIGH);
  face_that_was_last_lit_up = face;
  delay(5);
  switch (face){
    case 1: rgb(1,1,r,g,b); rgb(1,0,r,g,b); rgb(2,0,r,g,b); rgb(2,1,r,g,b); break;
    case 2: rgb(2,1,r,g,b); rgb(2,0,r,g,b); rgb(3,0,r,g,b); rgb(3,1,r,g,b); break;
    case 3: rgb(3,1,r,g,b); rgb(3,0,r,g,b); rgb(4,0,r,g,b); rgb(4,1,r,g,b); break;
    case 4: rgb(4,1,r,g,b); rgb(4,0,r,g,b); rgb(1,0,r,g,b); rgb(1,1,r,g,b); break;
    case 5: for(int i=1;i<=4;i++){rgb(i,1,r,g,b);} break;
    case 6: for(int i=1;i<=4;i++){rgb(i,0,r,g,b);} break;
}
}


int rgbr(){
for(int i=1;i<=4;i++){
rgb(i,1,1,0,0);
rgb(i,0,1,0,0);}
}

int rgbp(){
for(int i=1;i<=4;i++){
rgb(i,1,1,0,1);
rgb(i,0,1,0,1);}
}

int rgbb(){
for(int i=1;i<=4;i++){
rgb(i,1,0,0,1);
rgb(i,0,0,0,1);}
}

int blink_green(int times)
{
for(int i = 0; i <times;i++){rgbg();delay(100);rgboff();delay(100);} 
}

int blink_purple(int times)
{
for(int i = 0; i <times;i++){rgbp();delay(100);rgboff();delay(100);} 
}

int blink_blue(int times)
{
for(int i = 0; i <times;i++){rgbb();delay(100);rgboff();delay(100);} 
}

int blink_red(int times)
{
for(int i = 0; i <times;i++){rgbr();delay(100);rgboff();delay(100);} 
}

int rgbg(){
for(int i=1;i<=4;i++){
rgb(i,1,0,1,0);
rgb(i,0,0,1,0);}
}

int rgboff() {
  digitalWrite(Switch, HIGH);
  for(int tb = 0;tb<=1;tb++)
  {
  for(int num = 1;num<=4;num++)
  {
  Wire.beginTransmission(num);// Face Board # 4 I2C address
  Wire.write(byte(tb));//Register for TOP LED
  Wire.write(byte(0x00));// 
  Wire.endTransmission();

  }
  }
}
