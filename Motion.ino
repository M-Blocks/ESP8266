bool rock_back_and_forth()
{
Serial.println("bldcaccel f 4000 400");
if(is_ros_on){publishAwake();}
if(is_ros_on){nh.spinOnce();}
ros_safe_delay(400);
Serial.println("bldcstop b");
ros_safe_delay(400);
Serial.println("bldcaccel r 4000 400");
ros_safe_delay(400);
if(is_ros_on){publishAwake();} 
if(is_ros_on){nh.spinOnce();}
Serial.println("bldcstop b");
ros_safe_delay(400);
}

bool move_fast_twice( )//String for_rev, String rotation_ammount, int flywheel_rpm, int brake_current, int brake_time, String ending,  int delay_time)
{
  Serial.println("bldcaccel f 5000 350");
  ros_safe_delay(600);
  Serial.print("brake f 2500 10");
  ros_safe_delay(600);
  Serial.println("bldcstop b");
  ros_safe_delay(600);
  Serial.println("bldcaccel f 5000 350");
  ros_safe_delay(600);
  Serial.print("brake f 2500 10");
  ros_safe_delay(600);
}

int move_horizontal()//(String for_rev, String rotation_ammount, int flywheel_rpm, int brake_current, int brake_time, String ending,  int delay_time)
{
  delay(100);  
  Serial.println("bldcaccel f 2200 320");
  delay(250);
  Serial.println("brake f 3500 4 e 10");
  //Serial.println("brake " + for_rev + String(brake_current) + " " + String(brake_time));
  delay(30);
  Serial.println("bldcaccel r 3500 600");
  delay(700);
  Serial.println("bldcstop b");
}

//Move returns
// 0 = move failed... 
// -number = move failed and wrong face is up + Violence makes number Higher
// 90 for 90 degree movement
// 180 for 180 degree movement
// 45, and 135 for partial moves...
int move_normal(String for_rev, String rotation_ammount, int flywheel_rpm, int brake_current, int brake_time, String ending,  int delay_time)
{
  int attempts = 3;
  int up_face = which_face_is_up(12000);
  int forward_face = which_face_is_forward(which_plane_fast());
  int reverse_face = opposite_face(forward_face);
  int expected_up_face = -1;
  int backup_expected_up_face = opposite_face(up_face);
 
  // This area reasons to figure out expected_up_face, and applies the cube specific offsets to 
  if(for_rev == "r")
  {
      brake_current = brake_current*reverse_offset; 
      if(rotation_ammount == "90 deg"){expected_up_face = forward_face;}
      else if(rotation_ammount == "180 deg"){expected_up_face = opposite_face(up_face);}
      else if(rotation_ammount == "0 deg"){expected_up_face = -1;}
      else{return(false);}
  }
      
  else if(for_rev == "f")
  {
      brake_current = brake_current*forward_offset;
      if(rotation_ammount == "90 deg"){expected_up_face = reverse_face;}
      else if(rotation_ammount == "180 deg"){expected_up_face = opposite_face(up_face);}
      else if(rotation_ammount == "0 deg"){expected_up_face = -1;}
      else{return(false);}
      
  }
  else
  {return(false);} // if string is neither "f" or "r" we exit...
  
  if(expected_up_face == -1) // this is for moves where we don't know what will be the new face... check movement ammount
  {
      rgbb(); // turn face LED's to be blue! JUST FOR FUN
      String command = "ia "+ for_rev +" " + String(flywheel_rpm)+" "+String(brake_current+special_offset)+" "+String(brake_time)+" "+ ending;
      Serial.println(command);
      delay(250); rgboff();
      attempts += 1;
      for(int i = 1; i < 3;i++){Serial.println("stillalive");delay(50);}
      ros_safe_delay(delay_time/2);
      for(int i = 1; i < 3;i++){Serial.println("stillalive");delay(50);}
      ros_safe_delay(delay_time/2);
      Serial.println("stillalive");  
  }
  else // in this case we will move until we either exhaust number of tries, or we end up in the correct face
  {
    while(up_face != backup_expected_up_face && up_face != expected_up_face && attempts <= 3 && which_face_is_up(12000) < 7)
    {
      rgbp(); // turn face LED's to be purple! JUST FOR FUN
      String command = "ia "+ for_rev +" " + String(flywheel_rpm)+" "+String(brake_current+special_offset)+" "+String(brake_time)+" "+ ending;
      Serial.println(command);
      delay(250); rgboff();
      attempts += 1;
      for(int i = 1; i < 3;i++){Serial.println("stillalive");delay(50);}
      ros_safe_delay(delay_time/2);
      for(int i = 1; i < 3;i++){Serial.println("stillalive");delay(50);}
      ros_safe_delay(delay_time/2);
      Serial.println("stillalive");      
      // If the command is printed again, This means we failed, so we now bump up all of the parameters, to hopefully succeed next time
      brake_current = brake_current + 300; 
      flywheel_rpm = flywheel_rpm + 400;
      brake_time = brake_time + 2;
      up_face = which_face_is_up(12000);
    }
  }
       if(up_face == expected_up_face)            {Serial.println("stillalive");rgbg();delay(250);rgboff();return(90);}
  else if(up_face == backup_expected_up_face)     {Serial.println("stillalive");rgbg();delay(250);rgboff();return(180);}
  else                                            {Serial.println("stillalive");rgbr();delay(250);rgboff(); return(0);}
}


