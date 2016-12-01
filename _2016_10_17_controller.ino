////////////////// 2016-11-04_17-44-53***
//////////////////
#define DEBUG                               0 // Setting this to 1 generates lots of debug messages
#define DEBUG_2                             0
#define IS_ROS_ON_AT_BEGIN                  1
#define COLOR_MENU_ENABLED                  0
#define SIMPLE_TEST_LOOP_ENABLED            0
////////////////// Commonly Toggled
#define MESSAGE_THRESHOLD                   5
#define STANDARD_MESSAGE_LENGTH             7
#define USE_WIFICON
#define LED                                 13
#define Switch                              12
#define ACTIVE_FACES                        7 // 5 = 1 2 3 4,    7 = 1 2 3 4 5 6
#define FACES                               7
#define MPU_addr                            0x69  // I2C address of the MPU-6050
#define ARRAY_LENGTH_LONG                   22
#define ARRAY_LENGTH_SHORT                  10
//////////////////
#include <ArduinoHardware.h>
#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Int32MultiArray.h>
#include <std_msgs/MultiArrayLayout.h>
#include <std_msgs/MultiArrayDimension.h>
#include <ESP8266WiFi.h>
#include <Wire.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////MAIN_LOOP VARIABLES////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int cube_id                 = 0;  
int is_ros_on               = 0;
int ambient_low             = 5;
int ambient_medium          = 14;
int ambient_high            = 70;
int ambient_very_high       = 125;
int long_message_length     = 10;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
long quick_timer            = 0;
int quick_timer_length      = 99; // in milliseconds
int x10xfast_loop_counter   = 0; // this counts 1...2....3....4..5...6.. every (quick_timer_length) ms
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
long slow_timer             = 0;
int slow_timer_length       = 980; // in milliseconds This sets how often the portion of the code is run
int loop_counter            = 0;
//////////////////////////////////////////////Important State Variables///////////////////////////////////////////////////////////////
String state                              = "chilling"; // also "being_shaken" "two_neighbors" "forever_alone" ...
String demo                               = "nothing"; // also... "dance_party" etc...
String cmd                                = "nothing";
String cmd_received                       = "nothing";
String cmd_previous                       = "chill";
String color                              = "nothing";
String light_tracking_internal_state      = "";
//////////Orientation Related///////////////
int which_plane_am_i_in                   = -1; // either 1234, 1536 or 2546
int forward_face                          = -1;
///////////////////////////////////////
// WIP
int face_that_is_up                       = -1; // face that is up lists most recent face that is point up... (big surprise there!)
int face_that_was_last_up                 = 0; // kinda like the two below...
int previous_up_face[ARRAY_LENGTH_LONG+1] = {9,10,11,12,13,14,15,16,17,18,19,20,14,15,13,15,13,13,13,13};
///////////////////////////////////////
int face_that_was_last_lit_up             = 0; // stupid BS to avoid blinking LED's... if this is the same as before it doesn't reset
int movement_ammount                      = 1000; 
int previous_movement_ammount             = 1000;
int sum_of_diff                           = 0;
int most_recent_traverse_result           = 0;
int attempts_traverse                     = 0;
int vbat[5]                               = {0,0,0,0,0}; // most recently updated battery voltage
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//int face_neighbors[FACES][6]
int n_cube_id       = 0;
int n_face_id       = 1;
int n_angle         = 2;
int n_time          = 3;
int n_certainty     = 4;
int n_num_messages  = 5;

