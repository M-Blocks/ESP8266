//////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// BEGIN MAIN LOOP///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

void loop()
{
  if(true)                   { things_to_do_as_fast_as_possible(); }
  if(millis() > quick_timer) { things_to_do_at_fast_rate_______(); }
  if(millis() > slow_timer)  { things_to_do_at_slow_rate_______(); }
}

//////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// FAST RATE ////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
void essentials_of_fast_things()
{
   x10xfast_loop_counter++; quick_timer = millis() + quick_timer_length; // Deal with updating loop timer
   shift_and_update_IMU();
   shift_and_update_ambient_sensors();
   shift_and_update_lights();
}

void  things_to_do_at_fast_rate_______()
{
  essentials_of_fast_things();
  //process_general_ros();
  check_for_action_slow();
  if(x10xfast_loop_counter % 3 == 0){process_messages_all_faces();}
  move_counter--; 
  light_tracking_delay_counter--;
  
       if(demo == "light_track" && ambient_values[face_that_is_up][0] > 3)       {light_track_update();}
  else if(demo == "show_brightest")                                              {display_brightest_face();}
}


//////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// SLOW RATE ////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
void essentials_of_slow_things()
{
  loop_counter++; 
  slow_timer = millis() + slow_timer_length;                            // Deal with updating loop timer
  if(DEBUG)                                                             {Serial.print("loop_counter = "); Serial.println(loop_counter);delay(1);}
  Serial.println("stillalive"); // general upkeep work...
  if(motion_sum_log[0] == 0 && motion_sum_log[3] == 0)                  {attempt_reset_i2c(); for(int i = 0; i< 3; i++){Serial.println("esprst"); delay(40);}}
  if(is_ros_on)                                                         {publishAwake();}  
  if(loop_counter % 19 == 0)                                            {fbrxen(true);}

}

