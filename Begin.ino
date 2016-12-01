void setup() 
{ 
  things_to_do_once_as_program_starts(); 
  prepare_standard_messages();

  if(COLOR_MENU_ENABLED)                            {main_alt_choose_color_menu();}
  if(SIMPLE_TEST_LOOP_ENABLED)                      {simple_test_loop();}
  if(IS_ROS_ON_AT_BEGIN)                            {is_ros_on = 1;}
  if(is_ros_on)                                     {setupWiFi();delay(500);init_nh();}  // setup WIFI and ROS functionality
  
  Serial.println("BEGINNING MAIN LOOP!!!");
}

void simple_test_loop()
{
  int simple_loop_counter = 0;
  while(simple_loop_counter < 1000) //Serial.available() == 0)
  {
   Serial.println(readMagnet());
   simple_loop_counter++;
   delay(100);
   
//   if((simple_loop_counter+1) % 4 == 0)     {send_ir_message_string(1, hey_message);delay(200);fbrxflush(1);}
//   else if((simple_loop_counter+2) % 4 == 0){send_ir_message_string(2, hey_message);delay(500);fbrxflush(2);}
//   else if((simple_loop_counter+3) % 4 == 0){send_ir_message_char(3, hey_message, 4);delay(200);fbrxflush(3);}
//   else if((simple_loop_counter+4) % 4 == 0){send_ir_message_char(4, hey_message, 5);delay(200);fbrxflush(4);}
   if(simple_loop_counter > 10){rgboff(); }
  }
}

void things_to_do_once_as_program_starts()
{
  setupSensors_and_coms();
  look_up_offset_values();
  delay(1000); Serial.println("stillalive");// 
  randomSeed(analogRead(A0));
  delay(1000);Serial.println("stillalive");
  Serial.println("stillalive");
  delay(500);
  Serial.print("ID NUMBER IS: ");Serial.println(IDnumber);
  Serial.print("5_BIT ID NUMBER IS: ");Serial.println(cube_id);
  Serial.println("running...");
  if(get_vin() > 3400){turn_off_esp();}// This turns off ESP if we are on a charging pad 
}

int look_up_offset_values() // this function looks up the cube-specific offset values
{
  int index = 0;
  for(int x = 0; x < how_many_cubes; x++)
  {
    if(ID_numbers[x] == IDnumber)
      {
      index = x;
      break;
      }
  }
  forward_offset              = calibrations_forward[index];
  special_offset              = calibrations_special[index];
  reverse_offset              = calibrations_reverse[index];
  plane_change_offset         = calibrations_plane__[index];
  cube_id             = index;
  return(index);
}


void setupWiFi()
{
  WiFi.mode(WIFI_STA);
  int begin_time = millis();
  while (status != WL_CONNECTED)
  {
    if(DEBUG){Serial.print("Attempting to connect to SSID: "); Serial.println(WiFiSSID);}
    status = WiFi.begin(WiFiSSID, WiFiPSK);
    delay(200);
    int current_time = millis();
    if((current_time - begin_time) > 10000)
        {
          delay(40);
          break;
        }
  }
  if(DEBUG){Serial.println("Escaped first loop");}
  if(status != WL_CONNECTED && millis() - begin_time > 10000) 
  {
    setupWiFi_alternate();
    if(DEBUG){Serial.print("Connected to WiFi: ");}
    if(DEBUG){Serial.println(WiFiSSID_alternate);}
    }
  else{
  Serial.print("Connected to WiFi: ");
  Serial.println(WiFiSSID);
  }
  if(is_ros_on)
  {
  ip_address = WiFi.localIP();
  WiFi.setAutoReconnect(true);
  }
  if(DEBUG){Serial.print("IP Address: ");}
  if(DEBUG){Serial.println(ip_address);}
}

void setupWiFi_alternate()
{
 if(DEBUG){Serial.println("Beginning wifi Alternate");}
  which_alternate = 0;
  long start_time = millis();
  while (status != WL_CONNECTED && millis() - start_time < 17000)
  {
    if(DEBUG){Serial.print("Attempting to connect to SSID: "); Serial.println(WiFiSSID_alternate);}
    status = WiFi.begin(WiFiSSID_alternate, WiFiPSK_alternate);
    delay(200);
    if((millis() - start_time) > 16000){go_to_sleep();}
  }
}

void setupSensors_and_coms()
{
  pinMode(LED,    OUTPUT);
  pinMode(Switch, OUTPUT);
  
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW); 
  
  Serial.begin(115200); 
  Wire.begin(2, 14); // START i2c bus
 
  delay(500);
//  sensor.init();
//  sensor.configureDefault();
//  sensor.setTimeout(500);
  fbrxen(1);
}
