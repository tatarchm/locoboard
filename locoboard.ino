#include "src/locoboard.h"

void process_remote_button()
{
  if(check_ir_button_pressed())
  {
    switch(get_ir_button())
    {
      case BTN_0:
      {
        Serial.println("Testing display");
        test_display();
        break;
      }
    }
  }
}

void test_display()
{
  clear_display();
  draw_line(0, 0, 10, 10);
  show();
}

void setup() {
  Serial.begin(115200);
  
  setup_display();
}

void loop() {
  process_remote_button();
}