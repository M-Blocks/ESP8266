// int  look_for_signature_in_ambient(int face)
// void fill_out_temp_message(char define_message_TYPE, int define_message_value, char define_message_char)
// bool process_message(int face)
// bool process_messages_all_faces()
// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// index                                            [0]     [1]     [2]      [3]    [4]    [5]     [6]    [7]
//                                                  type   type     type  [value] [value]  char    type   type
// char location_message[STANDARD_MESSAGE_LENGTH] = {'l',  'l',    'l',      'l'    '1',    '9',    'a',  'l'}; // do not pass this message on...

int look_for_signature_in_ambient(int face)
{
  
}

void fill_out_temp_message(char define_message_TYPE, int define_message_value, char define_message_char)
{
  char buf [3];
  sprintf(buf, "%02i",define_message_value);
  int digit1 = 0;
  int digit2 = 0;
  digit1 = buf [0];
  digit2 = buf [1];
  
   temp_active_message[0] = define_message_TYPE;
   temp_active_message[1] = define_message_TYPE;
   temp_active_message[2] = char(digit1);
   temp_active_message[3] = char(digit2);
   temp_active_message[4] = define_message_char;
   temp_active_message[5] = define_message_TYPE;
   temp_active_message[6] = define_message_TYPE;

}

bool process_message(int face)
{
  int fb_message_size_temp =  fb_message_size(face);
  if (fb_message_size_temp >= MESSAGE_THRESHOLD)
     {
      fb_receiveFromRxBuffer(face, fb_message_size_temp);  /// contents are now in temp_message[] - an array
      if(fb_message_size_temp == MESSAGE_THRESHOLD && isupper(temp_message[1]))     {temp_message[5] = temp_message[1];} //  Pad message if it is too short...
      if(fb_message_size_temp == (MESSAGE_THRESHOLD+1) && isupper(temp_message[1])) {temp_message[6] = temp_message[1];} //  Pad message if it is too short...
      if(DEBUG){for(int i = 0; i < 15; i++){Serial.print(" "); Serial.print(temp_message[i]);}}
      /// temp_message_type       - char
      /// temp_message_id         - int            these get updated according to the below
      /// temp_message_char      - char
        if(read_message_information_and_align(temp_message))
            {
              if(DEBUG)
              {
                Serial.print("PROCESSED A MESSAGE!!!!: "); 
                Serial.print("  type: ");Serial.print(temp_message_type);
                Serial.print("  value: ");Serial.print(temp_message_value);
                Serial.print("  char: ");Serial.print(temp_message_char);
              }
            
                     if(temp_message_type == 'S')  {process_special_message (face, temp_message_value, temp_message_char);}
                else if(temp_message_type == 'L')  {process_location_message(face, temp_message_value, temp_message_char);}         
                else if(temp_message_type == 'H')  {process_hey_message     (face, temp_message_value, temp_message_char);}
                else if(temp_message_type == 'C')  {process_color_message   (face, temp_message_value, temp_message_char);}
                else if(temp_message_type == 'M')  {process_motion_message  (face, temp_message_value, temp_message_char);}
            }
        else // this is if the message type is not accaptable... do nothing
          {
            delay(1);
            Serial.println(" I JUST FOUND A MESSAGE THAT DIDNT MEET CRITERIA =(");
            for(int i = 0; i < MESSAGE_THRESHOLD; i++){Serial.print(" | ");Serial.print(temp_message[i]);}
            Serial.print(" : is the Message text is: ");
          }
        
      }    
   else if(fb_message_size_temp > 0 && ir_face_buffer_previous[face] > 0) // this means buffer is still partially full, after one check...
      {
          Serial.println("checked partial message");
          
          fb_receiveFromRxBuffer(face, fb_message_size_temp);
//          for(int i = 0; i < fb_message_size_temp; i ++) 
//            {
//              temp_message_value = 53;
//                   if(temp_message[i] == 'R'){temp_message_char = 'r'; process_color_message (face, temp_message_value, temp_message_char);}
//              else if(temp_message[i] == 'G'){temp_message_char = 'g'; process_color_message (face, temp_message_value, temp_message_char);}
//              else if(temp_message[i] == 'B'){temp_message_char = 'b'; process_color_message (face, temp_message_value, temp_message_char);}
//              else if(temp_message[i] == 'Y'){temp_message_char = 'y'; process_color_message (face, temp_message_value, temp_message_char);}
//              else if(temp_message[i] == 'P'){temp_message_char = 'p'; process_color_message (face, temp_message_value, temp_message_char);}
//              else if(temp_message[i] == 'W'){temp_message_char = 'w'; process_color_message (face, temp_message_value, temp_message_char);}
//              else if(temp_message[i] == 'T'){temp_message_char = 't'; process_color_message (face, temp_message_value, temp_message_char);}
//            }
          fbrxflush(face);
          fb_message_size_temp = 0;
      }
   else { }  // this means that fb_message_size(face) is less than or equal to zero
   ir_face_buffer_previous[face] = fb_message_size_temp;
   erase_received_message();  
}
              
