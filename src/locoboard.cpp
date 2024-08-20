#include "locoboard.h"

#include <Servo.h>

// #define USE_CALLBACK_FOR_TINY_RECEIVER
#include <TinyIRReceiver.hpp>

#ifdef USE_DISPLAY
#include <Adafruit_SSD1306.h>
#endif

Motor motor[2];
Servo servo[3];
Sonar sonar[3];
Remote remote;

#ifdef USE_DISPLAY
Adafruit_SSD1306 display(128, 64, &Wire, -1);
#endif

void rotate_motor(unsigned char motor_ind, unsigned char direction, unsigned char speed)
{
  unsigned char v1, v2;
  switch(direction)
  {
    case 0:
    {
      v1 = LOW;
      v2 = speed;
      break;
    }
    case 1:
    {
      v1 = HIGH;
      v2 = 255 - speed;
      break;
    }
  }
  digitalWrite(motor[motor_ind].pin_a, v1);
  analogWrite(motor[motor_ind].pin_b, v2);
}

void stop_motor(unsigned char motor_ind)
{
  digitalWrite(motor[motor_ind].pin_a, LOW);
  digitalWrite(motor[motor_ind].pin_b, LOW);
}

void setup_motor_pins(unsigned char motor_ind, unsigned char pin_a, unsigned char pin_b)
{
  motor[motor_ind].pin_a = pin_a;
  motor[motor_ind].pin_b = pin_b;

  pinMode(motor[motor_ind].pin_a, OUTPUT);
  pinMode(motor[motor_ind].pin_b, OUTPUT);

  digitalWrite(motor[motor_ind].pin_a, LOW);
  digitalWrite(motor[motor_ind].pin_b, LOW);
}

void rotate_servo(unsigned char servo_ind, unsigned char degrees)
{
  servo[servo_ind].write(degrees);
}

void setup_servo(unsigned char servo_ind, unsigned char pin)
{
  servo[servo_ind].attach(pin);
}

void setup_sonar(unsigned char sonar_ind, unsigned char trig_pin, unsigned char echo_pin)
{
  sonar[sonar_ind].echo_pin = echo_pin;
  sonar[sonar_ind].trig_pin = trig_pin;
  pinMode(sonar[sonar_ind].trig_pin, OUTPUT);
  pinMode(sonar[sonar_ind].echo_pin, INPUT);
}

void measure_sonar_distance(unsigned char sonar_ind)
{
  digitalWrite(sonar[sonar_ind].trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(sonar[sonar_ind].trig_pin, LOW);

  float duration_us = pulseIn(sonar[sonar_ind].echo_pin, HIGH);
  float distance_cm = 0.017 * duration_us;
  if(distance_cm > 0) sonar[sonar_ind].distance = (int)(0.017 * duration_us);
}

int get_sonar_distance(unsigned char sonar_ind)
{
  return sonar[sonar_ind].distance;
}

bool check_ir_button_pressed()
{
  if(TinyReceiverDecode())
  {
    remote.button = TinyIRReceiverData.Command;
    return true;
  }
  else return false;
}

unsigned char get_ir_button()
{
  return remote.button;
}

#ifdef USE_DISPLAY

void setup_display()
{
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    for(;;); // Don't proceed, loop forever
  }
}

void draw_line(int x1, int y1, int x2, int y2)
{
  display.drawLine(x1, y1, x2, y2, SSD1306_WHITE);
}

void draw_rectangle(int x1, int y1, int x2, int y2, bool filled)
{
  if(!filled) display.drawRect(x1, y1, x2, y2, SSD1306_WHITE);
  else display.fillRect(x1, y1, x2, y2, SSD1306_WHITE);
}

void draw_circle(int x, int y, int radius, bool filled)
{
  if(!filled) display.drawCircle(x, y, radius, SSD1306_WHITE);
  else display.fillCircle(x, y, radius, SSD1306_WHITE);
}

void draw_pixel(int x, int y)
{
  display.drawPixel(x, y, SSD1306_WHITE);
}

void draw_text(int x, int y, char* text, unsigned char size)
{
  display.setTextSize(size);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(x, y);
  display.println(text);
}

void show()
{
  display.display();
}

void clear_display()
{
  display.clearDisplay();
}

#endif