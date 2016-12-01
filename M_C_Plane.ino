void change_plane_to_parallel(int plane)
{
  if(plane == 5 || plane == 6){cmd = "cp5";}
  else if (plane == 2 || plane == 4){cmd = "cp2";}
  else if (plane == 1 || plane == 3){cmd = "cp1";}
}

//  if(cmd == "cp1"){go_to_plane(2546)}
//  if(cmd == "cp5"){go_to_plane(1234)}
//  if(cmd == "cp2"){go_to_plane(1536)}

// which_orientation_is_central(); -- Uses central IMU to return either 1234 1536 or 2546
// read_accel_central();
// which_face_is_up(int threshold);
// face_rgb(int face, int r, int g, int b, int lights_off)
// which_plane_fast(); returns  either 1234 1536 or 2546
// which_plane_slow()
// ros_safe_delay(int delay_time)

int which_plane_cautious()
{
   int up_face = which_face_is_up(12000);
   if(up_face == -1){up_face = face_that_was_last_up;}
   int best_guess = which_plane_fast();
   int other_guess = which_plane_slow();
   if(best_guess == other_guess && best_guess != -1) // If the two methods aggree. return
   {return(best_guess);}
   
   else // This means that one of the methods returns -1... we assume that the 
   {
   
    if(up_face == 5)
    {
    face_rgb(5,1,0,1,0);
    delay(200);
    rgboff();
    if (AcZ_c > 1000 && AcY_c > 1000 && AcX_c > 1000) {best_guess = 52;}     // GREEN in test
    else if (AcZ_c < 0 && AcX_c > 10000 && AcX_c > AcY_c) {best_guess = 21;} // BLUE  in test
    else if (AcY_c < -1000 && AcX_c > 1000) {best_guess = 15;}               // RED   in test
    }
   else if(up_face == 1)
   {
    face_rgb(1,1,0,1,0);
    delay(200);
    rgboff();
    if (AcZ_c > 1000 && AcY_c > 1000 && AcX_c > 1000) {best_guess = 15;}
    else if (AcZ_c < 0 && AcX_c > 10000 && AcX_c > AcY_c) {best_guess = 52;}
    else if (AcY_c < -1000 && AcX_c > 1000) {best_guess = 21;}
   }
   else if(up_face == 2)
   {
    face_rgb(2,1,0,1,0);
    delay(200);
    rgboff();
    if (AcZ_c > 1000 && AcY_c > 1000 && AcX_c > 1000) {best_guess = 21;}
    else if (AcZ_c < 0 && AcX_c > 10000 && AcX_c > AcY_c) {best_guess = 15;}
    else if (AcY_c < -1000 && AcX_c > 1000) {best_guess = 52;}
   }
   else if(up_face == 3)
   {
    face_rgb(3,1,0,1,0);
    delay(200);
    rgboff();
    if (AcZ_c < -1000 && AcY_c < -1000 && AcX_c < -1000) {best_guess = 52;}
    else if (AcZ_c < 0 && AcX_c > 10000 && AcX_c > AcY_c) {best_guess = 21;}
    else if (AcY_c < -1000 && AcX_c > 1000) {best_guess = 15;}
   }
   else if(up_face == 4)
   {
    face_rgb(5,1,0,1,0);
    delay(200);
    rgboff();
    if (AcZ_c < -1000 && AcY_c < -1000 && AcX_c < -1000) {best_guess = 52;}
    else if (AcZ_c < 0 && AcX_c > 10000 && AcX_c > AcY_c) {best_guess = 21;}
    else if (AcY_c < -1000 && AcX_c > 1000) {best_guess = 15;}
   }  
   else if(up_face == 6)
   {
    face_rgb(6,1,0,1,0);
    delay(200);
    rgboff();
    if (AcZ_c < -1000 && AcY_c < -1000 && AcX_c < -1000) {best_guess = 52;}
    else if (AcZ_c < 0 && AcX_c > 10000 && AcX_c > AcY_c) {best_guess = 21;}
    else if (AcY_c < -1000 && AcX_c > 1000) {best_guess = 15;}
   }
  }
   return(best_guess);
}

