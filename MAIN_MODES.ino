  
  void light_track_part_2() 
  {
    light_track_update_part2();
  }

  void light_track_update_part_2() {light_track_update_part2();}

  void roll_to_plane()
  {
    if(ambient_values[face_that_is_up][0] > 0)
    {
           if(most_recent_traverse_result == 180 )   {cmd = "g"; demo = "nothing";}
      else if(attempts_traverse > 5)                 {cmd = "r"; demo = "nothing";}
      else if(most_recent_traverse_result == 90)     {Serial.println("stillalive"); most_recent_traverse_result = move_normal("f","90 deg",6000, 24, 12,"e 10", 4000);attempts_traverse++;}
      else if(most_recent_traverse_result == 0)      {Serial.println("stillalive"); most_recent_traverse_result = move_normal("f","90 deg",6000, 24, 12,"e 10", 4000);attempts_traverse++;}
    }
    else if(ambient_values[face_that_is_up][0])   {y_counter = 8;}
  }
  
  void ready_set_jump()
  {
    if(ambient_values[face_that_is_up][0] > 0)
    {
      ros_safe_delay(1000);
      move_normal("f","90 deg",6000, 24, 12,"e 10", 3500);
      ros_safe_delay(600);
      Serial.println("stillalive"); 
      move_normal("f","90 deg",14000, 34, 22,"e 10", 4000);
      ros_safe_delay(1200);
    }
  }
  