bool process_messages_all_faces()
{
  for(int face = 1; face < ACTIVE_FACES; face++)
  {
    if(i_just_sent_an_ir_message[face] > 0) // this is designed to not check for messages immeaditly after sending them...
    {
      i_just_sent_an_ir_message[face]--; 
      if(i_just_sent_an_ir_message[face] == 1) {fbrxflush(face);}
    }
    else {process_message(face);}
  }
}
//////////////////////////////////////
//////////////////////////////////////
//////////////////////////////////////
void prepare_standard_messages()
{
 edit_message_value(location_message, cube_id);
 edit_message_value(special_message,  cube_id);
}

// index                                                                  [0]     [1]     [2]      [3]    [4]    [5]      [6]      [7]     [8]   [9]
//                                                 type   type    type    type  [value] [value]  char    type   type
//char location_message[STANDARD_MESSAGE_LENGTH]   {M_L,  M_L,    M_L,    M_L,    '1',    '9',    'a',    M_L,   M_L,   M_L};

bool read_message_information_and_align  (char message[])
{
  if(DEBUG){Serial.println("read_message_information_and_align");}
  bool success = false;
  int max_tries = 5;
  int tries = 0;
  while(tries < max_tries)
  {
      if(message[tries] == message[4+tries] && isupper(message[tries]) && isdigit(message[1+tries]) && isdigit(message[2+tries]) && islower(message[3+tries]))
        {
            success = true;
            temp_message_value = ((message[1+tries] - '0')*10 + (message[2+tries] - '0'));
            temp_message_char = message[3+tries];
                 if(message[tries] == 'S')  {temp_message_type = 'S';}
            else if(message[tries] == 'L')  {temp_message_type = 'L';}
            else if(message[tries] == 'H')  {temp_message_type = 'H';}
            else if(message[tries] == 'C')  {temp_message_type = 'C';}
            else if(message[tries] == 'M')  {temp_message_type = 'M';}
            else                              {temp_message_type = 'X'; success = false; Serial.println("read_message_type failed!");}
            
            break;
        }
      tries++; if(DEBUG){Serial.println("Incremented tries...");}
  }
   Serial.print("TRIES: "); Serial.println(tries); 
   return success;
}

//////////////////////////////////////
//////////////////////////////////////
//////////////////////////////////////
  // index                      [0]    [1]     [2]      [3]     [4]     [5]     [6]    [7]
//                              type   type    type    [id1]   [id2]    char    type   type
//char location_message[8] =   {'l',  'l',    'l',      '1',    '9',    'a',    'l',   'l'}; // do not pass this message on...
//char hey_message[8] =        {'h',  'h',    'h',      '1',    '4',    'h',    'h',   'l'}; // pass these on by default

bool edit_message_char (char message[], char new_char)
{  
  bool success = true;
  message[message_info_begin_index+char_position_offset] = new_char;
  return success;
}

bool edit_message_value    (char message[], int new_value)
{
  char buf [3];
  sprintf(buf, "%02i", new_value);
  int digit1 = 0;
  int digit2 = 0;
  digit1 = buf [0];
  digit2 = buf [1];
  message[message_info_begin_index]   = char(digit1);
  message[message_info_begin_index+1] = char(digit2);
  bool success = true;
  return success;
}

void erase_received_message()
{
  for(int i = 0; i < ARRAY_LENGTH_LONG; i++){temp_message[i] = '_';}
}


int bitArrayToInt32(bool arr[], int count_)
{
    int ret = 0;
    int tmp;
    for (int i = 0; i < count_; i++) {
        tmp = arr[i];
        ret |= tmp << (count_ - i - 1);
    }
    return ret;
}

// Pulse should look like this:
// _|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_