void things_to_do_at_slow_rate_______()
{
  ///////////////////////////////////////////////General Upkeep///////////////////////////////////////////////
  essentials_of_slow_things();
  schedule_message_send(); // sends out the active_message on faces according to decrementing counter... once per second
  if(DEBUG && loop_counter % 5){Serial.print(cmd);Serial.println(" is the current cmd");}
  ///////////////////////////////////////////////Random Checking///////////////////////////////////////////////
  if(x10xfast_loop_counter > 30000)                                     {x10xfast_loop_counter = 1;}
  if(motion_sum_log[0] > 8000 && demo != "light_track")           {main_alt_choose_color_menu();} // This activates the menu system if we shake the module
  if(loop_counter % 5  == 0 && DEBUG)                                   {Serial.print("sum_of_motion = ");{Serial.println(motion_sum_log[0]);}}
  if(loop_counter % 2511 == 0)                                          {get_battery_voltage();}
  if(loop_counter % 10 == 0)                                            {which_plane_fast();}
  if(loop_counter % 11 == 0)                                            {which_face_is_forward(which_plane_fast());}
  if(loop_counter % 13 == 0 && is_ros_on)                               {publishstatus();}
  if(loop_counter % 27 == 0)                                            {if(get_vin() > 3400){turn_off_esp();}}
  if(loop_counter % 10 == 0 && is_ros_on)                               {Serial.println("About to publish Neighbors...");publish_neighbor_info();}
  if(loop_counter % (60*10) == 0)                                       {cmd = "sleep";}
  
  if((loop_counter) % 15 == 0)                                          {message_location_blink_counter[1] = 14;}
  if((loop_counter) % 15 == 0)                                          {send_ir_message_char(1, hey_message, MESSAGE_THRESHOLD);}
  if((loop_counter+2) % 15 == 0)                                        {send_ir_message_char(2, hey_message, MESSAGE_THRESHOLD);}
  if((loop_counter+4) % 15 == 0)                                        {send_ir_message_char(3, hey_message, MESSAGE_THRESHOLD);}
  if((loop_counter+6) % 15 == 0)                                        {send_ir_message_char(4, hey_message, MESSAGE_THRESHOLD);}
  if((loop_counter+8) % 15 == 0)                                        {send_ir_message_char(5, hey_message, MESSAGE_THRESHOLD);}
  if((loop_counter+10) % 15 == 0)                                       {send_ir_message_char(6, hey_message, MESSAGE_THRESHOLD);}
  
  for(int face = 1; face < ACTIVE_FACES; face++)
  {
    if(ambient_values[face][0] > ambient_high && ambient_values[face][1] > ambient_high)
    {
      if(DEBUG){Serial.println("Changing Color!");}
           edit_message_value(color_message, 55);
           if(face == 1){edit_message_char(color_message, 'p'); send_ir_message_on_faces(1,1,1,1,1,1,color_message);rgbp();}
      else if(face == 2){edit_message_char(color_message, 'g'); send_ir_message_on_faces(1,1,1,1,1,1,color_message);rgbg();}
      else if(face == 3){edit_message_char(color_message, 'b'); send_ir_message_on_faces(1,1,1,1,1,1,color_message);rgbb();}
      else if(face == 4){edit_message_char(color_message, 'y'); send_ir_message_on_faces(1,1,1,1,1,1,color_message);rgby();}
      else if(face == 5){edit_message_char(color_message, 'r'); send_ir_message_on_faces(1,1,1,1,1,1,color_message);rgbr();}
      else if(face == 6){edit_message_char(color_message, 'w'); send_ir_message_on_faces(1,1,1,1,1,1,color_message);rgbw();}
    }
  }
  
//  if((loop_counter)   % 11 == 0)                                          {send_ir_message_char(1, hey_message, MESSAGE_THRESHOLD);}
//  if((loop_counter+1) % 11 == 0)                                        {send_ir_message_char(2, hey_message, MESSAGE_THRESHOLD);}
//  if((loop_counter+2) % 11 == 0)                                        {send_ir_message_char(3, hey_message, MESSAGE_THRESHOLD);}
//  if((loop_counter+3) % 11 == 0)                                        {send_ir_message_char(4, hey_message, MESSAGE_THRESHOLD);}
//  if((loop_counter+4) % 11 == 0)                                        {send_ir_message_char(1, special_message, MESSAGE_THRESHOLD);}
//  if((loop_counter+5) % 11 == 0)                                        {send_ir_message_char(1, motion_message, MESSAGE_THRESHOLD);}
//  if((loop_counter+6) % 11 == 0)                                        {send_ir_message_char(1, color_message, MESSAGE_THRESHOLD);}
//  if((loop_counter+7) % 11 == 0)                                        {send_ir_message_char(1, color_message, MESSAGE_THRESHOLD);}

  ///////////////////////////////////////////////demos///////////////////////////////////////////////
       if(demo == "light_track_part_2")   {light_track_update_part_2();}
  else if(demo == "roll_to_plane")        {roll_to_plane();}
  else if(demo == "ready_set_jump")       {ready_set_jump();}
} 
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// HELPER FUNCTIONS//////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// HELPER FUNCTIONS//////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// HELPER FUNCTIONS//////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
void process_general_ros()
{
  if(cmd_received != "nothing") // only process this once...
  {
  if(cmd_received.substring(0,2).toInt() == cube_id){cmd = cmd_received.substring(2,3);} // this command was meant for a specific cube.. only works with colors WIP
  else if(cmd_received.substring(0,2).toInt() > 0){}
  else if(cmd_received.substring(0,2).toInt() == 0){cmd = cmd_received;}
  cmd_received = "nothing";
  }
}

//   int ambient_low             = 5;
//   int ambient_medium          = 14;
//   int detected_special_blink_recently =   {0, 0, 0, 0, 0, 0, 0};
//   int detected_signature_recently =       {0, 0, 0, 0, 0, 0, 0};
//   int bit_1_detected =                    {0, 0, 0, 0, 0, 0, 0};
//   int bit_2_detected =                    {0, 0, 0, 0, 0, 0, 0};


