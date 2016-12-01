//bool send_ir_message_string(int face, String message)
//{
//  long begin_time = millis();
//  IRLED(face,0,0,0,0);
//  delay(1);
//  fbtxled(face,1,1,1,1);
//  delay(1);
//  fbtx_send_msg_string(face, message.length(), message);
//  i_just_sent_an_ir_message[face] = 3;
//  return(true);
//}

//bool fbtx_send_msg_string(int face, int numBytes, String message)
//{
//    digitalWrite(Switch, HIGH);
//    char twiBuf[30];
//    if (face > 6 || face < 1) {return false;}
//    twiBuf[0] = 0x30; //FB_REGISTER_ADDR_TX_BUF;
//    twiBuf[1] = twiBuf[2] = twiBuf[3] = 'l';    
//    for(int i = 0; i < numBytes; i++){twiBuf[i+4] = message.charAt(i);}
//    
//    Wire.beginTransmission(face);
//    for(int i = 0; i < (4 + numBytes); i++)
//    {
//     Wire.write(byte(twiBuf[i]));
//    }
//    Wire.endTransmission();
//    return(true);
//}


//#include <Adafruit_MPL3115A2.h>\
//  baro.begin(); // Eveuntually get average from BARO - long initial_baro;

//long initial_baro; // this will be the calibrated ofset for the barometer

  /////////////////////////// Populate 5 bit ID //////////////////////////
//  int j = 0;
//  for(int i = 4; i >= 0 ; i--)  
//  {five_bit_id_array[i] = bitRead(five_bit_id_int, j); j++;}
  ////////////////////////////////////////////////////////////////////////


  //
//bool read_message_value    (char message[])
//{
//  if(DEBUG){Serial.println("read_message_value");}
//  bool success = true;
//  if(isdigit(message[message_value_index_1]) && isdigit(message[message_value_index_1+1])) // Check if both are digits 0-9
//    {temp_message_value = ((message[message_value_index_1] - '0')*10 + (message[message_value_index_1+1] - '0'));} // converts this into a digit
//  else{success = false; Serial.println("read_message_value failed!");}
//  return success;
//}

//bool read_message_char (char message[])
//{
//  if(DEBUG){Serial.println("read_message_char");}
//  bool success = true;
//  if(isalpha(message[message_char_index]))    {temp_message_char = message[message_char_index];}
//  else                                        {success = false;Serial.println("read_message_char failed!");}
//  return success;
//}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////THINGS FROM GHETTO LIGHT DECODING////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//void read_on_face_testing_only(int face)
//{
//  while(read_ambient(face) < 8)
//  {delay(1);}
//  while(read_ambient(face) > 8)
//  {delay(1);}
//  Serial.println("MADE IT PAST PEAK 1");
//  while(read_ambient(face) < 8)
//  {delay(1);}
//  while(read_ambient(face) > 8)
//  {delay(1);}
//  Serial.println("MADE IT PAST PEAK 2");
//  while(read_ambient(face) < 8)
//  {delay(1);}
//  Serial.println("WAITING FOR FINAL PEAK TO END");
//  while(read_ambient(face) > 8)
//  {delay(1);}
//  sample_10_ms(face);
//  delay(500);
//}

//int convert_int_to_binary_array(int number, int array_length)
//{
//  int j = 0;
//  for(int i = array_length-1; i >= 0 ; i--)
//  {
//  blink_pattern[i] = bitRead(number, j); j++;
//  }
//}
//
//int                       samples[90];
//int  fast_ambient_sampling_time__[90];
//int  fast_ambient_sampling_count_[90];
//int  light_high_threshold             = 28;
//int  is_message_good                  = 1;
//int  filtered_message_13_bit[]        = {0,0,0,0,0,0,0,0,0,0,0,0,0};
//bool filtered_message_13_bit_bool[]   = {0,0,0,0,0,0,0,0,0,0,0,0,0};
//int  incomingByte                     = 0;
//int  blink_pattern[9];