//               [16] [15] [14] [13] [12] [11] [10] [9]  [8]  [7]  [6]  [5]  [4]  [3]   [2]  [1] 
//               *              *             *              *              *                        
//               _    _    A    A   A    _    _    _   b1    b2   b3   c1   c2    c2     _    [send message...]
// led0 - 0  0   0    0    1    1   1    0    0    0    0    0    0    0    0     0      0
// led1 - 0  1   0    0    1    1   1    0    0    0    0    0    0    1    1     1      0
// led2 - 1  0   0    0    1    1   1    0    0    0    1    1    1    0    0     0      0
// led3 - 1  1   0    0    1    1   1    0    0    0    1    1    1    1    1     1      0

void general_counter(int counter_array[])
{
   for(int face = 1; face < ACTIVE_FACES; face++)
  {
    if(counter_array[face] < 1) {} // we are done... wait for this number to get bumped up again
    else
    {     
           if(counter_array[face] == 5 )  {}
      else if(counter_array[face] == 2 )  {}
      counter_array[face]--;
    }
  }
}

void signature_blink()
{
   for(int face = 1; face < ACTIVE_FACES; face++)
  {
    if(signature_blink_array[face] < 1) {} // we are done... wait for this number to get bumped up again
    else
    {     
           if(signature_blink_array[face] == 4 )  {IRLED(face,1,1,1,1);}
      else if(signature_blink_array[face] == 1 )  {IRLED(face,0,0,0,0);}
      signature_blink_array[face]--;
    }
  }
}

void location_message_special_blink()
{
  for(int face = 1; face < ACTIVE_FACES; face++)
  {
    if(message_location_blink_counter[face] < 1) {} // we are done... wait for this number to get bumped up again
    else
    {
      if(message_location_blink_counter[face] == 1)
            {
              //location_message[index_of_face_number] = char(face_a); //location_message[index_of_face_number + 2] = char(face);
              //send_ir_message_char(face, location_message_long, long_message_length);
                char new_char_temp = 'a';
                     if(face == 1){new_char_temp = 'a';}
                else if(face == 2){new_char_temp = 'b';}
                else if(face == 3){new_char_temp = 'c';}
                else if(face == 4){new_char_temp = 'd';}
                else if(face == 5){new_char_temp = 'e';}
                else if(face == 6){new_char_temp = 'f';}                
                edit_message_char(location_message, new_char_temp);
                send_ir_message_char(face, location_message, STANDARD_MESSAGE_LENGTH);
                if(DEBUG){Serial.println("I JUST SENT A LOCATION MESSAGE");}
            }
      else if(message_location_blink_counter[face] == 14)  {IRLED(face,1,1,1,1);}
      else if(message_location_blink_counter[face] == 11)  {IRLED(face,0,0,0,0);}
      else if(message_location_blink_counter[face] == 8 )  {IRLED(face,0,0,1,1);}
      else if(message_location_blink_counter[face] == 5 )  {IRLED(face,0,1,0,1);}
      else if(message_location_blink_counter[face] == 2 )  {IRLED(face,0,0,0,0);}
      message_location_blink_counter[face]--;
    }
  }
}

//color_message, MESSAGE_THRESHOLD);
bool send_ir_message_on_faces(bool face1, bool face2, bool face3, bool face4, bool face5, bool face6, char message[])
{
  if(face1) {send_ir_message_char(1, message, MESSAGE_THRESHOLD);}
  if(face2) {send_ir_message_char(2, message, MESSAGE_THRESHOLD);}
  if(face3) {send_ir_message_char(3, message, MESSAGE_THRESHOLD);}
  if(face4) {send_ir_message_char(4, message, MESSAGE_THRESHOLD);}
  if(face5) {send_ir_message_char(5, message, MESSAGE_THRESHOLD);}
  if(face6) {send_ir_message_char(6, message, MESSAGE_THRESHOLD);}
}

bool send_ir_message_char(int face, char message[], int message_length)
{
  IRLED(face,0,0,0,0);
  delay(1);
  fbtxled(face,1,1,1,1);
  delay(1);
  fbtx_send_msg_char(face, message_length, message);
  i_just_sent_an_ir_message[face] = 3;
  return(true);
}
///////// SENDING
// fbtxcount <facenumber>
// fbtx <facenumber> <message>
//**** fbtxled <faceNumber> [<irLEDNumber1>] [<irLEDNumber2>] [<irLEDNumber3>] [<irLEDNumber3>]

/////// RECEIVING
//**** fbrx <faceNumber> <byteCount>
//**** fbrxflush <faceNumber>
//**** fbrxcount <facenumber>

