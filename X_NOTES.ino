// Notes
//const char WiFiSSID[] = "TP-LINK_9B88A0"; // Sebastian's router
//const char WiFiPSK[] = "279B88A0";// Sebastian's router

// Format for sensor values Message
// U_int16 array
////////////////////////////////////
//
//
// Cube ID 0xVVV,                             State[0]
// Face1_light,                               State[1]
// Face2_light,                               State[2]
// Face3_light,                               State[3] 
// Face4_light,                               State[4]
// Face5_light,                               State[5] 
// Face6_light,                               State[6]
// Magnet sensor Reading                      State[7]                    (1234, 1536, 2546 or 0000) with 0000 meaning undetermined or error
// Grav_face1_x,                              State[8]
// Grav_face1_y,                              State[9]
// Grav_face1_z,                              State[10]
// Gyro_face1_x,                              State[11]
// Gyro_face1_y,                              State[12]
// Gyro_face1_z,                              State[13]
// TEMP_ACCEL   ,                             State[14] 
// ALS_parasite,                              State[15]        
// Altitude_parasite,                         State[16]                     Value in mm, zeroed on initial setup
//
//
//    while (Serial.available() > 0) // while there are things in the serial buffer...
//  {
//      char c = Serial.read();
//      if (c != '\n') 
//    {
//        ser_str += String(c);
//    } else 
//    {
//        serial_msg.data = ser_str.c_str();
//        pub_ser.publish(&serial_msg);
//        ser_str = "";
//    }
//  }
