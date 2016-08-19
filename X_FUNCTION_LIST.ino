/*
 * 
 * 
 * 1. Setup 
 *    a. Wifi Setup
 *    b. Ros Setup
 *    c. Sensors Setup
 * 
 * 2. Main Loop State Machine ... Does Ros things...
 *    States:
 *    1. Chilling - Publishes regular updates of sensors values// accepts short commands
 *          Listens to General ROS topic, accepts commands based on general or on cube specific topic
 *    
 *    2. Driving a car*
 *    
 *    3. Aggregate - Does not regularly update sensors... Internally evaluates sensor readings to move toward light
 *    
 *       Begin... Get 6 light readings... Check Current orientation (1234, 1536, 2546)
 *       Determine if free to move with fn::Wiggle (20% of the time do this 2x or stronger)
 *       Find Face with Highest value, above a threshold. 
 *            Determine if flywheel is currently aligned with this.
 *                If so - slowly roll fw or bkwards.
 *                if not - random num generator ? Wiggle : Jump
 *                
 *    4. Climb?
 *    5. Sleep?
 *    6. Reset?
 *    7. Demo? - hard programmed demo routine
 * 
 * 
 * 
 * 
 * 3. Short Commands
 *    1. Turn on/off IR LED 1....24
 *    2. Turn on RGB led 1-8 to R or G or B
 *    3. All RGB specific color
 *    4. Any IA command
 *    
 *    
 * 4. Longer Commands
 * 
 *    3. Wiggle_test
 *        Check gravity.
 *        Begin recording array of GYRO values
 *        Do a short, sharp Move
 *        IF (CP_state) is alligned with Ground
 *        
 *        ELSE
 *    
 *    
 *    
 *    4. IA_MOVE
 *        Updates internal 6 light readings, (1234, 1536, 2546) and 3x Gravity Readings.
 *        Determine expected sucessfull gravity readings from a Lookup Table.
 *        Attempt to move using (standard values*Specific Cube Calibration number).
 *            Checks to see if successful, unsucessfull, or WTF!! (look at gravity, maybe array of gyro readings)
 *            If successful - send sucess message to (ROS + Flash LED green)
 *            (If unsucessfull and fail counter <3) - Wait 1000ms, then try IA again with 1.(fail_Counter)*Current value 1.(fail_Counter)*Speed value
 *                  Increment fail_Counter.
 *            if WTF - check wiggle.
 *                 If attached, send message.
 *                 If Free - send message
 *                 <Break>
 *            If fail counter >3
 *                Turn LED's Red
 *                Send Message
 *                <break>
 * 
 *     5. CP(1234 (a) or 1536 (b), or 2546 (c))
 *         Does Wiggle Test (Connected or not?)
 *         if (Connected)
 *            Check Magnet readings...
 *            SMA retract(3000)
 *            Begin recording array of Magnet Values... Also Acceleromater... and Gyro Values
 *                IF (1234 to 1536...) Possible Transistions: (a)->(b), (b)->(c), (c)->(a), (b)->(a), (c)->(b), (a)->(c)
 *                      While (Not disloged) -- Accerate (or ebrake...) eneough to disloge with increasing power
 *                 Check magnet state... IF correct... way to go! 
 *                 <break>
 *                 
 *                 If not correct, either in (a), (b), (c)
 *                 If not check array readings to try to determine if in region between (a-b) (b-c) or (c-a)
 *                  
 *                 Choose reasonable value for FLYWHEEL
 *                 Loop back, and try again, increase SMA retract time
 *                 
 *         if (Not Connected)
 *                Same as above, but with lower power values.
 * 
 * 
 */
