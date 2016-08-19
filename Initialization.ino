void setup() 
{
  pinMode(Switch, OUTPUT);
  pinMode(LED, OUTPUT); 
  digitalWrite(LED, HIGH);
  delay(100);
  digitalWrite(LED, LOW);
  Serial.begin(115200);
  Wire.begin(2, 14);
  delay(100);
  //baro.begin();
  // Eveuntually get average from BARO - long initial_baro;
  setupWiFi();
  delay(100);
  init_nh();
  delay(100);
  setupSensors();
  delay(100);
  look_up_offset_values();  
}
//ID_numbers[30]
//int calibrations_forward[30]
//int calibrations_reverse[30]
//int forward_offset = 0;
//int reverse_offset = 0;
//IDnumber
void look_up_offset_values()
{
  int index = 0;
  for(int x = 0; x < how_many_cubes; x++)
  {
    if(ID_numbers[x] == IDnumber)
      {
      //Serial.print(ID_numbers[x],HEX);
      index = x;
      break;
      }
  }
  
  forward_offset      = calibrations_forward[index];
  special_offset      = calibrations_special[index];
  reverse_offset      = calibrations_reverse[index];
  plane_change_offset = calibrations_plane__[index];
}
void setupWiFi()
{
  WiFi.mode(WIFI_STA);
  int begin_time = millis();
  while (status != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to SSID: ");
    //Serial.println(WiFiSSID);
    status = WiFi.begin(WiFiSSID, WiFiPSK);
    delay(200);
    int current_time = millis();
    if((current_time - begin_time) > 10000)
        {
          delay(40);
          break;
        }
//        Serial.println("fbsleep");
//        delay(200);
//        Serial.println("fbsleep");
//        delay(200);
//        Serial.println("fbsleep");
//        delay(200);
//        Serial.println("espoff");
//        delay(2000);
//        Serial.println("espoff");
//        delay(2000);
        
  }
  //Serial.print("Escaped first loop");
  if(status != WL_CONNECTED) 
  {
    setupWiFi_alternate();
    //Serial.print("Connected to WiFi: ");
    //Serial.println(WiFiSSID_alternate);
    }
  else{
  //Serial.print("Connected to WiFi: ");
  //Serial.println(WiFiSSID);
  }
  ip_address = WiFi.localIP();
  WiFi.setAutoReconnect(true);
  //Serial.print("IP Address: ");
  //Serial.println(ip_address);
}

void setupWiFi_alternate()
{
// Serial.println("Beginning wifi Alternate");
//  WiFi.mode(WIFI_STA);
  long start_time = millis();
  while (status != WL_CONNECTED)
  {
    //Serial.print("Attempting to connect to SSID: ");
    //Serial.println(WiFiSSID);
    status = WiFi.begin(WiFiSSID_alternate, WiFiPSK_alternate);
    delay(200);
    if((millis() - start_time) > 10000){
        Serial.println("fbsleep");
        delay(200);
        Serial.println("fbsleep");
        delay(200);
        Serial.println("fbsleep");
        delay(200);
        Serial.println("espoff");
        delay(2000);
        Serial.println("espoff");
        delay(200);}
  }
}

void setupSensors()
{
  randomSeed(analogRead(A0));
  if(DEBUG){Serial.println("Entering setupSensors");}
    pinMode(Switch, OUTPUT);
//  sensor.init();
//  sensor.configureDefault();
//  sensor.setTimeout(500);
  fbrxen();
  if(DEBUG){Serial.println("Leaving Setupsensors");}
}