bool go_to_plane(int desired_plane)
{
//STEP 0: Initialize variables, check if we are already in correct plane...
    int lattice_connected = 0;
    int up_face = which_face_is_up(12000);
    face_that_was_last_up = up_face;
    int current_plane = which_plane_cautious();
    if(current_plane == desired_plane){return(true);}
    
//STEP 1: Check to see if we are attached to a lattice...
    // Perform Wiggle Test --- IF NOT RETURN: also look if face_that_is_up changed... RETURN IF SO
    if(current_plane > 1000)
        {
        if(wiggle_test("f") < 500){lattice_connected = 1;}
        }
    if(up_face == which_face_is_up(12000)){lattice_connected = 1;}
    else{lattice_connected = 0;return(false);}
    
//Step 2:
    long start_time = millis();
    Serial.println("sma retract 7000"); // Begin by retracting the pin, allowing the central part to spin
    ros_safe_delay(400); // wait to let it take effect
    int milliamps_updateable = 4000; // these are the default values for the burst of acceleration
    int milliseconds_updateable = 100; // time default
    milliseconds_updateable += plane_change_offset;
    milliamps_updateable += 2*plane_change_offset;
    int loop_count = 0;
    int loop_count_1 = 0;
    while(millis()-start_time < 7000 && current_plane != desired_plane) // while we aren't in correct plane, and we are still under 6 seconds...
    {
     Serial.println("stillalive");
     sum_of_diff = 0;
     delay(50);
     String command_forward = "bldcaccel f " + String(2300+loop_count_1*15) + " " + String(110 + loop_count_1 + plane_change_offset/3);
     String command_reverse = "bldcaccel r " + String(2300+loop_count_1*15) + " " + String(110 + loop_count_1 + plane_change_offset/3);
     if(current_plane > 1000)
     {
     accel_jump("f", milliamps_updateable, milliseconds_updateable); // try to move plane 
     ros_safe_delay(400);
     loop_count += 25; // This counter counts how many times we have tried accel_jump
     }
     else{
     delay(100);
     current_plane = which_plane_cautious();
     if((current_plane == 52 && desired_plane == 1234) || (current_plane == 21 && desired_plane == 1536) || (current_plane == 15 && desired_plane == 2546)) {Serial.println(command_forward); ros_safe_delay(400); loop_count_1 += 10;}
     else if( (current_plane == 52 && desired_plane == 1536) || (current_plane == 21 && desired_plane == 2546) || (current_plane == 15 && desired_plane == 1234)) {Serial.println(command_reverse); ros_safe_delay(400); loop_count_1 += 10;}
     else if(current_plane == desired_plane){break;}
     else if(current_plane > 1000) {r_counter = 4;Serial.println("bldcaccel f 4000 140");ros_safe_delay(600);}
     else{Serial.println("bldcaccel r 4000 200"); ros_safe_delay(200); Serial.println("bldcstop b");}
     }
     if(sum_of_diff < 10000){milliamps_updateable += 300; milliseconds_updateable += 40;}
     milliseconds_updateable += loop_count;
     current_plane = which_plane_cautious();
     delay(20);
    }
    // This section of code runs when the timer is up... or if there is a problem with the plane
    current_plane = which_plane_cautious();
    delay(50);
    if(current_plane != desired_plane && current_plane > 1000) // if the central part is not alligned with any axis.
    {
     Serial.println("stillalive");
     delay(500);Serial.println("sma retract 2000");
     delay(50);Serial.println("bldcaccel r " + String(2500+plane_change_offset*15) + " " + String(110 + plane_change_offset/3));
     delay(200);Serial.println("bldcaccel f " + String(2600+plane_change_offset*15) + " " + String(110 + plane_change_offset/3));
     Serial.println("stillalive");
     }
    while(millis()-start_time < 8000){ros_safe_delay(400);}
    if(current_plane == desired_plane){return(true);}
    else{return(false);}
}

