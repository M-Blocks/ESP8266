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
  int Status[19];
  for(int x = 2; x <= 7; x++){
    Status[x] = read_ambient(x-1);
  }
  read_accel();
  Status[0] = IDnumber;
  Status[1] = millis();//int(nh.now()); 
  Status[8] = face_that_is_up;
  Status[9] = forward_face;
  Status[10] = which_plane_am_i_in;
  Status[11] = readMagnet();
  Status[12] = vbat_average;
  Status[13] = connected_faces[1];
  Status[14] = connected_faces[2];
  Status[15] = connected_faces[3];
  Status[16] = connected_faces[4];
  Status[17] = connected_faces[5];
  Status[18] = connected_faces[6];
//  Status[10] = AcX;
//  Status[11] = AcY;
//  Status[12] = AcZ;
//  Status[13] = AcX_c;
//  Status[14] = AcY_c;
//  Status[15] = AcZ_c;
//  Status[14] = Tmp;
//  Status[14] = sensor.readAmbientSingle(); // Ambient light sensor on VL6180
  update_msg.data = Status;
  pub_update.publish(&update_msg);
}

void publishstatus()
{ 
  if(DEBUG){Serial.println("Publishing things");}
  fbrxen();
  delay(2);
  int Status[19];
  for(int x = 2; x <= 7; x++){
    Status[x] = read_ambient(x-1);
  }
  read_accel();
  Status[0] = IDnumber;
  Status[1] = millis();//int(nh.now()); 
  Status[8] = face_that_is_up;
  Status[9] = forward_face;
  Status[10] = which_plane_am_i_in;
  Status[11] = readMagnet();
  Status[12] = vbat_average;
  Status[13] = connected_faces[1];
  Status[14] = connected_faces[2];
  Status[15] = connected_faces[3];
  Status[16] = connected_faces[4];
  Status[17] = connected_faces[5];
  Status[18] = connected_faces[6];
//  Status[10] = AcX;
//  Status[11] = AcY;
//  Status[12] = AcZ;
//  Status[13] = AcX_c;
//  Status[14] = AcY_c;
//  Status[15] = AcZ_c;
//  Status[14] = Tmp;
//  Status[14] = sensor.readAmbientSingle(); // Ambient light sensor on VL6180
  status_msg.data = Status;
  pub_status.publish(&status_msg);
}

void init_nh() // Begins ROS bs
{ 
  if(DEBUG){Serial.println("Beginning init_nh");}
  nh.initNode("192.168.0.100");
  if(DEBUG){Serial.println("Made it past initNode");}
  nh.subscribe(sub_cmd);
  nh.subscribe(sub_general);
  nh.advertise(pub_status);
  nh.advertise(pub_update);
  status_msg.data_length=19;
  nh.advertise(pub_awake);
  nh.advertise(pub_ser);
  if(DEBUG){Serial.println("ending init_nh");}
}