//int three_pulse_length = 6;
//int three_pulse[three_pulse_array_length] = {(-ambient_low),  ambient_high,  ambient_high,  ambient_high,  0, (-ambient_low)};
//                                                  -5           15              15              15        0      -5 

bool does_pattern_match_ambient(int face, int start_position, int pattern_to_match[], int pattern_array_length)
//*************************************************************************************************************************
//   this function applies the a filter, and will return true if all conditions in ambient_buffer beet the values
//*************************************************************************************************************************
{
   bool result = true;
   for(int i = 0; i  < pattern_array_length; i++)
      {
           if(result && pattern_to_match[i] == 0) {} // do nothing... we don't check this value
      else if(result && pattern_to_match[i]  > 0) {if(ambient_values[face][i+start_position] < pattern_to_match[i])      {result = false;}}
      else if(result && pattern_to_match[i]  < 0) {if(ambient_values[face][i+start_position] > abs(pattern_to_match[i])) {result = false;}}
      if(result == false){break;}
      }
   return result;
}


void detect_three_long_ambient_filter_uptdate()
//*************************************************************************************************************************
//   this function updates the numbered variables below.... which are decremented every times step by a different function
//   and bit 1[face] and 2[face] are reset then the detected_blink[face] reaches zero
//   (1) int detected_special_blink_recently[faces] =   {0, 0, 0, 0, 0, 0, 0};
//   (2) int detected_signature_recently[faces] =       {0, 0, 0, 0, 0, 0, 0};
//   (3) int bit_1_detected[faces] =                    {0, 0, 0, 0, 0, 0, 0};
//   (4) int bit_2_detected[faces] =                    {0, 0, 0, 0, 0, 0, 0};// updates
//*************************************************************************************************************************
{
  int beginning = 0;
  int signature = 10;
  //int three_pulse[three_pulse_length] = {(-ambient_low),  ambient_high,  ambient_high,  ambient_high,  0, (-ambient_low)};
  for(int face = 1; face < FACES; face ++) // go over every face
    {    
      if(does_pattern_match_ambient(face, beginning, three_pulse, three_pulse_length)) {detected_signature_recently[face] = 5;}
      if(does_pattern_match_ambient(face, signature, three_pulse, three_pulse_length)) {detected_special_blink_recently[face] = 5;}
      {  
        detected_special_blink_recently[face] = 5;
        bit_1_detected[face] = 0;        
        bit_2_detected[face] = 0;        
      }      
   }
}
void print_out_ambient_buffer(int face)
{
  Serial.print("Face: ");Serial.print(face);Serial.print("  [most recent]");
  for(int i = 0; i < ARRAY_LENGTH_LONG-1; i++)
  {
    Serial.print(" | ");Serial.print(ambient_values[face][i]);
   }
   Serial.print(" | ");Serial.println(ambient_values[face][ARRAY_LENGTH_LONG-1]);
}

void print_out_connections()
{
  Serial.println("__________________");
  Serial.print("HI! I am cube number: "); Serial.println(cube_id);
  //
  for(int face = 1; face < FACES; face++)
  {
    
    Serial.print(" face: ");
    Serial.print(face);
    Serial.print(" is connected to cube");
    Serial.print(face_neighbors[face][n_cube_id]);
    Serial.print(" face ");
    Serial.print(face_neighbors[face][n_face_id]);
    //Serial.print(" face ");
    //Serial.print(face_neighbors[face][n_face_id]);
    Serial.println(" WOOOOOOO!");
    
  }
  Serial.println("__________________");
}
void reset_esp()
{
  Serial.println("esprst");
}

void shift_and_update_lights()
{
   location_message_special_blink();
   signature_blink();
   blink_green();
   blink_yellow();
   blink_red();
   blink_blue();
   blink_purple();
   blink_teal();  
   blink_white();
   blink_ir();
   blink_ir_face();
}

void shift_and_update_ambient_sensors()
{
   for(int face = 1; face < ACTIVE_FACES; face++)
   {
      for(int j = ARRAY_LENGTH_LONG-1; j > 0; j--){ambient_values[face][j] = ambient_values[face][j-1];}
      ambient_values[face][0] = read_ambient(face); 
   }
}

