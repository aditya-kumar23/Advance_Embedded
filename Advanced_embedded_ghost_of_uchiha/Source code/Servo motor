#include <Servo.h>

int pos = 0;

Servo servo_1;

void setup()
{
  servo_1.attach(2);
}

void loop()
{
  for (pos = 0; pos <= 180; pos += 1) {
    servo_1.write(pos);
    delay(15);
  }
  
  delay(1000);
  
  for (pos = 180; pos >= 0; pos -= 1) {
    servo_1.write(pos);
    delay(15);
  }
  
  delay(1000);
}