bool fbtx_send_msg_char(int face, int numBytes, char message[]) 
{
    char twiBuf[30];
    if (face > 6 || face < 1) {return false;}
    twiBuf[0] = 0x30; //FB_REGISTER_ADDR_TX_BUF;
    for(int i = 0; i < numBytes; i++){twiBuf[i+1] = message[i];}
    
    Wire.beginTransmission(face);
    for(int i = 0; i < (1 + numBytes); i++)
    {
      Wire.write(byte(twiBuf[i]));
    }
    Wire.endTransmission();
    return(true);
}

int fb_message_size(int face)//, uint8_t *bytes) 
{
    int number = 0;
    if ((face < 1) || (face > 6)) {return -1;}        
    Wire.beginTransmission(face);// 
    Wire.write(byte(0x41)); //FB_REGISTER_ADDR_RX_CONSUMED_COUNT 
    Wire.endTransmission();
    
    Wire.requestFrom(face, 1);
    if (Wire.available() > 0) {number = Wire.read(); Wire.endTransmission();} // read the number stored in faceboard
    if(number > 0){Serial.print("fb_message_size_buffer: ");Serial.print(number);Serial.print(" on face: ");Serial.println(face);}
    return number;
    
}

bool fb_receiveFromRxBuffer(int face, int numBytes)//, uint8_t *bytes) 
{
    if ((face < 1) || (face > 6)) {return -1;}        
    Wire.beginTransmission(face);// 
    Wire.write(byte(0x40)); // int i2c1 = 0x40; //FB_REGISTER_ADDR_RX_BUF  0x40 
    Wire.endTransmission();
    
    Wire.requestFrom(face, numBytes);
    if (Wire.available() >= numBytes) //ambientLight  = twiBuf[0] << 2;
    {
      for(int i = 0; i < numBytes; i++)
      {
      temp_message[i] = Wire.read(); 
      }    
      if(DEBUG){for(int i = 0; i < numBytes; i++) {Serial.print(temp_message[i]);} Serial.println(" ");}
    }
    return true;
}


bool fbrxflush(int face)
{
  if(DEBUG){Serial.print("Flushing buffer on face: ");Serial.println(face);}
  if (face < 1 || face > 6){return(false);}
  digitalWrite(Switch, HIGH);
  int i2c1 = 0x42; // FB_REGISTER_ADDR_RX_FLUSH  
  int i2c2 = 0x01;
  Wire.beginTransmission(face);
  Wire.write(byte(i2c1));
  Wire.write(byte(i2c2));
  Wire.endTransmission();
  return(true);
}

bool fbtxled(int face, bool led1, bool led2, bool led3, bool led4)
{
  if (face < 1){return(false);}  
  int i2c1 = 0x33; // FB_REGISTER_ADDR_TX_LED_SELECT
  int i2c2 = 0x00;
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


void schedule_message_send()
{
  for(int face = 1; face < ACTIVE_FACES; face++)
  {
    if(message_general_counter[face] > 0) 
    {
      send_ir_message_char(face, temp_active_message, MESSAGE_THRESHOLD);
      message_general_counter[face]--;
    } // we are done... wait for this number to get bumped up again
    else{}// we are done... wait for this number to get bumped up again
  }
}
//
//bool fb_getIRTxLEDs(int face, bool led1, bool led2, bool led3, bool led4) 
//{
//    if ((face < 1) || (face > 6)) {return -1;}
//    uint8_t twiBuf[2];
//    bool success = true;
//    twiBuf[0] = 0x33; // FB_REGISTER_ADDR_TX_LED_SELECT   
//    Wire.beginTransmission(face);// 
//    Wire.write(byte(0x40)); // int i2c1 = 0x40; //FB_REGISTER_ADDR_RX_BUF            0x40 
//    Wire.endTransmission();
//    
//    Wire.requestFrom(face, 1);
//    if (numBytes <= Wire.available()) //ambientLight  = twiBuf[0] << 2;
//    {
//      for(int i = 0; i < numBytes; i++)
//      {
//      temp_message[i] = Wire.read(); 
//      }    
//      //for(int i = 0; i < numBytes; i++) { Serial.print(temp_message[i]); }
//    }
//    return true;
//
//    success &= twi_master_transfer((faceNum << 1), twiBuf, 1, true);
//    success &= twi_master_transfer((faceNum << 1) | TWI_READ_BIT, twiBuf, 1, true);
//
//    twi_master_deinit();
//
//    if (success) {
//  led1 = (twiBuf[0] & 0x01) ? true : false;
//  led2 = (twiBuf[0] & 0x02) ? true : false;
//  led3 = (twiBuf[0] & 0x04) ? true : false;
//  led4 = (twiBuf[0] & 0x08) ? true : false;
//    }
//
//    return success;
//}