void shift_and_update_IMU()
{
  for(int j = ARRAY_LENGTH_LONG;  j > 0; j--){motion_sum_log[j] = motion_sum_log[j-1];}  motion_sum_log[0] = read_gyro_and_accel(); 
  for(int j = ARRAY_LENGTH_SHORT; j > 0; j--){GyX_log[j] = GyX_log[j-1];}  GyX_log[0] = GyX;
  for(int a = ARRAY_LENGTH_SHORT; a > 0; a--){GyY_log[a] = GyY_log[a-1];}  GyY_log[0] = GyY;
  for(int b = ARRAY_LENGTH_SHORT; b > 0; b--){GyZ_log[b] = GyZ_log[b-1];}  GyZ_log[0] = GyZ;
  for(int j = ARRAY_LENGTH_SHORT; j > 0; j--){AcX_log[j] = AcX_log[j-1];}  AcX_log[0] = AcX;
  for(int j = ARRAY_LENGTH_SHORT; j > 0; j--){AcY_log[j] = AcY_log[j-1];}  AcY_log[0] = AcY;
  for(int j = ARRAY_LENGTH_SHORT; j > 0; j--){AcZ_log[j] = AcZ_log[j-1];}  AcZ_log[0] = AcZ;
}

String check_for_patterns_gyros()
{
int low = 1200;
int high = 5000;
String color = "default";
///////////////////////////////////////THESE represent rolling about 3 axis...
     if(motion_sum_log[1] > 15000 && motion_sum_log[8] > 15000 && motion_sum_log[17] > 15000)                                           {color = "angry";cmd = "chill";}
     if(GyX_log[0] > low && GyX_log[0] < high && GyX_log[3] > low && GyX_log[3] < high && GyX_log[8] > low && GyX_log[8] < high)         {color = "y"; y_counter = 4;}
else if(GyX_log[0] < -low && GyX_log[0] > -high && GyX_log[3] < -low && GyX_log[3] > -high &&GyX_log[8] < -low && GyX_log[8] > -high)   {color = "p"; p_counter = 4;}
else if(GyY_log[0] > low && GyY_log[0] < high && GyY_log[3] > low&& GyY_log[3] < high &&GyY_log[8] > low && GyY_log[8] < high)          {color = "r"; r_counter = 4;}
else if(GyY_log[0] < -low && GyY_log[0] > -high && GyY_log[3] < -low && GyY_log[3] > -high &&GyY_log[8] < -low && GyY_log[8] > -high)   {color = "b"; b_counter = 4;}
else if(GyZ_log[0] > low && GyZ_log[0] < high && GyZ_log[3] > low && GyZ_log[3] < high &&GyZ_log[8] > low && GyZ_log[8] < high)         {color = "t"; t_counter = 4;}
else if(GyZ_log[0] < -low && GyZ_log[0] > -high && GyZ_log[3] < -low && GyZ_log[3] > -high &&GyZ_log[8] < -low && GyZ_log[8] > -high)   {color = "g"; g_counter = 4;}
return(color);
}

void attempt_reset_i2c()
{
Serial.println("RESETING i2c NOW!!!");
digitalWrite(Switch, LOW);
delay(100);
digitalWrite(Switch, HIGH);
Wire.begin(2, 14);
}