int accel_jump(String for_rev, int milliamps, int milliseconds)
{
     Serial.println("stillalive");
     if(milliamps > 5500){milliamps = 5000;}
     delay(20);
     //read_accel_central(); // now AcX_c, AcY_c, and AcZ_c are updated
     //delay(75);
     String command = "bldcaccel "+ for_rev + " " + String(milliamps) + " " + String(100 + milliseconds);
     //                bldcaccel        f/r        4000                            300
     Serial.println(command);
     int numb = 2;
     if(milliseconds > 200 && milliseconds < 300){numb = 3;}
     else if(milliseconds > 300 && milliseconds < 400){numb = 4;}
     else if(milliseconds > 400 && milliseconds < 500){numb = 5;}
     else if(milliseconds > 500 && milliseconds < 600){numb = 6;}
     else if(milliseconds > 600 && milliseconds < 700){numb = 7;}
     delay(50);
     Serial.println("stillalive");
     for(int i = 0; i < numb; i++) {sum_of_diff += wait_and_detect_difference_maget();}
     Serial.println("bldcstop b");
     delay(50);
     for(int i = 0; i < 3; i++){sum_of_diff += wait_and_detect_difference_maget(); delay(100);}
     delay(30);
     Serial.println("stillalive");
}

int wait_and_detect_difference_maget()
{
 Serial.println("stillalive");
 int variance_of_mag_readings = 0;
 int sum_of_mag_readings = 0;
 int mag_readings[5] = {0,0,0,0,0};
 for(int i = 0; i<5;i++){mag_readings[i] = readMagnet();delay(20);}
 for(int i = 0; i<5;i++){sum_of_mag_readings += mag_readings[i];}
 for(int i = 0; i<5;i++){variance_of_mag_readings += abs(sum_of_mag_readings/5 - mag_readings[i]);}
 return(variance_of_mag_readings);
}
int wait_and_detect_difference()
{
 Serial.println("stillalive");
 delay(50);
 AcX_c_old = AcX_c; AcY_c_old = AcY_c; AcZ_c_old = AcZ_c;
 delay(5);
 read_accel_central();
// Serial.println(AcX_c_old); 
// Serial.println(AcX_c);
// Serial.println(AcY_c_old);
// Serial.println(AcY_c);
// Serial.println(AcZ_c_old);
// Serial.println(AcZ_c);
 delay(30);
 //Serial.println("stillalive");
 sum_of_diff += (abs(AcX_c_old - AcX_c) + abs(AcY_c_old - AcY_c) + abs(AcZ_c_old - AcZ_c));
 //Serial.print("sum_of_diff = ");
 //Serial.println(sum_of_diff);
 delay(10);
}

//    if      ((AcX_c > (1 - e))     && (AcX_c < (1 + e))     && (AcY_c > (1 - e))     && (AcY_c < (1 + e))     && (AcZ_c > (16000 - e)) && (AcZ_c < (16000 + e)) ){best_guess = 1234;}
//    else if ((AcX_c > (1 + e))     && (AcX_c < (10000))     && (AcY_c > (1 + e))     && (AcY_c < (10000))     && (AcZ_c > -1000)       && (AcZ_c < (16000 + e)) ){best_guess = 52;}
//    else if ((AcX_c > (11000 - e)) && (AcX_c < (11000 + e)) && (AcY_c > (11000 - e)) && (AcY_c < (11000 + e)) && (AcZ_c > (1 - e))     && (AcZ_c < (1 + e))     ){best_guess = 1536;}
//    else if ((AcX_c > (10000 - e)) && (AcX_c < (16000 + e)) && (AcY_c > (-11000 - e))&& (AcY_c < (10000 - e)) && (AcZ_c > (-7000 - e)) && (AcZ_c < (1 - e))     ){best_guess = 21;}
//    else if ((AcX_c > (11000 - e)) && (AcX_c < (11000 + e)) && (AcY_c > (-11000 - e))&& (AcY_c < (-11000 + e))&& (AcZ_c > (1 - e))     && (AcZ_c < (1 + e))     ){best_guess = 1536;}
//    else if ((AcX_c > (1 - e))     && (AcX_c < (11000 + e)) && (AcY_c > (-11000 - e))&& (AcY_c < (1 + e))     && (AcZ_c > (-11000 - e))&& (AcZ_c < (14000 - e)) ){best_guess = 15;}
//    else{best_guess = -1;}
//   }



