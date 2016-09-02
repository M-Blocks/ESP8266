void loop()
{
  fast_loop_counter++;
  //if(DEBUG){Serial.println("Main loop");}
  ///////////////////////
  if(cmd == "update"){publishupdate(); cmd = cmd_previous;}
  if(cmd == "lit"){face_rgb(which_face_is_up(12000),1,0,1,0);}
  if(cmd == "show_connections"){display_connected();delay(50);}
  if(cmd == "chill"){delay(50);}
  if(cmd == "rain"){taste_the_rainbow();}
  if(cmd == "f1"){move_horizontal();cmd = "chill";}
  if(cmd == "f2"){move_fast_twice();cmd = "chill";}
  if(cmd == "r"){rgbr();delay(20);cmd ="chill";}
  if(cmd == "g"){rgbg();delay(20);cmd ="chill";}
  if(cmd == "b"){rgbb();delay(20);cmd ="chill";}
  if(cmd == "p"){rgbp();delay(20);cmd ="chill";}
  if(cmd == "off"){rgboff();IRLED_OFF();delay(40);cmd ="chill";for(int i = 1; i < 7; i++){connected_faces[i] = 0;}}// go_to_plane(int plane)
  if(cmd == "cp1"){if(go_to_plane(2546)){cmd = "g";} else{cmd = "r";}}
  if(cmd == "cp5"){if(go_to_plane(1234)){cmd = "g";} else{cmd = "r";}}
  if(cmd == "cp2"){if(go_to_plane(1536)){cmd = "g";} else{cmd = "r";}}
  if(cmd == "dance"){Serial.println("bldcaccel f 5000 800"); cmd ="rain";}
  if(cmd == "sleep"){go_to_sleep();}
  //////////////////////////////////
  if(cmd == "traverse_forward"){move_normal("f","90 deg",6000, 24, 12,"e 10", 3000);cmd ="chill";}
  if(cmd == "traverse_reverse"){move_normal("r","90 deg",6000, 24, 12,"e 10", 3000);cmd ="chill";}
  if(cmd == "corner_forward"){move_normal("f","180 deg",14000, 40, 5,"e 10", 6000);cmd ="chill";}
  if(cmd == "corner_reverse"){move_normal("r","180 deg",14000, 40, 5,"e 10", 6000);cmd ="chill";}
  //////////////////////////////////
  if(cmd.substring(0,8) == "forward" || cmd.substring(0,8) == "reverse") // This convoluted thing allows us to send IR LED commands... it parses the state
  {
    if(cmd == "forward_traverse"){move_normal("f","90 deg",6000, 24, 12,"e 10", 3000);cmd ="chill";}
    if(cmd == "reverse_traverse"){move_normal("r","90 deg",6000, 24, 12,"e 10", 3000);cmd ="chill";}
    if(cmd == "forward_corner"){move_normal("f","180 deg",14000, 40, 5,"e 10", 6000);cmd ="chill";}
    if(cmd == "reverse_corner"){move_normal("r","180 deg",14000, 40, 5,"e 10", 6000);cmd ="chill";}
    if(cmd == "forward_stair"){move_normal("f","90 deg",6000, 23, 10,"e 10", 3000);cmd ="chill";}
    if(cmd == "reverse_stair"){move_normal("r","90 deg",6000, 23, 10,"e 10", 3000);cmd ="chill";}
    if(cmd == "forward_down"){move_normal("f","0 deg",6000, 24, 12,"e 10", 6000);cmd ="chill";}
    if(cmd == "reverse_down"){move_normal("r","0 deg",6000, 24, 12,"e 10", 6000);cmd ="chill";}
    if(cmd == "forward_wall"){move_normal("f","90 deg",6000, 23, 10,"e 10", 3000);cmd ="chill";}
    if(cmd == "reverse_wall"){move_normal("r","90 deg",6000, 23, 10,"e 10", 3000);cmd ="chill";}
    if(cmd == "forward_horizontal_traverse"){move_normal("f","180 deg",14000, 40, 5,"e 10", 6000);cmd ="chill";}
    if(cmd == "reverse_horizontal_traverse"){move_normal("r","180 deg",14000, 40, 5,"e 10", 6000);cmd ="chill";}
  }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
if(cmd == "find_connections") // this is designed to periodically send out the follow pattern OFF_ON_OFF_ON_OFF - with 100 ms intervals
{
  // Step 0: Update Zero faces... turn off all IR LED's and regular LED's... 
  // long funny_bs = millis();
    // ... Function...
    // ...............
    // Serial.print("This functiontook: ");
    // Serial.println(millis() - funny_bs);
  update_zero_faces();
  //for(int i = 1; i <7; i++) {if(zero_faces[i] == 1){Serial.print("ZERO FACE: "); Serial.println(zero_faces[i]);}}
  rgboff();
  IRLED_OFF();
  //for(int i = 1; i <7; i++){Serial.print("Face " + String(i) + " status: "); Serial.println(connected_faces[i]);}
  for(int face = 0; face < 7; face++)
  { 
    // Step 1: Start for loop over faces to TRANSMIT... Update ROS things...
    if(zero_faces[face] == 1)
      {
        nh.spinOnce(); publishAwake();
        delay(random(10,80));
        int ir_val[7][7] = {{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0}}; // Face[1][prev.1.prev.2.3.4.5
        // Step 2: Start 60ms Loop:
        for(int i = 0; i < 13; i++)
          {
   // Blink correct part of message...
            if(i%2 == 1){face_rgb(face, 1, 0, 1, 0); IRLED(face,1,1,1,1);} else {rgboff(); IRLED_OFF();} // Blinks IR LED's on "face" face
   // Update ir_values_(faces on zero list)
            delay(5);
            for(int j = 1; j < 7; j++)
            {
            int readinn = read_ambient(j);
            //if(zero_faces[j] == 1){face_rgb(j, 1, 0, 0, 0);
            if(readinn > 1){{ir_val[j][0] = 1;} if(j == face && i%2 == 1){ir_val[j][0] = -1;}}
            else if(readinn <= 1) {ir_val[j][0] = 0;}
            else {ir_val[j][0] = -1;}
            
            } // Updates all faces item 0
   // For all faces... Check to see if we see THE pattern... 
            
            for(int i = 1; i < 7; i++)
            {if(ir_val[i][0] == 0 && ir_val[i][1] == 1 && ir_val[i][2] == 0 && ir_val[i][3] == 1 && ir_val[i][4] == 0 && ir_val[i][5] == 1)
              {connected_faces[i] += 1;}}
   // For faces on connection_list ... also check to see if they are all high... if so remove from list...
             for(int i = 1; i < 7; i++)
            {if(ir_val[i][0] == 1 && ir_val[i][1] == 1 && ir_val[i][2] == 1 && ir_val[i][3] == 1 && ir_val[i][4] == 1 && ir_val[i][5] == 1)
              {connected_faces[i] = 0;}}
   // Shift all values one to the left...
            for(int i = 1; i < 7; i++)
            {
              ir_val[i][5] = ir_val[i][4];
              ir_val[i][4] = ir_val[i][3];
              ir_val[i][3] = ir_val[i][2];
              ir_val[i][2] = ir_val[i][1];
              ir_val[i][1] = ir_val[i][0];
            }
            delay(50);
         }
      }
   }

  cmd_previous = cmd;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ///////////////////////
  //i r l e d _ 1 _ 1  0  1 0
  // 0 1 2 3 4 5 6 7 8 9 10 11 12
if(cmd.substring(0,5) == "irled") // This convoluted thing allows us to send IR LED commands... it parses the state
  { 
    if(cmd.substring(6) == "off"){delay(15);IRLED_OFF();delay(15);IRLED_OFF();cmd = "chill";}
    else
    {
    String temp_face = cmd.substring(6,7);
    //face_rgb(temp_face.toInt(), 1, 1, 1, 0);
    String LED1 = cmd.substring(8,9); String LED2 = cmd.substring(9,10); String LED3 = cmd.substring(10,11); String LED4 = cmd.substring(11,12);
    IRLED(temp_face.toInt(),LED1.toInt(),LED2.toInt(),LED3.toInt(),LED4.toInt()); 
    //delay(30);rgboff();
    cmd = "chill";
    }
  }

  
  if (millis() > publisher_timer) // This thing tries to runs every 
  { 
      is_frame_connected = is_i2c_connected();
      if(loop_counter % 114 == 0){read_accel_central();}
      if(loop_counter % 151 == 0){get_battery_voltage();}
      if(loop_counter % 10 == 0){which_plane_fast();}
      if(loop_counter % 11 == 0){which_face_is_forward(which_plane_fast());}
      if(loop_counter % 13 == 0){publishstatus();}
      ////////////////////////////////////////////////////
      ////////////////////////////////////////////////////
      if(loop_counter % 3 == 0 && cmd == "light")
      {
      light_track_update();
      }
      ////////////////////////////////////////////////////
      ////////////////////////////////////////////////////
      //if(loop_counter % 2 == 0){face_rgb(which_face_is_up(12000),1,0,1,0);}
      //else{face_rgb(which_face_is_forward(which_plane_fast()),0,1,0,0);}
      loop_counter++;
      publishAwake();  
////////////////////////////////////////
      publisher_timer = millis() + 500;
  }
  
nh.spinOnce();
if(DEBUG){Serial.println("SPIN ONCE");}
}