void things_to_do_as_fast_as_possible()
{
      if(cmd == "sleep")                         {go_to_sleep();}
      if(cmd == "off")                           {rgboff();IRLED_OFF();delay(40);cmd ="chill";}// go_to_plane(int plane)
      if(cmd == "esp_off")                       {turn_off_esp();}
      if(cmd == "esp_reset")                     {reset_esp();}
      if(DEBUG && DEBUG_2 && is_ros_on)          {Serial.println("BEFORE SPIN ONCE");}
      if(is_ros_on)                              {nh.spinOnce();} 
      if(DEBUG && DEBUG_2 && is_ros_on)          {Serial.println("AFTER SPIN ONCE");}
      if(cmd == "update" && is_ros_on)           {publishstatus(); cmd = cmd_previous;}
      if(cmd == "chill")                         {delay(1);}
      if(cmd == "special")                       {delay(1);}
      
      if(cmd == "r" || color == "r")             {if(loop_counter % 1 == 0) {rgbr();}}//cmd ="chill";}
 else if(cmd == "g" || color == "g")             {if(loop_counter % 1 == 0) {rgbg();}}//cmd ="chill";}
 else if(cmd == "b" || color == "b")             {if(loop_counter % 1 == 0) {rgbb();}}//cmd ="chill";}
 else if(cmd == "p" || color == "p")             {if(loop_counter % 1 == 0) {rgbp();}}//cmd ="chill";}
 else if(cmd == "y" || color == "y")             {if(loop_counter % 1 == 0) {rgby();}}//cmd ="chill";} 
 else if(cmd == "t" || color == "t")             {if(loop_counter % 1 == 0) {rgbt();}}//cmd ="chill";} 
 else if(cmd == "w" || color == "w")             {if(loop_counter % 1 == 0) {rgbw();}}//cmd ="chill";} 
    
      cmd_previous = cmd;
      
}

void check_for_action_slow()
{
  if(cmd == "f1")                                 {move_horizontal();cmd = "chill";}
  if(cmd == "f2")                                 {move_fast_twice();cmd = "chill";}
  if(cmd == "lit")                                {face_rgb(which_face_is_up(12000),1,0,1,0);}
  if(cmd == "rain")                               {taste_the_rainbow();}
  if(cmd == "cp1")                                {if(go_to_plane(2546)){cmd = "g";} else{cmd = "r";}}
  if(cmd == "cp5")                                {if(go_to_plane(1234)){cmd = "g";} else{cmd = "r";}}
  if(cmd == "cp2")                                {if(go_to_plane(1536)){cmd = "g";} else{cmd = "r";}}
  if(cmd == "dance")                              {Serial.println("bldcaccel f 5000 800"); cmd ="rain";}
  if(cmd.substring(0,8) == "forward" || cmd.substring(0,8) == "reverse")
  {
            
         if(cmd == "forward_traverse" || cmd == "traverse_forward")   {move_normal("f","90 deg",6000, 24, 12,"e 10", 3000);cmd ="chill";}
    else if(cmd == "reverse_traverse" || cmd == "traverse_reverse")   {move_normal("r","90 deg",6000, 24, 12,"e 10", 3000);cmd ="chill";}
    else if(cmd == "forward_corner" || cmd == "corner_forward")       {move_normal("f","180 deg",14000, 40, 5,"e 10", 6000);cmd ="chill";}
    else if(cmd == "reverse_corner" || cmd == "corner_reverse")       {move_normal("r","180 deg",14000, 40, 5,"e 10", 6000);cmd ="chill";}
    else if(cmd == "forward_stair")                                   {move_normal("f","90 deg",6000, 23, 10,"e 10", 3000);cmd ="chill";}
    else if(cmd == "reverse_stair")                                   {move_normal("r","90 deg",6000, 23, 10,"e 10", 3000);cmd ="chill";}
    else if(cmd == "forward_down")                                    {move_normal("f","0 deg",6000, 24, 12,"e 10", 6000);cmd ="chill";}
    else if(cmd == "reverse_down")                                    {move_normal("r","0 deg",6000, 24, 12,"e 10", 6000);cmd ="chill";}
    else if(cmd == "forward_wall")                                    {move_normal("f","90 deg",6000, 23, 10,"e 10", 3000);cmd ="chill";}
    else if(cmd == "reverse_wall")                                    {move_normal("r","90 deg",6000, 23, 10,"e 10", 3000);cmd ="chill";}
    else if(cmd == "forward_horizontal_traverse")                     {move_normal("f","180 deg",14000, 40, 5,"e 10", 6000);cmd ="chill";}
    else if(cmd == "reverse_horizontal_traverse")                     {move_normal("r","180 deg",14000, 40, 5,"e 10", 6000);cmd ="chill";}
  }
}

