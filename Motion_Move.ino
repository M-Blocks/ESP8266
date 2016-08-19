// which_face_is_reverse();
// which_face_is_forward(which_plane_fast()))
// forward_offset
// reverse_offset
// Sample:  move_normal("f"/"r", 90/180,               3000-18000,       1500-6000/100,     10-50 ms      , "a 10 r"/"e 10",    2000, or 3000
//   move_normal("f","90 deg",6000, 2300, 10, "e 10", 2000);

bool rock_back_and_forth()

{
Serial.println("bldcaccel f 4000 400");
delay(200);
publishAwake();
nh.spinOnce();
delay(200);
Serial.println("bldcstop b");
delay(200);
Serial.println("bldcaccel r 4000 400");
delay(200);
publishAwake();
nh.spinOnce();
delay(200);
Serial.println("bldcstop b");
}


bool roll_one_step(int for_rev)
// Positive values = forward, negative is reverse

{
  if(for_rev > 0)
  {
  Serial.println("bldcaccel f 3800 320");
  }
  else
  {
  Serial.println("bldcaccel r 3800 320");
  }
  delay(350);
  Serial.println("bldcstop b");
  return(true);
}
bool move_fast_twice( )//String for_rev, String rotation_ammount, int flywheel_rpm, int brake_current, int brake_time, String ending,  int delay_time)
{
  Serial.println("bldcaccel f 5000 350");
  delay(440);
  Serial.print("brake f 2500 10");
  delay(200);
  Serial.println("bldcstop b");
  delay(500);
  Serial.println("bldcaccel f 5000 350");
  delay(440);
  Serial.print("brake f 2500 10");
  delay(200);
}

bool move_horizontal()//(String for_rev, String rotation_ammount, int flywheel_rpm, int brake_current, int brake_time, String ending,  int delay_time)
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
    
  
//  if(for_rev == "r"){brake_current = brake_current*reverse_offset;}
//  else if(for_rev == "f"){brake_current = brake_current*forward_offset;}
//  int movement_ammount_1 = 0;
//  int attempts = 0;
//  while(movement_ammount_1 <= 1000 && attempts <= 3)
//    {
//      rgbb(); // turn face LED's to be blue!
//      if(flywheel_rpm < 3300)
//        {
//          Serial.println("bldcaccel " + for_rev + " 4000 300");
//          delay(300);
//          Serial.println("brake " + for_rev + String(brake_current) + " " + String(brake_time));
//          delay(20);
//          Serial.println("bldcaccel e");
//        }
//      String command = "ia "+ for_rev +" " + String(flywheel_rpm)+" "+String(brake_current+special_offset)+" "+String(brake_time)+" "+ ending;
//      Serial.println(command);
//      delay(250);
//      rgboff();
//      attempts += 1;
//      ros_safe_delay(delay_time);
//      // If the command is printed again, This means we failed, so we now bump up all of the parameters, to hopefully succeed next time
//      brake_current = brake_current + 300; 
//      flywheel_rpm = flywheel_rpm + 400;
//      brake_time = brake_time + 2;
//  }
}

bool move_normal(String for_rev, String rotation_ammount, int flywheel_rpm, int brake_current, int brake_time, String ending,  int delay_time)
{
  // ia f 6000 4000 10 e 10
  int attempts = 1;
  int up_face = which_face_is_up(12000);
  int forward_face = which_face_is_forward(which_plane_fast());
  int reverse_face = opposite_face(forward_face);
  int expected_up_face = -1;
 
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
  else{return(false);} // if string is neither "f" or "r" we exit...
  //
  Serial.println("Made it here");
  if(expected_up_face == -1) // this is for moves where we don't know what will be the new face... check movement ammount
  {
      rgbb(); // turn face LED's to be blue! JUST FOR FUN
      String command = "ia "+ for_rev +" " + String(flywheel_rpm)+" "+String(brake_current+special_offset)+" "+String(brake_time)+" "+ ending;
      Serial.println(command);
      delay(250);
      rgboff();
      attempts += 1;
      ros_safe_delay(delay_time);
  }
  else
  {
    while(up_face != expected_up_face && attempts <= 3 && which_face_is_up(12000) < 7)
    {
      rgbp(); // turn face LED's to be purple! JUST FOR FUN
      String command = "ia "+ for_rev +" " + String(flywheel_rpm)+" "+String(brake_current+special_offset)+" "+String(brake_time)+" "+ ending;
      Serial.println(command);
      delay(250);
      rgboff();
      attempts += 1;
      ros_safe_delay(delay_time);
      // If the command is printed again, This means we failed, so we now bump up all of the parameters, to hopefully succeed next time
      brake_current = brake_current + 300; 
      flywheel_rpm = flywheel_rpm + 400;
      brake_time = brake_time + 2;
      up_face = which_face_is_up(12000);
  }
  }
  if(up_face == expected_up_face){Serial.println("SUCCESS!");rgbg();delay(250);rgboff();return(true);}
  else{Serial.println("FAILURE =(");rgbr();delay(250);rgboff(); return(false);}
}

