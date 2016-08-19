////////////////// 2016-08-19
#define DEBUG 0 // Setting this to 1 generates lots of debug messages
#define USE_WIFICON
#define LED 13
#define Switch 12
#define how_many_cubes 30
//////////////////
#include <ArduinoHardware.h>
#include <ros.h>
#include <mblocks_wifi/Status.h>
#include <std_msgs/String.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Int32MultiArray.h>
#include <std_msgs/MultiArrayLayout.h>
#include <std_msgs/MultiArrayDimension.h>
//#include <std_msgs/Float32.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
//#include <VL6180X.h>
#include <ros/time.h>
#include <Adafruit_MPL3115A2.h>
//////////////////////
// Global variables //
//////////////////////
//  8A2AAD
// Calibration values are an offset to the current of the braking attached to each specific cube
// 100 indicates that the current in each _Move_ command will be the avera, less then 100
// will apply less braking current, and over 100 applies more.
//                                                    red bolts 
//                                                    dirty bolts         red and bk
/// ID_numbers[how_many_cubes] =             {DONE----, DONE----, 0x9d5b8f, 0xe58ec1, 0xe58d94, 0x9f06eb, 0xe58f6d, SPARKFUN, 0,    0,        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int ID_numbers[how_many_cubes] =             {0xe58f4a, 0xe58f5b, 0x9d5b8f, 0xe58ec1, 0xe58d94, 0x9f06eb, 0xe58f6d, 0x8a2aad, 0xea8ad, 0x9f06eb, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int calibrations_forward[how_many_cubes]   = {105     , 60      , 95     , 115     , 90      , 135      , 100     , 120     , 100    , 150     , 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};
///// Special_offset adds time to brake in ms//////
int calibrations_special[how_many_cubes]   = {0       , 0       , 0       , 0       , 0       , 0       , 0       , 0       , 0      , 4       , 0, 0, 0, 0, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};
/////
int calibrations_reverse[how_many_cubes]   = {115     , 70      , 105     , 100     , 100     , 120     , 100     , 222     , 100    , 135     , 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};
int calibrations_plane__[how_many_cubes]   = {170     , 0       , 0       , 0       , 0       , 0       , 0       , 0       , 100    , 0       , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int forward_offset = 100; 
int reverse_offset = 100;
int special_offset = 0;
int plane_change_offset = 0;
//////////////////////
//TESTING//
int movement_ammount = 1000;
int previous_movement_ammount = 1000;
int sum_of_diff = 0;
/////////////////////
int zero_faces[7] = {1,0,0,0,0,0,0};
int connected_faces[7] = {0,0,0,0,0,0,0};
/////////////////////
String ser_str;
int count_er = 1;
long publisher_timer;
const int MPU_addr=0x69;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ, u, AcX_c, AcY_c, AcZ_c, AcX_c_old, AcY_c_old, AcZ_c_old; // Variables from all of the accelerometer/ gyros
bool cmd_ready = true;
long startTime;
long stopTime;
long myDesiredTime;
long initial_baro; // this will be the calibrated ofset for the barometer
String cmd = "";
String cmd_previous = "";
int face_that_was_last_up = 0; // kinda like the two below...
int face_that_was_last_lit_up = 0; // stupid BS to avoid blinking LED's... if this is the same as before it doesn't reset
int face_that_is_up = -1; // face that is up lists most recent face that is point up... (big surprise there!)
int forward_face = -1;
int central_orientation = -1;
int vbat1 = 0; // most recently updated battery voltage
int vbat2 = 0;
int vbat3 = 0;
int vbat4 = 0;
int vbat_average = 0;
int loop_counter = 0;
int fast_loop_counter = 0;
int which_plane_am_i_in = -1;
int fblight[6] = {-1,-1,-1,-1,-1,-1};
int Status[14];
int is_frame_connected = 0;
//VL6180X sensor;
Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();
//////////////////////
// WiFi Definitions //
//////////////////////
const char WiFiSSID[] = "TP-LINK_9B7022"; // My Router
const char WiFiPSK[] = "279B7022"; // My Router
const char WiFiSSID_alternate[] = "TP-LINK_9B6D90"; // Back_Up_Router
const char WiFiPSK_alternate[] = "279B6D90"; // Back_Up_Router
int status = WL_IDLE_STATUS;
IPAddress ip_address;
//////////////////////
// ROS Definitions //
//////////////////////
//unsigned long IDnumber = __builtin_bswap32(ESP.getChipId()); // Gets the chip mac address
long IDnumber = ESP.getChipId();

String ID = String(IDnumber, HEX); // Converts this to HEX
//String ID4 = ID.substring(0); // all characters...
//////////////////////////////////////////////////////////////////
// Unique Names for each Cube's Topic Advertisements//////////////
//////////////////////////////////////////////////////////////////
String STATE = "cube_"+ID+"/status";
String AWAKE = "awake";
String CERIAL = "cube_"+ID+"/serial";
String UPDATE = "cube_"+ID+"/update";
//////////////////////
// ROS Definitions //
//////////////////////
ros::NodeHandle nh;

void commandIACb(const std_msgs::String& cmd_msg) {
  String ss = cmd_msg.data;
  cmd = ss;
  Serial.println(ss);
}

void commandGENERAL(const std_msgs::String& general_msg) {
  String ss = general_msg.data;
  cmd = ss;
  Serial.println(ss);
}

void commandfn(const std_msgs::String& cmd_msg) {
  Serial.println(cmd_msg.data);
}

std_msgs::Int32MultiArray status_msg;
std_msgs::Int32MultiArray update_msg;
std_msgs::Int32 awake_msg;
std_msgs::Int32 als_msg;
std_msgs::String serial_msg;

ros::Publisher pub_status(STATE.c_str(), &status_msg);
ros::Publisher pub_update(UPDATE.c_str(), &update_msg);
ros::Publisher pub_awake(AWAKE.c_str(), &awake_msg);
ros::Publisher pub_ser(CERIAL.c_str(), &serial_msg);

ros::Subscriber<std_msgs::String> sub_general("general", &commandGENERAL);
ros::Subscriber<std_msgs::String> sub_cmd(CERIAL.c_str(), &commandIACb);