//                                       --beginning------  | 0/1  ID0   ID1   ID2   ID3   ID4    S1    S2   | F1    F2    F3   LED1   LED2   clear
//int message_to_blink[22] = {0,  1,  0,  1,  0,  1,  1,  0,     0,   0,    0,    0,    0,    0,     1,    0,     0,    0,    0,    2,     3,   0};
//                         [0] [1] [2] [3] [4] [5] [6] [7]  | [8]  [9]   [10]  [11]  [12]  [13]   [14]  [15] | [16]  [17]  [18]  [19]   [20] [21]
// Received Message                                         | [0]  [1]   [2]   [3]   [4]   [5]    [6]   [7]  | [8]   [9]   [10]  [11]   [12]
//                                                            ||||||||||||||||||||||||||||||||||||||||||     
//    Cube ID                                     ----------------------------      
//    
// Sampling 8ms     | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | 
// Message type:
//                  High      Low      bit_0          bit_1      bit_2      bit_3      bit_4      bit_5      bit_6      bit_7     
// Sending Message [100 ms] [48 ms]    [48 ms]        [48 ms]    [48 ms]    [48 ms]    [48 ms]    [48 ms]    [48 ms]    [48 ms]    
//                                    Message Type    
//                                    1 = location    CubeID_1   CubeID_2   CubeID_3   CubeID_4   CubeID_5   Parity 1   Parity 2    
//                                    0 = command     

//
//void send_location_message_test(int face)
//{
//  message_to_blink[8] = 1;
//  bool face_number_binary[3] = {0, 0, 0}; //// I KNOW THE FOLLOWING IS GHETTO AS F***, Apologies to any real programmers who happen to read this
//  //                           [0][1][2]
//  if     (face == 1) {face_number_binary[2] = 1;}                           // [001]
//  else if(face == 2) {face_number_binary[1] = 1;}                           // [010]
//  else if(face == 3) {face_number_binary[1] = 1;face_number_binary[2] = 1;} // [011]
//  else if(face == 4) {face_number_binary[0] = 1;}                           // [100]
//  else if(face == 5) {face_number_binary[0] = 1;face_number_binary[2] = 1;} // [101]
//  else if(face == 6) {face_number_binary[0] = 1;face_number_binary[1] = 1;} // [110]
//  for(int i = 9;  i < 14; i++) {message_to_blink[i] = five_bit_id_array[i-9];}
//  for(int i = 16; i < 19; i++) {message_to_blink[i] = face_number_binary[i-16];}
//  //void blink_array(int bit_array[], int array_length, int face, int blink_period)
//  blink_array(message_to_blink, 22, face, 50);
//  Serial.println("stillalive");
//  delay(500);
//  //Serial.println("stillalive");
//}



