/*
  HardServo.h - Hardware Servo Timer Library for Leonardo boards

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

#include "HardServo.h"

HardServo::HardServo() 
{
  _pin       = 0;
  _angle     = -1;
  _min_angle = 0;
  _max_angle = 180;
  _min_uS    = 500;
  _max_uS    = 2500;
}

uint8_t HardServo::attach(uint8_t pin)
{
  return attach(pin, _min_angle, _max_angle, _min_uS, _max_uS);
}

uint8_t HardServo::attach(uint8_t pin, float min_angle, float max_angle,
                                       float min_uS,    float max_uS)
{
  // Detach existing connection if one exists
  if (_pin > 0) detach();
  
  // Set OC Behavior {Set on BOTTOM, Clear on MATCH, PWM-Mode}
  switch(pin)
  {
    case 5:                  // OC3A, !OC4A
      attach_servo_TC3();
      TCCR3A &= 0x3F;
      TCCR3A |= 0x80;
      break;
      
    case 6:                  // OC4D
      attach_servo_TC4();
      TCCR4C &= 0xF0;
      TCCR4C |= 0x09;
      break;
      
    case 9:                 // OC1A, !OC4B
      attach_servo_TC1();
      TCCR1A &= 0x3F;
      TCCR1A |= 0x80;
      break;
      
    case 10:                 // OC1B, OC4B
#ifdef PIN10_USE_TC4
      attach_servo_TC4();
      TCCR4A &= 0xCA;
      TCCR4A |= 0x21;
#else
      attach_servo_TC1();
      TCCR1A &= 0xCF;
      TCCR1A |= 0x20;
#endif
      break;
        
    case 11:                 // OC0A, OC1C
      attach_servo_TC1();
      TCCR1A &= 0xF3;
      TCCR1A |= 0x08;
      break;
      
    case 13:                 // OC4A
      attach_servo_TC4();
      TCCR4A &= 0x35;
      TCCR4A |= 0x82;
      break;
    default:
      return -1;
  }
  
  _pin = pin;
  _min_angle = min_angle;
  _max_angle = max_angle;
  _min_uS = min_uS;
  _max_uS = max_uS;
  
  pinMode(_pin, OUTPUT);
  return 0;
}

void HardServo::detach()
{
  writeMicroseconds(0);
  pinMode(_pin, INPUT);
  _pin = 0;
}

void HardServo::attach_servo_TC1() // 16 bit resolution
{
  // Set Fast PWM Mode
  TCCR1A &= 0xFC;
  TCCR1A |= 0x02;

  TCCR1B &= 0xD7;
  TCCR1B |= 0x18;
  
  // Set Prescaler - 32ms
  TCCR1B &= 0xF8;
  TCCR1B |= 0x02;
  
  // Set TOP value - 20ms
  ICR1 = 0x9FFF;
}

void HardServo::attach_servo_TC3() // 16 bit resolution
{
  // Set Fast PWM Mode
  TCCR3A &= 0xFC;
  TCCR3A |= 0x02;

  TCCR3B &= 0xD7;
  TCCR3B |= 0x18;
  
  // Set Prescaler - 32ms
  TCCR3B &= 0xF8;
  TCCR3B |= 0x02;
  
  // Set TOP value - 20ms
  ICR3 = 0x9FFF;
}

void HardServo::attach_servo_TC4()  // 10-bit resolution
{
  // Set Timer/Counter 4 prescaler (32ms)
  TCCR4B &= 0xF0;
  TCCR4B |= 0x09;
  
  // Set Timer4 TOP to 20ms
  TC4H  = 0x02;
  OCR4C = 0x80;  
  TC4H  = 0x00;
}

void HardServo::writeMicroseconds(float uS)
{
  const uint16_t counts_TC1_1ms = 0x0800;
  const uint16_t counts_TC3_1ms = 0x0800;
  const uint16_t counts_TC4_1ms = 0x0020;
  
  // Calibrate the bias of the system oscilator
  const float cpu_time_kludge = 1500.0/1536.0;
  
  if (uS < 0)     uS = 0;
  if (uS > 20000) uS = 20000;
  
  switch(_pin)
  {
    case 5:
      OCR3A = counts_TC3_1ms * (uS / 1000.0) * cpu_time_kludge;
      break;
    case 6:
      OCR4D = counts_TC4_1ms * (uS / 1000.0) * cpu_time_kludge;
      break;
    case 9:
      OCR1A = counts_TC1_1ms * (uS / 1000.0) * cpu_time_kludge;
      break;
    case 10:
#ifdef PIN10_USE_TC4
      OCR4B = counts_TC4_1ms * (uS / 1000.0) * cpu_time_kludge;
#else
      OCR1B = counts_TC1_1ms * (uS / 1000.0) * cpu_time_kludge;
#endif
      break;
    case 11:
      OCR1C = counts_TC1_1ms * (uS / 1000.0) * cpu_time_kludge;
      break;
    case 13:
      OCR4A = counts_TC4_1ms * (uS / 1000.0) * cpu_time_kludge;
      break;
    }
}

void HardServo::write(float angle)
{
  if (angle < _min_angle) angle = _min_angle;
  if (angle > _max_angle) angle = _max_angle;
  _angle = angle;
  
  float angle_percent = (_angle - _min_angle) / (_max_angle - _min_angle);
  
  writeMicroseconds( _min_uS + (_max_uS -_min_uS) * angle_percent );
}

uint8_t HardServo::read()
{
  return _angle;
}

uint8_t HardServo::attached()
{
  return _pin != 0;
}
