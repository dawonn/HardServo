// Sweep Example

#include <HardServo.h> 
 
HardServo myservo[6];  // create servo objects for each servo
 
 
void setup() 
{ 
  // Attaching a servo to a pin also 'consumes' the attached timer
  // by configuring it for a 50Hz rate (20ms period). The same timer
  // can, or course, control multiple servos but may interfere with
  // other software libraries; Coder Beware. ;)

  myservo[0].attach(5);  // 16 bit (Timer 3)
  myservo[1].attach(6);  // 10 bit (Timer 4)
  myservo[2].attach(9);  // 16 bit (Timer 1)
  myservo[3].attach(10); // 16 bit (Timer 1)
  myservo[4].attach(11); // 16 bit (Timer 1)
  myservo[5].attach(13); // 10 bit (Timer 4)
  
} 
 
 
void loop() 
{ 
  float min_angle = 40;
  float max_angle = 140;
  float sweep_step = 0.1;
  int sweep_delay = 5;
  
  // Sweep up
  for(float pos = min_angle; pos < max_angle; pos += sweep_step)
  { 
    // Command all servos to go to position in variable 'pos' 
    for(int i=0; i<6; i++)
      myservo[i].write(pos);         
    
    delay(sweep_delay);                      
  } 
  
  // Sweep down
  for(float pos = max_angle; pos>=min_angle; pos -= sweep_step)
  {                                
    // Command all servos to go to position in variable 'pos' 
    for(int i=0; i<6; i++)
      myservo[i].write(pos);

    delay(sweep_delay); 
  } 
} 