//void sample_10_ms(int face)
//{
//  int num_samples       = 13;
//  int begin_time        = millis();
//  int  sampling_period  = 9;
//  int  sample_counter   = 0;
//  long sample_timer     = begin_time + sampling_period;
//  
//  int received_data_bit = 0;
//  int received_id       = 0;
//  int received_face     = 0;
//  int received_command  = 0;
//  
//  while(sample_counter < (num_samples*5+15))
//  { 
//  if(millis() > sample_timer)
//     {
//       sample_timer = millis() + sampling_period;
//       fast_ambient_sampling_time__[sample_counter] = millis()-begin_time;
//       sample_counter++; // Increment a counter       
//       samples[sample_counter] =  read_ambient(face);
//       fast_ambient_sampling_count_[sample_counter] = sample_counter;
//     }  
//  }
//for(int i = 0; i < num_samples; i++)
//{
//filtered_message_13_bit[i] = samples[((i*5)+4)] + samples[((i*5)+5)] + samples[((i*5)+6)] + samples[((i*5)+7)] + samples[((i*5)+8)];
//}
//
//for(int i = 0; i < num_samples; i++)
//{
//if      (filtered_message_13_bit[i] > light_high_threshold*3) {filtered_message_13_bit_bool[i] = 1;}
//else if (filtered_message_13_bit[i] < light_high_threshold*2) {filtered_message_13_bit_bool[i] = 0;}
//else                                                          {is_message_good = -1;}
//}
//for(int i = 0; i < (num_samples*5+15); i++)
//{
//  Serial.print(samples[i]);
//  Serial.print(" Clock = ");
//  Serial.print(fast_ambient_sampling_time__[i]);
//  Serial.print(" Count = ");
//  Serial.println(fast_ambient_sampling_count_[i]);
//  if((i+2)%5 == 0){Serial.print("-----------"); Serial.println((i+2)%5);}
//}
//Serial.println("THIS IS THE ACCUMULATED MASSAGE!!!");
//for(int i = 0; i < num_samples; i++)
//{
//   Serial.println(filtered_message_13_bit[i]);
//}
//Serial.println("ENDING - ACCUMULATED MASSAGE!!!");
//
//if(filtered_message_13_bit_bool[0] == 1) {Serial.println("LOOKING FOR NEIGHBORS");}
//if(filtered_message_13_bit_bool[6] == 1) {Serial.println("WOoooOOOO!!!!!");}
//for(int i = 0; i < 5; i++) {five_bit_id_array_received[i] = filtered_message_13_bit_bool[i+1];}
//five_bit_id_int_received         = bitArrayToInt32(five_bit_id_array_received, 5);
//
//for(int i = 0; i < 3; i++) {received_face_number_bits[i] = filtered_message_13_bit_bool[i+8];}
//received_face_number             = bitArrayToInt32(received_face_number_bits, 3);
//
//for(int i = 0; i < 2; i++) {received_LED_number_bits[i] = filtered_message_13_bit_bool[i+11];}
//received_LED_number              = bitArrayToInt32(received_LED_number_bits, 2);
//
//Serial.print("And the NEIGHBORS ID IS.... ");
//Serial.println(five_bit_id_int_received);
//Serial.print("And the NEIGHBORS'S FACE NUMBER .... ");
//Serial.println(received_face_number);
//Serial.print("And the NEIGHBORS LED NUMBER IS .... ");
//Serial.println(received_LED_number);
//}



//void blink_array(int bit_array[], int array_length, int face, int blink_period)
//{
//  for(int i = 0; i < array_length; i++)
//  {
//   if      (bit_array[i] == 1) {IRLED(face,1,1,1,1);}
//   else if (bit_array[i] == 2) {IRLED(face,0,0,1,1);}
//   else if (bit_array[i] == 3) {IRLED(face,0,1,0,1);}
//   else             {IRLED(face,0,0,0,0);}
//   delay(blink_period);
//  }
//  IRLED(face,0,0,0,0);
//}


//void accept_data_from_serial_testing()
//{
//if (Serial.available() > 0) 
//    {
//                // read the incoming byte:
//                incomingByte = Serial.parseInt();
//
//                // say what you got:
//                Serial.print("I received: ");
//                Serial.println(incomingByte);
//                
//                convert_int_to_binary_array(incomingByte,9);
//                for(int i = 6; i < 15; i++) {message_to_blink[i] = blink_pattern[i-6];}
//                //blink_boolean_array(bool bit_array[]     ,int array_length, int face, int blink_period);
//                blink_array  (message_to_blink,15              , 3       , 50              );
//      }
//}

//void stupid_test(int face)
//{
//  char testingbs[4];
//  for(int i = 0; i < 3; i++)
//  {
//  testingbs[i] = bitRead(face, i);
//  Serial.print(bitRead(face, i));
//  }
//  Serial.print("is the bit for face: ");Serial.println( face);
//}
// for(int i = 1; i < 7; i++){stupid_test(i);}
// 
//int  five_bit_id_array[5]             = {0,0,0,0,0};
//int  five_bit_id_int                  = 0;
//
//bool five_bit_id_array_received[5]    = {0,0,0,0,0};
//int  five_bit_id_int_received         = 0;
//
//bool received_face_number_bits[3]    = {0,0,0};
//int  received_face_number             = 0;
//
//bool received_LED_number_bits[2]    = {0,0};
//int  received_LED_number            = 0;

