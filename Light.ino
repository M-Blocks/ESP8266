// for(int i = 0; i <5;i++){rgbb();delay(100);rgboff();delay(100);} // This just flashes the lights blue 5 times

// for(int x = 1; x <= 6; x++){Serial.print("Face light level ");Serial.print(x); Serial.print(" = ");Serial.println(fblight_temp[x]);}
// Serial.print("Up Face is: ");Serial.println(up_face);
// for(int x = 1; x <= 6; x++){Serial.print("Face bright level ");Serial.print(x); Serial.print(" = ");Serial.println(brightest[x]);}

//////// How Long did this take?
//long begin_time = millis();
//Serial.print("Function took: ");Serial.println(millis()-begin_time);
//////// 

int light_track_update()
{
//Step 1: Update Sensor Values //
          //Serial.println("Beginning light_track_update");
          digitalWrite(Switch, HIGH);
          rgboff();
          int threshold = 3;
          int for_face = which_face_is_forward(which_plane_fast());
          int rev_face = which_face_is_reverse();
          int up_face = which_face_is_up(12000);
          int value_of_top_face = read_ambient(up_face);
          int fblight_list[7] = {0,0,0,0,0,0,0};
          for(int x = 1; x < 7; x++){fblight_list[x] = read_ambient(x);} // Populates temp. array of light values
          if(up_face > 0 && up_face <7){fblight_list[up_face] = -1;} // Zeros out the exclude face
          else{for(int x = 1; x <= 6; x++){fblight_list[x] = 0;}} // this means cube isn't stable, so we zero everything
          int sum_of_horizontal = 0;
          for(int x = 0; x < 7; x++){sum_of_horizontal+=fblight_list[x];}
          int brightest[2] = {which_face_is_brightest(fblight_list),which_face_is_brightest(fblight_list)};
          
          //Serial.print("sum of horizontal: ");Serial.println(sum_of_horizontal);
          //for(int x = 1; x <= 6; x++){Serial.print("Face light level ");Serial.print(x); Serial.print(" = ");Serial.println(fblight_list[x]);}
          //Serial.print("    Brightest Face is: ");Serial.println(brightest[0]);
          //Serial.print("2nd Brightest Face is: ");Serial.println(brightest[1]);
          //for(int x = 1; x <= 6; x++){Serial.print("Face bright level ");Serial.print(x); Serial.print(" = ");Serial.println(brightest[x]);}
          //face_rgb(brightest[0], 1, 1, 1, 1);

//Step 2: Choose Action
     // Forward face has brightest light: Action move forward
          if(brightest[0] == for_face && sum_of_horizontal > threshold) // 
          {
          Serial.println("bldcspeed f 9000");
          face_rgb(for_face,0,1,0,1);
          ros_safe_delay(2600);
          Serial.println("bldcstop b");
          
          }
     // Reverse face has brightest light: Action move forward
          else if(brightest[0] == rev_face && sum_of_horizontal > threshold)
          {
          Serial.println("bldcspeed r 9000");
          face_rgb(rev_face,1,1,0,1);
          ros_safe_delay(2600);
          Serial.println("bldcstop b");
          }
     // Cube flywheel is coplaner with ground...: go crazy!!!!!!
          else if(for_face > 7) // This means that the cube has no "forward" face and rolls around randomly.
          {
          for(int i = 0; i <3;i++){rgbb();delay(100);rgboff();delay(100);} // This just flashes the lights blue 5 times
          Serial.println("bldcaccel f 4800 1500");
          ros_safe_delay(1000);
          }
      // Forward face is second brightest: Action: Roll cautionsly forward
          else if(brightest[1] == for_face && sum_of_horizontal > threshold)
          {
          face_rgb(for_face,1,1,0,1);
          roll_one_step(1);
          ros_safe_delay(1000);
          }
      // Reverse Face is second Brightest: Action: Roll cautionsly in reverse
          else if(brightest[1] == rev_face && sum_of_horizontal > threshold)
          {
          face_rgb(which_face_is_forward(which_plane_fast()),1,0,1,0);
          face_rgb(rev_face,1,1,0,1);
          roll_one_step(-1);
          ros_safe_delay(1000);
          
          }
      // None of these conditions are met...
          else
          {
          for(int i = 0; i <5;i++){rgbb();delay(100);rgboff();delay(100);}
          rock_back_and_forth();
          ros_safe_delay(1000);
          //Serial.println("ia r 4000 2000 10 a 10 r");
          }
          
//Step 3: Test update state Machine
ros_safe_delay(1000);
previous_movement_ammount = movement_ammount;
movement_ammount = wiggle_test();

if(movement_ammount < 250 && previous_movement_ammount < 250){previous_movement_ammount = 1000; movement_ammount = 1000; Serial.println("I THINK I CONNECTED");cmd = "g";}

//
}

bool light_track_update_part2()
{
  // Step 1: Look up up face...
  // Step 2: Figure out what plane corresponds to horizontal...
  // if(current_plane != horizontal plane)
//  {
//    change plane...
//  }
     // else if(not 100% connected){Serial.println("ia f/r 5000 4000 10");
     // else if(100% connected){flash_green(5);cmd = "g"}
     // else {  }// we don't know what is going on here...
}

int wiggle_test()
//
//read_accel() read_gyro();
//AcX,AcY,AcZ,GyX,GyY,GyZ,
{
  //Serial.println("BEGINNING WIGGLE_TEST");
  int movement = 0;
  String command = "bldcaccel f " + String(2500 + 3*plane_change_offset) + " " + String(700 + plane_change_offset);
  Serial.println(command);
  
  ////////////////////////////// Reads the gyro for 10*30 ms
  for(int i = 0; i < 12; i++)
  {
   read_gyro();
   delay(10);
   movement += (abs(GyX)+abs(GyY)+abs(GyZ))/100;
   delay(55);
  }
  delay(plane_change_offset);
  //////////////////////////////
  Serial.println("bldcstop b");
  ////////////////////////////// Reads the gyro for 10*30 ms
  for(int i = 0; i < 8; i++)
  {
   read_gyro();
   delay(2);
   movement += (abs(GyX)+abs(GyY)+abs(GyZ))/100;
   delay(20);
  }
  //////////////////////////////
  //Serial.print("Sum of movement is: ");Serial.println(movement);
  return(movement);
}

int which_face_is_brightest(int fblight_temp[])
// Sorts input list (fblight_temp). Running it a second time, returns 2nd brightest...
// running a 3rd time returns 3rd brightest...
{
  int brightest_list[6] = {0,0,0,0,0,0};
  for(int i = 1; i < 7 ;i++){if(fblight_temp[brightest_list[0]] < fblight_temp[i]){brightest_list[0] = (i);}}
  fblight_temp[brightest_list[0]] = 0;
  return(brightest_list[0]);
}
