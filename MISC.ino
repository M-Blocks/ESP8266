//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// I2C COMMANDS!!!///////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool face_board_sleep(int face)
{
  if (face < 1 || face > 6){return(false);}
  digitalWrite(Switch, HIGH);
  int i2c1 = 0x50; // FB_REGISTER_ADDR_RX_FLUSH  
  int i2c2 = 0x01;
  Wire.beginTransmission(face);
  Wire.write(byte(i2c1));
  Wire.write(byte(i2c2));
  Wire.endTransmission();
  return(true);
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

int opposite_face(int face)
{
  if(face == 1){return(3);}
  else if(face == 2){return(4);}
  else if(face == 3){return(1);}
  else if(face == 4){return(2);}
  else if(face == 5){return(6);}
  else if(face == 6){return(5);}
  else{return(-1);}
}

void turn_off_esp()
{
digitalWrite(Switch, HIGH);
delay(100);
fbrxen(false);
blink_yellow_old(2); // This just flashes the lights blue 5 times
int test_time = millis();
while(Serial.available() && (millis() - test_time) < 300){Serial.read();} // empty serial buffer just in case...
delay(100);
digitalWrite(Switch, LOW);
delay(200);
Serial.println("stillalive");
for(int i = 0; i < 2;i++){Serial.println("fbsleep");delay(300);}// Tell the NRF51422 to sleep the face-boards 3 seperate times.
digitalWrite(Switch, LOW);
for(int i = 0; i < 2;i++){Serial.println("fbsleep");delay(300);}// Tell the NRF51422 to sleep the face-boards 3 seperate times.
delay(500);
for(int i = 0; i < 4;i++){Serial.println("espoff");delay(400);}// This should turn the lights completely off... Suicide basically
cmd = "chill";
}

void go_to_sleep()
{
digitalWrite(Switch, HIGH);
delay(100);
fbrxen(false);
blink_blue_old(6); // This just flashes the lights blue 5 times
int test_time = millis();
while(Serial.available() && (millis() - test_time) < 300){Serial.read();} // empty serial buffer just in case...
delay(100);
digitalWrite(Switch, LOW);
delay(100);
for(int i = 0; i < 2;i++){Serial.println("fbsleep");delay(300);Serial.println("stillalive");delay(100);}// Tell the NRF51422 to sleep the face-boards 3 seperate times.
digitalWrite(Switch, LOW);
for(int i = 0; i < 2;i++){Serial.println("fbsleep");delay(300);Serial.println("stillalive");delay(100);}// Tell the NRF51422 to sleep the face-boards 3 seperate times.
delay(200);
for(int i = 0; i < 4;i++){Serial.println("sleep");delay(400);Serial.println("stillalive");delay(100);}// This should turn the lights completely off... Suicide basically
cmd = "chill";
}

int which_plane_slow()
{
  int f = which_face_is_up(12000); //
  int c = which_orientation_is_central();
  if(f == -1){delay(100); f = which_face_is_up(11000);} // if cube is moving... wait, try again with lower threshold
  //
       if(f == c && f != -1 && c != -1)                                                                                          {which_plane_am_i_in = 1234;}
  else if((f == 5 && c == 2)||(f == 4 && c == 3)||(f == 3 && c == 6)||(f == 2 && c == 1)||(f == 1 && c == 5)||(f == 6 && c == 4)){which_plane_am_i_in = 2546;}
  else if((f == 5 && c == 1)||(f == 3 && c == 4)||(f == 6 && c == 3)||(f == 1 && c == 2)||(f == 4 && c == 6)||(f == 2 && c == 5)){which_plane_am_i_in = 1536;}
  else{which_plane_am_i_in = -1;}
  return(which_plane_am_i_in);
}

int which_plane_fast()
{
  int threshold = 1700;
  int value_1234 = 4000;
  int value_2546 = 12000;
  int value_1536 = 1000;
  int magnet_value = readMagnet();
       if(magnet_value > (value_1234 - threshold) && magnet_value < (value_1234 + threshold))                            {which_plane_am_i_in = 1234;}
  else if(magnet_value > (value_2546 - threshold) && magnet_value < (value_2546 + threshold))                            {which_plane_am_i_in = 2546;}
  else if((magnet_value > (value_1536 - threshold) && magnet_value < (value_1536 + threshold)) || magnet_value > 15900)  {which_plane_am_i_in = 1536;}
  else{which_plane_am_i_in = -1;}
  delay(4);
  return(which_plane_am_i_in);
}

int which_orientation_is_central(){
//AcX_c, AcY_c, AcZ_c // Central IMU values
  int central_orientation = 0;
  int eps = 2800;
  int thr = 11000; // this is a value low eneough to be less than sqrt(16000) 
  int zer = 0; // Anything below this value is considered to be zero
  int z_t = 16000; // this value is meant to catch anything
  read_accel_central();
  if     (abs(AcX_c - (-thr)) < eps && abs(AcY_c - (thr)) < eps  && abs(AcZ_c - (zer)) < eps) {central_orientation = 4;}
  else if(abs(AcX_c - (thr)) < eps  && abs(AcY_c - (thr)) < eps  && abs(AcZ_c - (zer)) < eps) {central_orientation = 1;}
  else if(abs(AcX_c - (thr)) < eps  && abs(AcY_c - (-thr)) < eps && abs(AcZ_c - (zer)) < eps) {central_orientation = 2;}
  else if(abs(AcX_c - (-thr)) < eps && abs(AcY_c - (-thr)) < eps && abs(AcZ_c - (zer)) < eps) {central_orientation = 3;}
  else if(abs(AcX_c - (-zer)) < eps && abs(AcY_c - (zer)) < eps  && abs(AcZ_c - (z_t)) < eps) {central_orientation = 5;}
  else if(abs(AcX_c - (-zer)) < eps && abs(AcY_c - (zer)) < eps  && abs(AcZ_c - (-z_t)) < eps){central_orientation = 6;}
  else                                                                                        {central_orientation = -1;} // cannot determine orientation... something is wrong
  return(central_orientation);
}

int which_face_is_up(int threshold){
  // threshold should be around 12000
  // This function returns an integer representing
  //the face which is point upwards. Returns -1 if results are inconclusive
  digitalWrite(Switch, HIGH);
  face_that_is_up = -1;
  read_accel(); // returns AcX, AcY AcZ values representing the acceleration
  int sum = (abs(AcX)+abs(AcY)+abs(AcZ));
  if(AcZ < -threshold && sum < 25000)        { face_that_is_up = 1; }
  else if (AcZ > threshold && sum < 25000)   { face_that_is_up = 3; }
  else if (AcX < -threshold && sum < 25000)  { face_that_is_up = 6; }
  else if (AcX > threshold && sum < 25000)   { face_that_is_up = 5; }
  else if (AcY < -threshold && sum < 25000)  { face_that_is_up = 2; }
  else if (AcY > threshold && sum < 25000)   { face_that_is_up = 4; }
  return(face_that_is_up);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// LED COMMANDS!!!///////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void taste_the_rainbow()
{
  //Serial.println("Tring to taste...");
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


int display_ring_purple(int which_plane)
{
  int del = 200;
  if     (which_plane == 1234){face_rgb(1,1,0,1,1); delay(del); face_rgb(2,1,0,1,1); delay(del); face_rgb(3,1,0,1,1); delay(del); face_rgb(4,1,0,1,1); delay(del);rgboff();}
  else if(which_plane == 1536){face_rgb(1,1,0,1,1); delay(del); face_rgb(5,1,0,1,1); delay(del); face_rgb(3,1,0,1,1); delay(del); face_rgb(6,1,0,1,1); delay(del);rgboff();}
  else if(which_plane == 2546){face_rgb(2,1,0,1,1); delay(del); face_rgb(5,1,0,1,1); delay(del); face_rgb(4,1,0,1,1); delay(del); face_rgb(6,1,0,1,1); delay(del);rgboff();}
  else if(which_plane == 52)  {blink_green_old(3);}
  else if(which_plane == 21)  {blink_blue_old(3);}
  else if(which_plane == 15)  {blink_red_old(3);}
  else{rgboff();}
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
  return(true);
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
  if (face > 6) {return(false);}
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
  switch (face)
  {
    case 1: rgb(1,1,r,g,b); rgb(1,0,r,g,b); rgb(2,0,r,g,b); rgb(2,1,r,g,b); break;
    case 2: rgb(2,1,r,g,b); rgb(2,0,r,g,b); rgb(3,0,r,g,b); rgb(3,1,r,g,b); break;
    case 3: rgb(3,1,r,g,b); rgb(3,0,r,g,b); rgb(4,0,r,g,b); rgb(4,1,r,g,b); break;
    case 4: rgb(4,1,r,g,b); rgb(4,0,r,g,b); rgb(1,0,r,g,b); rgb(1,1,r,g,b); break;
    case 5: for(int i=1;i<=4;i++) {rgb(i,1,r,g,b);}                         break;
    case 6: for(int i=1;i<=4;i++) {rgb(i,0,r,g,b);}                         break;
  }
}

int rgbr(){
for(int i=1;i<=4;i++){
rgb(i,1,1,0,0);
rgb(i,0,1,0,0);}
}

int rgbg(){
for(int i=1;i<=4;i++){
rgb(i,1,0,1,0);
rgb(i,0,0,1,0);}
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

int rgby(){
for(int i=1;i<=4;i++){
rgb(i,1,1,1,0);
rgb(i,0,1,1,0);}
}


int irall(){
for(int face=1;face<7;face++){IRLED(face,1,1,1,1);}
}


int rgbt(){
for(int i=1;i<=4;i++){
rgb(i,1,0,1,1);
rgb(i,0,0,1,1);}
}

int rgbw(){
for(int i=1;i<=4;i++){
rgb(i,1,1,1,1);
rgb(i,0,1,1,1);}
}

void blink_teal()
{
  if(t_counter < 1){return;} // 
  else
    {
      if(t_counter % 4 == 0 && t_counter > 0)        {rgbt();}
      else if((t_counter+2) % 4 == 0 && t_counter > 0)  {rgboff();}
      else                          {}
    }
    t_counter = t_counter - 1;
}


void blink_purple()
{
  if(p_counter < 1){return;} // 
  else
    {
      if(p_counter % 4 == 0 && p_counter > 0)        {rgbp();}
      else if((p_counter+2) % 4 == 0 && p_counter > 0)  {rgboff();}
      else                          {}
    }
    p_counter = p_counter - 1;
}

void blink_red()
{
  if(r_counter < 1){return;} // 
  else
    {
      if(r_counter % 4 == 0 && r_counter > 0)        {rgbr();}
      else if((r_counter+2) % 4 == 0 && r_counter > 0)  {rgboff();}
      else                          {}
    }
    r_counter = r_counter - 1;
}

void blink_yellow()
{
  if(y_counter < 1){return;} // 
  else
    {
      if(y_counter % 4 == 0 && y_counter > 0)        {rgby();}
      else if((y_counter+2) % 4 == 0 && y_counter > 0)  {rgboff();}
      else                          {}
    }
    y_counter = y_counter - 1;
}

void blink_blue()
{
  if(b_counter < 1){return;} // 
  else
    {
      if(b_counter % 4 == 0 && b_counter > 0)        {rgbb();}
      else if((b_counter+2) % 4 == 0 && b_counter > 0)  {rgboff();}
      else                          {}
    }
    b_counter = b_counter - 1;
}

void blink_ir()
{
  if(ir_counter[0] < 1){return;} // 
  else
    {
      if(ir_counter[0] % 4 == 0 && ir_counter[0] > 0)        {irall();}
      else if((ir_counter[0]+2) % 4 == 0 && ir_counter[0] > 0)  {IRLED_OFF();}
      else                          {}
    }
    ir_counter[0] = ir_counter[0] - 1;
}

void blink_ir_face()
{
  for(int i = 1; i <7; i++)
  {
  if(ir_counter[i] < 1){delayMicroseconds(5);} // 
  else
    {
      if(ir_counter[i] % 4 == 0 && ir_counter[i] > 0)        {IRLED(i,1,1,1,1);}
      else if((ir_counter[i]+2) % 4 == 0 && ir_counter[i] > 0)  {IRLED_OFF();}
      else                          {}
    }
    ir_counter[i] = ir_counter[i] - 1;
  }
}

void blink_white()
{
  if(w_counter < 1){return;} // 
  else
    {
      if(w_counter % 4 == 0 && w_counter > 0)        {rgbw();}
      else if((w_counter+2) % 4 == 0 && w_counter > 0)  {rgboff();}
      else                          {}
    }
    w_counter = w_counter - 1;
}

void blink_green()
{
  if(g_counter < 1){return;} // 
  else
    {
      if(g_counter % 4 == 0 && g_counter > 0)        {rgbg();}
      else if((g_counter+2) % 4 == 0 && g_counter > 0)  {rgboff();}
      else                          {}
    }
    g_counter = g_counter - 1;
}

int blink_green_old(int times)
{
Serial.println("stillalive");
for(int i = 0; i <times;i++){rgbg();delay(100);rgboff();delay(75);} 
}

int blink_yellow_old(int times)
{
Serial.println("stillalive");
for(int i = 0; i <times;i++){rgby();delay(100);rgboff();delay(75);} 
}

int blink_purple_old(int times)
{
Serial.println("stillalive");
for(int i = 0; i <times;i++){rgbp();delay(100);rgboff();delay(75);} 
}

int blink_blue_old(int times)
{
Serial.println("stillalive");
for(int i = 0; i <times;i++){rgbb();delay(100);rgboff();delay(75);} 
}

int blink_red_old(int times)
{
Serial.println("stillalive");
for(int i = 0; i <times;i++){rgbr();delay(100);rgboff();delay(75);} 
}

int blink_teal_old(int times)
{
Serial.println("stillalive");
for(int i = 0; i <times;i++){rgbr();delay(100);rgboff();delay(75);} 
}

int blink_white_old(int times)
{
Serial.println("stillalive");
for(int i = 0; i <times;i++){rgbw();delay(800);rgboff();delay(750);} 
}

int blink_IR_old(int times)
{
Serial.println("stillalive");
for(int i = 0; i <times;i++){irall();delay(100);IRLED_OFF();delay(750);} 
}

int blink_rainbow_old(int times)
{
for(int i = 0; i <times;i++)
{Serial.println("stillalive");rgby();delay(130);rgbr();delay(130);rgbp();Serial.println("stillalive");delay(130);rgbb();delay(130);rgbt();delay(130);rgbg();delay(130);Serial.println("stillalive");} 
}

int rgboff() 
{
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

int blink_red_length(int len)
{
rgbr();delay(len);rgboff();
} 



