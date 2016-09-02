//ros::Time time = ros::Time::now();

void ros_safe_delay(int delay_time)
{
for(int i = 0;i < delay_time/200; i++){delay(200);nh.spinOnce();publishAwake();} 
}

void publishAwake()
{
  awake_msg.data = IDnumber;
  pub_awake.publish(&awake_msg);
}
void publishupdate()
{ 
  if(DEBUG){Serial.println("Publishing things");}
  fbrxen();
  delay(2);
  int Status[13];
  for(int x = 2; x <= 7; x++){
    Status[x] = read_ambient(x-1);
  }
  read_accel();
  Status[0] = IDnumber;
  Status[1] = millis();//int(nh.now()); 
  Status[8] = which_face_is_up(12000);
  Status[9] = forward_face;
  Status[10] = which_plane_am_i_in;
  Status[11] = readMagnet();
  Status[12] = vbat_average;
  
  update_msg.data = Status;
  pub_update.publish(&update_msg);
}
////
void publishstatus()
{ 
  if(DEBUG){Serial.println("Publishing things");}
  fbrxen();
  delay(2);
  int Status[13];
  for(int x = 2; x <= 7; x++){
    Status[x] = read_ambient(x-1);
  }
  read_accel();
  Status[0] = IDnumber;
  Status[1] = millis();//int(nh.now()); 
  Status[8] = which_face_is_up(12000);
  Status[9] = forward_face;
  Status[10] = which_plane_am_i_in;
  Status[11] = readMagnet();
  Status[12] = vbat_average;

  status_msg.data = Status;
  pub_status.publish(&status_msg);
}

void init_nh() // Begins ROS bs
{ 
  if(DEBUG){Serial.println("Beginning init_nh");}
  
  if(which_alternate){nh.initNode("192.168.1.100");}
  else{nh.initNode("192.168.0.100");}
  
  if(DEBUG){Serial.println("Made it past initNode");}
  status_msg.data_length=13;
  update_msg.data_length=13;
  nh.subscribe(sub_cmd);
  nh.subscribe(sub_general);
  nh.advertise(pub_status);
  nh.advertise(pub_update);
  nh.advertise(pub_awake);
  nh.advertise(pub_ser);
  if(DEBUG){Serial.println("ending init_nh");}
}