int face_neighbors[FACES][7] = {
// ID            face          angle      time formed  certainty%  #of messages previous      
//"1-32" "1-6" "0-3"   "loop_counter_slow"  "1-100"       "1+"
//  [0]          [1]           [2]           [3]           [4]          [5]
    {0,           0,            0,            0,            0,           0}, // IGNORE THIS LINE this is so indexes start at 1
    {0,           0,            0,            0,            0,           0}, // FACE 1
    {0,           0,            0,            0,            0,           0}, // FACE 2
    {0,           0,            0,            0,            0,           0}, // FACE 3
    {0,           0,            0,            0,            0,           0}, // FACE 4
    {0,           0,            0,            0,            0,           0}, // FACE 5
    {0,           0,            0,            0,            0,           0}};// FACE 6
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////CALIBRATION VARIABLES//////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Calibration values are an offset to the current of the braking attached to each specific cube
// 100 indicates that the current in each _Move_ command will be the average, less then 100
// will apply less braking current, and over 100 applies more. 110 is 10% more braking power
#define how_many_cubes 32
//                                           [0]     [1]       [2]        [3]        [4]       [5]       [6]       [7]      [8]      [9]       [10]      [11]        [12]       [13]    [14]    [15]    [16]    [17]    [18]    [19]     [20]    [21]      [22]     [23]     [24]     [25]     [26]     [27]     [28]            [29]       [30]       [31]
int ID_numbers[how_many_cubes] =             {0x01234 , 0xe58f5b, 0x9d5b8f, 0xe58ec1, 0xe58d94, 0x9f06eb, 0xe58f6d, 0x8a2aad, 0xea8ad, 0x9f06eb, 0xEA8ED , 0xE58F4A  , 0x0000   , 0     , 0     , 0     , 0     , 0     , 0     , 0      , 0      , 0       , 0     , 0      , 0      , 0      , 0      , 0      , 0x8AA7CF      , 0xCC156D , 0x8AA9E3 , 0       };
int calibrations_forward[how_many_cubes]   = {100     , 60      , 70      , 115     , 90      , 135     , 100     , 120     , 100    , 150     , 110     , 100       , 105      , 100   , 100   , 100   , 100   , 100   , 100   , 100    , 100    , 100     , 100   , 100    , 100    , 100    , 100    , 100    , 100           , 100      , 100      , 100     };
int calibrations_reverse[how_many_cubes]   = {100     , 70      , 105     , 100     , 100     , 120     , 100     , 222     , 100    , 135     , 100     , 100       , 115      , 100   , 100   , 100   , 100   , 100   , 100   , 100    , 100    , 100     , 100   , 100    , 100    , 100    , 100    , 100    , 100           , 100      , 100      , 100     };
///// Special_offset adds time to brake in ms//////
int calibrations_special[how_many_cubes]   = {0       , 0       , 0       , 0       , 0       , 0       , 0       , 0       , 0      , 4       , 0       , 0         , 0        , 0, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};
int calibrations_plane__[how_many_cubes]   = {0       , 0       , 0       , 0       , 0       , 0       , 0       , 0       , 100    , 0       , 0       , 0         , 170      , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int maximum_acceleration[how_many_cubes]   = {0       , 6000    , 6000    , 6000    , 6000    , 6000    , 6000    , 6000    , 6000   , 6000    , 6000    ,6000       , 6000     , 6000, 6000, 6000, 6000, 6000, 6000, 6000, 6000, 6000, 6000, 6000, 6000};
int forward_offset = 100; // These are the defaults if there isn't an entry for the specific Cube's ID_number in the table.
int reverse_offset = 100;// These are the defaults if there isn't an entry for the specific Cube's ID_number in the table.
int special_offset = 0;// These are the defaults if there isn't an entry for the specific Cube's ID_number in the table.
int plane_change_offset = 0;// These are the defaults if there isn't an entry for the specific Cube's ID_number in the table.
int max_acceleration = 6000;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////Communication Variables////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int i_just_sent_an_ir_message[FACES] =        {0, 0, 0, 0, 0, 0, 0};
int message_location_blink_counter[FACES] =   {0, 0, 0, 0, 0, 0, 0};
int signature_blink_array[FACES] =            {0, 0, 0, 0, 0, 0, 0};
int message_general_counter[FACES] =          {0, 0, 0, 0, 0, 0, 0};
int ir_face_buffer_previous[FACES] =          {0, 0, 0, 0, 0, 0, 0};
int random_counter[FACES] =                   {0, 0, 0, 0, 0, 0, 0};
int received_message_count[FACES] =           {0, 0, 0, 0, 0, 0, 0};

int detected_special_blink_recently[FACES] =  {0, 0, 0, 0, 0, 0, 0};
int detected_signature_recently[FACES] =      {0, 0, 0, 0, 0, 0, 0};
int bit_1_detected[FACES] =                   {0, 0, 0, 0, 0, 0, 0};
int bit_2_detected[FACES] =                   {0, 0, 0, 0, 0, 0, 0};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int const three_pulse_length = 6;
int three_pulse[three_pulse_length] = {(-ambient_low),  ambient_high,  ambient_high,  ambient_high,  0, (-ambient_low)};

char temp_message[ARRAY_LENGTH_LONG+1];

char M_L = 'L'; // l = location
char M_H = 'H'; // h = hey message
char M_C = 'C'; // c = color
char M_M = 'M'; // m = motion message
char M_S = 'S'; // s = special message // signed
char M_D = 'E'; // ERROR - no message
char M_J = 'J'; // Jump Command
// color commands
char M_R = 'R';  
char M_G = 'G'; 
char M_B = 'B'; 
char M_P = 'P'; 
char M_Y = 'Y';
char M_T = 'T'; 
//

char temp_message_type        = 'E'; 
int  temp_message_value       = 0;
char temp_message_char        = 'n';
int  message_info_begin_index = 2;
int  char_position_offset     = 2;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// index                                            [0]     [1]     [2]         [3]         [4]       [5]     [6]    [7]    [8]    [9]
//                                                 type    type     (value[1])  (value[2])  char    type   type
char location_message[STANDARD_MESSAGE_LENGTH] =   {M_L,    M_L,    '1',        '9',        'a',      M_L,    M_L}; // do not pass this message on...
char hey_message[STANDARD_MESSAGE_LENGTH] =        {M_H,    M_H,    '1',        '4',        'h',      M_H,    M_H}; // pass these on by default
char color_message[STANDARD_MESSAGE_LENGTH] =      {M_C,    M_C,    '5',        '5',        'y',      M_C,    M_C}; // if message ID is > 50 - pass it on to all friends...
char motion_message[STANDARD_MESSAGE_LENGTH] =     {M_M,    M_M,    '6',        '9',        't',      M_M,    M_M}; // These are not passed on by default
char special_message[STANDARD_MESSAGE_LENGTH] =    {M_S,    M_S,    '6',        '6',        'g',      M_S,    M_S}; // to be defined

char synchronise_move[STANDARD_MESSAGE_LENGTH] =   {M_J,    M_J,    '6',        '6',        'g',      M_J,    M_J}; // to be defined
//                                                                                    f/r
//Receiving Cube                     //   bldcspeed "char[f/r]"  5+(value[1]) RPM .... Wait for Rising Edge... 
//Sending   Cube                     //   send Synchronise... (do bldcspeed self...)   Wait (time limit)               

char color_message_short[STANDARD_MESSAGE_LENGTH]= {'G',    'G',    'G',        'G'};
char temp_active_message[STANDARD_MESSAGE_LENGTH]= {M_C,    M_C,    '5',        '5',        'y',      M_C,    M_C};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////COUNTERS////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int change_plane_counter = 0;
int move_counter = 0;
int light_tracking_delay_counter = 0;
int light_off_timer = 0;

int r_counter = 0;
int g_counter = 0;
int b_counter = 0;
int p_counter = 0;
int y_counter = 0;
int t_counter = 0;
int w_counter = 0;
int ir_counter[FACES] =  {0,0,0,0,0,0,0};
int blink_timer[FACES] = {0,0,0,0,0,0,0};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// SENSOR VARIABLES /////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int ambient_values[FACES][ARRAY_LENGTH_LONG+1]; 
//= {Recent[0] ----->------>---- History[20]
//  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // IGNORE THIS LINE this is so indexes start at 1
//  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // FACE 1
//  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // FACE 2
//  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // FACE 3
//  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // FACE 4
//  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // FACE 5
//  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};// FACE 6 
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ, u, AcX_c, AcY_c, AcZ_c, AcX_c_old, AcY_c_old, AcZ_c_old; // Variables from all of the accelerometer/ gyros
int16_t motion_sum_log[ARRAY_LENGTH_LONG+1] = {15,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,1,1,1};
int16_t GyX_log[ARRAY_LENGTH_SHORT+1] = {0,0,0,0,0,0,0,0,0,0};
int16_t GyZ_log[ARRAY_LENGTH_SHORT+1] = {0,0,0,0,0,0,0,0,0,0};
int16_t GyY_log[ARRAY_LENGTH_SHORT+1] = {0,0,0,0,0,0,0,0,0,0};
int16_t AcX_log[ARRAY_LENGTH_SHORT+1] = {0,0,0,0,0,0,0,0,0,0};
int16_t AcY_log[ARRAY_LENGTH_SHORT+1] = {0,0,0,0,0,0,0,0,0,0};
int16_t AcZ_log[ARRAY_LENGTH_SHORT+1] = {0,0,0,0,0,0,0,0,0,0};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////WIFI VARIABLES//////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int which_alternate             = 1; // This sets the right IP address to correspond to the router
const char WiFiSSID[]           = "mblocks"; // Router used with the LINUX laptop
const char WiFiPSK[]            = "password"; // My Router
const char WiFiSSID_alternate[] = "TP-LINK_9B7022"; // Back_Up_Router
const char WiFiPSK_alternate[]  = "279B7022"; // Back_Up_Rou
int status                      = WL_IDLE_STATUS;
IPAddress ip_address;

long IDnumber = ESP.getChipId();

//int  idnum    = look_up_offset_values();

String ID       = String(IDnumber, HEX); // Converts this to HEX

String STATE    = "cube_"+ID+"/status";
String AWAKE    = "awake";
String GENERAL  = "general";
String NEIGHBOR = "neighbors";
String CERIAL   = "cube_"+ID+"/serial";
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////ROS VARIABLES/////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int neighbor_message_number = 42;

//////
String ser_str; // Used in ROS parts

ros::NodeHandle nh;

void commandIACb   (const std_msgs::String & cmd_msg)      {String ss = cmd_msg.data;     cmd = ss;                                 Serial.println(ss);} // Cube Specific Channel
void commandGENERAL(const std_msgs::String & general_msg)  {String ss = general_msg.data; cmd_received = ss; process_general_ros(); Serial.println(ss);} // General Topic

std_msgs::Int32MultiArray   neighbor_msg;
std_msgs::Int32MultiArray   status_msg;
std_msgs::Int32             awake_msg;
std_msgs::Int32             als_msg;
std_msgs::String            serial_msg;

ros::Publisher pub_neighbor(  NEIGHBOR.c_str(), &neighbor_msg);
ros::Publisher pub_status(    STATE.c_str(),    &status_msg);
ros::Publisher pub_awake(     AWAKE.c_str(),    &awake_msg);
ros::Publisher pub_ser(       CERIAL.c_str(),   &serial_msg);

ros::Subscriber<std_msgs::String> sub_general("general", &commandGENERAL);
ros::Subscriber<std_msgs::String> sub_cmd(CERIAL.c_str(), &commandIACb);