//   simple_loop_counter++;
//   write_ID(byte(0x15), byte(simple_loop_counter));
//   Serial.println("---------------------------------------");
//   Serial.print(read_register(1,byte(0x12))); Serial.println("  data in 0x12");
//   Serial.print(read_register(1,byte(0x13))); Serial.println("  data in 0x13");
//   Serial.print(read_register(1,byte(0x14))); Serial.println("  data in 0x14");
//   Serial.print(read_register(1,byte(0x15))); Serial.println("  data in 0x15");
//   Serial.print(read_register(1,byte(0x16))); Serial.println("  data in 0x16");
//   Serial.print(read_register(1,byte(0x17))); Serial.println("  data in 0x17");
//   Serial.print(read_register(1,byte(0x18))); Serial.println("  data in 0x18");
//   Serial.print(read_register(1,byte(0x19))); Serial.println("  data in 0x19");
//   Serial.println("---------------------------------------");
//   Serial.print(read_register(1,byte(0x09))); Serial.println("  ASSEMBLED DATA");
//   Serial.println("---------------------------------------");
//   Serial.print(read_ambient_new(1));Serial.println(" IS THE AMBIENT READING OLD");
//   //Serial.println("stillalive");
   //for(int i = 1; i < 7; i++){send_location_message_test(i);}
   //Serial.print(read_ambient_new(1));Serial.println(" IS THE AMBIENT READING ");
   //Serial.print(read_ambient(1));Serial.println(" IS THE AMBIENT READING OLD");
   //Serial.print(read_register(1));Serial.println(" IS THE REGISTER");
   //accept_data_from_serial_testing();
   //read_on_face_testing_only(1);
   //   which_demo = "fast_blink";
//Serial.println(five_bit_id_int);
//for(int i = 0; i < 5; i++){Serial.print(five_bit_id_array[i]);}
//delay(500);

//bool write_ID(byte register_address, byte databyte)//(int face)
//{
//
//  if(DEBUG){Serial.println("Beginning FBRXEN");}
//  digitalWrite(Switch, HIGH); // Takecontrol of I2C BUS
//  delay(3);
//  //for (int x = 1);//; x< 7 ; x++) {
//  Wire.beginTransmission(1);
//  Wire.write(register_address);
//  Wire.write(databyte);
//  Wire.endTransmission(); 
//
//}

//unsigned int read_register(int face, byte i2c_register)//(int face)
//{
//  unsigned int reading = 0;
//  digitalWrite(Switch, HIGH);
//  if(face > 6 || face < 1) {return(-1);}
//  Wire.beginTransmission(face);
//  Wire.write(i2c_register); // this is the register
//  Wire.endTransmission();
//  Wire.requestFrom(face, 1);
//  if (Wire.available()) //ambientLight  = twiBuf[0] << 2;
//  {
//    reading =  Wire.read();     //  ambientLight |= twiBuf[1] >> 6;
//    return reading;
//  }
//  else{return(-1);}
//}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//bool fbtx_send(int face, bool flash) {
//  if (face < 1 || face > 6){return(false);}
//  digitalWrite(Switch, HIGH);
//  byte i2c1 = 0x34; // FB_REGISTER_ADDR_TX_MSG_CONTROL
//  byte i2c2 = flash ? 0x02 : 0x00;
//  i2c2 |= 0x01;
//  Wire.beginTransmission(face);
//  Wire.write(byte(i2c1));
//  Wire.write(byte(i2c2));
//  Wire.endTransmission();
//  return true;
//}

//bool fbtx_que_message(int face, int numBytes, char message[]) {
//    digitalWrite(Switch, HIGH);
//    char twiBuf[128];
//    if (face > 6 || face < 1) {return false;}
//    twiBuf[0]  = 0x35; //FB_REGISTER_ADDR_TX_MSG_BUF         0x35
//    twiBuf[1] = twiBuf[2] = twiBuf[3] = 0xB7; // pad message as there is a tendency to drop the first few characters
//    for(int i = 0; i < numBytes; i++){twiBuf[i+4] = message[i];}
//    Wire.beginTransmission(face);
//    for(int i = 0; i < (4 + numBytes); i++)
//    {
//     Wire.write(byte(twiBuf[i]));
//    }
//    Wire.endTransmission();
//    return(true);
//}


//<datatype> array [DIM_0_SIZE] [DIM_1_SIZE] = {
//  //as many vals as dim1
// {val,val,val},
// {val,val,val}//as many rows as dim0
//};

//Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();



/*
 * 
 * 
 * 1. Setup 
 *    a. Wifi Setup
 *    b. Ros Setup
 *    c. Sensors Setup
 * 
 * 2. Main Loop State Machine ... Does Ros things...
 *    States:
 *    1. Chilling - Publishes regular updates of sensors values// accepts short commands
 *          Listens to General ROS topic, accepts commands based on general or on cube specific topic
 *    
 *    2. Driving a car*
 *    
 *    3. Aggregate - Does not regularly update sensors... Internally evaluates sensor readings to move toward light
 *    
 *       Begin... Get 6 light readings... Check Current orientation (1234, 1536, 2546)
 *       Determine if free to move with fn::Wiggle (20% of the time do this 2x or stronger)
 *       Find Face with Highest value, above a threshold. 
 *            Determine if flywheel is currently aligned with this.
 *                If so - slowly roll fw or bkwards.
 *                if not - random num generator ? Wiggle : Jump
 *                
 *    4. Climb?
 *    5. Sleep?
 *    6. Reset?
 *    7. Demo? - hard programmed demo routine
 * 
 * 
 * 
 * 
 * 3. Short Commands
 *    1. Turn on/off IR LED 1....24
 *    2. Turn on RGB led 1-8 to R or G or B
 *    3. All RGB specific color
 *    4. Any IA command
 *    
 *    
 * 4. Longer Commands
 * 
 *    3. Wiggle_test
 *        Check gravity.
 *        Begin recording array of GYRO values
 *        Do a short, sharp Move
 *        IF (CP_state) is alligned with Ground
 *        
 *        ELSE
 *    
 *    
 *    
 *    4. IA_MOVE
 *        Updates internal 6 light readings, (1234, 1536, 2546) and 3x Gravity Readings.
 *        Determine expected sucessfull gravity readings from a Lookup Table.
 *        Attempt to move using (standard values*Specific Cube Calibration number).
 *            Checks to see if successful, unsucessfull, or WTF!! (look at gravity, maybe array of gyro readings)
 *            If successful - send sucess message to (ROS + Flash LED green)
 *            (If unsucessfull and fail counter <3) - Wait 1000ms, then try IA again with 1.(fail_Counter)*Current value 1.(fail_Counter)*Speed value
 *                  Increment fail_Counter.
 *            if WTF - check wiggle.
 *                 If attached, send message.
 *                 If Free - send message
 *                 <Break>
 *            If fail counter >3
 *                Turn LED's Red
 *                Send Message
 *                <break>
 * 
 *     5. CP(1234 (a) or 1536 (b), or 2546 (c))
 *         Does Wiggle Test (Connected or not?)
 *         if (Connected)
 *            Check Magnet readings...
 *            SMA retract(3000)
 *            Begin recording array of Magnet Values... Also Acceleromater... and Gyro Values
 *                IF (1234 to 1536...) Possible Transistions: (a)->(b), (b)->(c), (c)->(a), (b)->(a), (c)->(b), (a)->(c)
 *                      While (Not disloged) -- Accerate (or ebrake...) eneough to disloge with increasing power
 *                 Check magnet state... IF correct... way to go! 
 *                 <break>
 *                 
 *                 If not correct, either in (a), (b), (c)
 *                 If not check array readings to try to determine if in region between (a-b) (b-c) or (c-a)
 *                  
 *                 Choose reasonable value for FLYWHEEL
 *                 Loop back, and try again, increase SMA retract time
 *                 
 *         if (Not Connected)
 *                Same as above, but with lower power values.
 * 
 * 
 */



 // Notes
//const char WiFiSSID[] = "TP-LINK_9B88A0"; // Sebastian's router
//const char WiFiPSK[] = "279B88A0";// Sebastian's router

