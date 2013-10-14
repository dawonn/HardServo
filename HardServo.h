#ifndef HardServo_h
#define HardServo_h

/*
  HardServo.h - Hardware Servo Timer Library

  Author: Dereck Wonnacott <dereck@gmail.com>
  Copyright (c) 2013 Dereck Wonnacott.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <Arduino.h>
#include <inttypes.h>

class HardServo
{
  private:
    uint8_t _pin;
    float _angle;
    float _min_angle;
    float _max_angle;
    float _min_uS;
    float _max_uS;
    
    void attach_servo_TC1();
    void attach_servo_TC3();
    void attach_servo_TC4();

  public:
    HardServo();
    
    // attach to a pin, sets pinMode, returns 0 on failure, won't
    // position the servo until a subsequent write() happens
    uint8_t attach(uint8_t);
    
    // min_angle: max angle in degrees
    // max_angle: min angle in degrees
    // min_uS: pulse length for min_angle in microseconds
    // max_uS: pulse length for max_angle in microseconds
    uint8_t attach(uint8_t pin, float min_angle, float max_angle, 
                                float min_uS   , float max_uS);
    
    void detach();
    void write(float angle);          // specify the angle in degrees, 0 to 180
    void writeMicroseconds(float uS);
    uint8_t read();
    uint8_t attached();
};

#endif