// Format for sensor values Message
// U_int16 array
////////////////////////////////////
//
//
// Cube ID 0xVVV,                             State[0]
// Face1_light,                               State[1]
// Face2_light,                               State[2]
// Face3_light,                               State[3] 
// Face4_light,                               State[4]
// Face5_light,                               State[5] 
// Face6_light,                               State[6]
// Magnet sensor Reading                      State[7]                    (1234, 1536, 2546 or 0000) with 0000 meaning undetermined or error
// Grav_face1_x,                              State[8]
// Grav_face1_y,                              State[9]
// Grav_face1_z,                              State[10]
// Gyro_face1_x,                              State[11]
// Gyro_face1_y,                              State[12]
// Gyro_face1_z,                              State[13]
// TEMP_ACCEL   ,                             State[14] 
// ALS_parasite,                              State[15]        
// Altitude_parasite,                         State[16]                     Value in mm, zeroed on initial setup
//
//
//    while (Serial.available() > 0) // while there are things in the serial buffer...
//  {
//      char c = Serial.read();
//      if (c != '\n') 
//    {
//        ser_str += String(c);
//    } else 
//    {
//        serial_msg.data = ser_str.c_str();
//        pub_ser.publish(&serial_msg);
//        ser_str = "";
//    }
//  }


//void special_blink(int face) // each time this is called, it will perform the next action.
//// If the function is called every 50ms it will take 1 second to complete
////   else if(blink_counter == 13){for(int i = 1; i < 7; i++){IRLED(face,0,0,0,1);}
//// Pulse should look like this:
//// _|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_
//// A A A A A - - - 1 1 1 2 2 2 3 3 3 4 4 4
//{
//  if(blink_counter == 22){blink_counter = 0;} // Starts sequential counter 0 causes it to not be stop running...
//  else if(blink_counter == 0){return;} // this means that the counter has already run once...
//  else if(blink_counter == 1){IRLED(face,1,1,1,1);}
//  else if(blink_counter == 2){delay(1);}
//  else if(blink_counter == 3){delay(1);}
//  else if(blink_counter == 4){delay(1);}
//  else if(blink_counter == 5){delay(1);}
//  else if(blink_counter == 6){IRLED_OFF();}
//  else if(blink_counter == 7){delay(1);} // Delay 1 time step
//  else if(blink_counter == 8){delay(1);} // Delay 1 time step
//  else if(blink_counter == 9){IRLED(face,1,0,0,0);} // Turn on IRLED # 1 for 2 time steps
//  else if(blink_counter == 10){delay(1);} // Delay 1 time step
//  else if(blink_counter == 11){delay(1);} // Delay 1 time step
//  else if(blink_counter == 12){IRLED(face,0,1,0,0);} // Turn on IRLED # 2 for 2 time steps
//  else if(blink_counter == 13){delay(1);} // Delay 1 time step
//  else if(blink_counter == 14){delay(1);} // Delay 1 time step
//  else if(blink_counter == 15){IRLED(face,0,0,1,0);} // Turn on IRLED # 3 for 2 time steps
//  else if(blink_counter == 16){delay(1);} // Delay 1 time step
//  else if(blink_counter == 17){delay(1);} // Delay 1 time step
//  else if(blink_counter == 18){IRLED(face,0,0,0,1);} // Turn on IRLED # 4 for 2 time steps
//  else if(blink_counter == 19){delay(1);} // Delay 1 time step
//  else if(blink_counter == 20){delay(1);} // Delay 1 time step
//  else if(blink_counter == 21){IRLED_OFF();}
//  blink_counter++;
//}
//
//void find_connections()
//{
//  // Step 0: Update Zero faces... turn off all IR LED's and regular LED's... 
//  // long funny_bs = millis();
//    // ... Function...
//    // ...............
//    // Serial.print("This functiontook: ");
//    // Serial.println(millis() - funny_bs);
//  update_zero_faces();
//  //for(int i = 1; i <7; i++) {if(zero_faces[i] == 1){Serial.print("ZERO FACE: "); Serial.println(zero_faces[i]);}}
//  rgboff();
//  IRLED_OFF();
//  //for(int i = 1; i <7; i++){Serial.print("Face " + String(i) + " status: "); Serial.println(connected_faces[i]);}
//  for(int face = 0; face < 7; face++)
//  { 
//    // Step 1: Start for loop over faces to TRANSMIT... Update ROS things...
//    if(zero_faces[face] == 1)
//      {
//        if(is_ros_on){nh.spinOnce(); publishAwake();}
//        delay(random(10,80));
//        int ir_val[7][7] = {{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0}}; // Face[1][prev.1.prev.2.3.4.5
//        // Step 2: Start 60ms Loop:
//        for(int i = 0; i < 13; i++)
//          {
//   // Blink correct part of message...
//            if(i%2 == 1){face_rgb(face, 1, 0, 1, 0); IRLED(face,1,1,1,1);} else {rgboff(); IRLED_OFF();} // Blinks IR LED's on "face" face
//   // Update ir_values_(faces on zero list)
//            delay(5);
//            for(int j = 1; j < 7; j++)
//            {
//            int readinn = read_ambient(j);
//            //if(zero_faces[j] == 1){face_rgb(j, 1, 0, 0, 0);
//            if(readinn > 1){{ir_val[j][0] = 1;} if(j == face && i%2 == 1){ir_val[j][0] = -1;}}
//            else if(readinn <= 1) {ir_val[j][0] = 0;}
//            else {ir_val[j][0] = -1;}
//            
//            } // Updates all faces item 0
//   // For all faces... Check to see if we see THE pattern... 
//            
//            for(int i = 1; i < 7; i++)
//            {if(ir_val[i][0] == 0 && ir_val[i][1] == 1 && ir_val[i][2] == 0 && ir_val[i][3] == 1 && ir_val[i][4] == 0 && ir_val[i][5] == 1)
//              {connected_faces[i] += 1;}}
//   // For faces on connection_list ... also check to see if they are all high... if so remove from list...
//             for(int i = 1; i < 7; i++)
//            {if(ir_val[i][0] == 1 && ir_val[i][1] == 1 && ir_val[i][2] == 1 && ir_val[i][3] == 1 && ir_val[i][4] == 1 && ir_val[i][5] == 1)
//              {connected_faces[i] = 0;}}
//   // Shift all values one to the left...
//            for(int i = 1; i < 7; i++)
//            {
//              ir_val[i][5] = ir_val[i][4];
//              ir_val[i][4] = ir_val[i][3];
//              ir_val[i][3] = ir_val[i][2];
//              ir_val[i][2] = ir_val[i][1];
//              ir_val[i][1] = ir_val[i][0];
//            }
//            delay(50);
//         }
//      }
//   }
//}
//
//void display_connected()
//{
//  for(int i = 1; i<7; i++)
//  {if(connected_faces[i] >= 1){face_rgb(i,0,1,0,1); delay(150); rgboff();}}
//}
//void update_zero_faces()
//{
//  int previous_values[7] = {0,0,0,0,0,0,0};
//  for(int i = 1; i <7; i++)
//  {
//  int light_value_ = read_ambient(i);
//  if(light_value_ < 1 && light_value_ != -1){zero_faces[i] = 1; previous_values[i] = light_value_;}
//  else{zero_faces[i] = 0;}
//  if(opposite_face(which_face_is_up(12000)) == i)
//  {zero_faces[i] = 0;}
//  }
//}

 // each time this is called, it will perform the next action.
// If the function is called every 50ms it will take 1 second to complete
//   else if(blink_counter == 13){for(int i = 1; i < 7; i++){IRLED(face,0,0,0,1);}
//
////if(ambient_values[face][0] == 0 && ambient_values[face][3] == 1 && ambient_values[face][4] == 1 && ambient_values[face][5] == 1 && ambient_values[face][8] == 0)
//        {
//          //Serial.print("FOUND A PULSE! on face number: ");Serial.println(face);
//          message_since_seeing_pulse[face]    = _50ms_loop_counter;
//          am_i_waiting_for_a_high_value[face] = 1;
//          //loop_counter
////          while(read_ambient(face) < high_ambient_threshold && (millis() - counter_begin) < 700)
////            {
////              delay(10);
////            }
//          //face_neighbors_angles[face] = millis() - counter_begin;
//          //message_blink_counter[face] = 1; // Send a message back as soon as possible
//          //connection_wait_time = millis() - counter_begin;
////          Serial.print("Time before second pulse = ");
////          Serial.println(connection_wait_time);
//        }



