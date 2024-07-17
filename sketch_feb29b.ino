#define INTERRUPT_ATTR IRAM_ATTR
#include <HardwareSerial.h>
#include "BleCombo.h"

HardwareSerial mk1(1);
HardwareSerial mk2(2);

//unsigned char alf[] = {'j', 'b', 'n', 'k', 'd', 'r', 'l', 'e', 's', 'u', ',', 'w', ';', '\''};
unsigned char alf[] = {'d', '1', 'n', 'k', 'b', 'v', 'l', 'e', 'h', 'n', 'd', 'w', 'g', 't'};
uint8_t nums[] = {29, 30, 17, 18, 15, 14, 65, 64, 68, 69, 58, 59, 55, 54};

int number;
bool toggle_mode = false;
bool is_pressed[] = {false, false, false, false, false};
bool pressed[14];
int x_axis[20];
int y_axis[20];
int x_mean = 0;
int y_mean = 0;
int button = 0;

void setup()
{
  mk1.begin(4800, SERIAL_8N1, 16, 17);
  mk2.begin(4800, SERIAL_8N1, 5, 18);
  Serial.begin(9600);

  pinMode(34, INPUT);
  pinMode(35, INPUT);
  pinMode(32, INPUT_PULLUP);

  for (uint8_t i = 0; i < 14; ++i)
  {
    pressed[i] = false;
  }
}

int doAxis(int* axis)
{
  int mean = 0;
  for (int i = 19; i > -1; --i)
  {
    mean += axis[i];
    if (i != 0) axis[i] = axis[i - 1];
  }
  mean /= 20;
  
  if (mean < 15 && mean > -15) mean = 0;
  
  return mean;
}

void pushButt(int num)
{
  for (uint8_t i = 0; i < 14; ++i)
  {
    if (num == nums[i] && !pressed[i])
    {
      pressed[i] = true;
      Keyboard.press(alf[i]);
    }
    else if (num == nums[i] + 20 && pressed[i])
    {
      pressed[i] = false;
      for (uint8_t j = 0; j < 5; ++j)
      {
        Keyboard.release(alf[i]);
      }
    }
  }
}

void loop()
{
  Keyboard.begin();
  Mouse.begin();
  delay(1000);
  while (Keyboard.isConnected())
  {
    x_axis[0] = -1 * (analogRead(34) - 1887);
    y_axis[0] = -1 * (analogRead(35) - 1960);
    x_mean = doAxis(x_axis);
    y_mean = doAxis(y_axis);
    if (x_mean || y_mean)
    {
      Serial.println();
      Serial.println(x_mean);
      Serial.println(y_mean);
      Serial.println();
      Mouse.move(x_mean / 300, y_mean / 300);
    }

    button = !digitalRead(32);
    //Serial.println(button);
    if (button)
    {
      if (!is_pressed[0])
      {
        is_pressed[0] = true;
        Mouse.press(MOUSE_LEFT);
      }
      else if (toggle_mode)
      {
        for (uint8_t i = 0; i < 5; ++i)
        {
          Mouse.release(MOUSE_LEFT);
        }
      }
    }
    else
    {
      if (is_pressed[0] && !toggle_mode)
      {
        is_pressed[0] = false;
        for (uint8_t i = 0; i < 5; ++i)
        {
          Mouse.release(MOUSE_LEFT);
        }
      }
    }
    /*number = Keyboard.readSecondHand();
    if (number)
    {
      Serial.print("Second Hand: ");
      Serial.println(number);
    }*/
    if (mk1.available())
    {
      number = mk1.read();
      Serial.print("1: ");
      Serial.println(number);
      if (number == 28)
      {
        if (toggle_mode)
        {
          if (!is_pressed[1])
          {
            Keyboard.press(KEY_LEFT_SHIFT);
            is_pressed[1] = true;
          }
          else
          {
            is_pressed[1] = false;
            for (uint8_t i = 0; i < 5; ++i)
            {
              Keyboard.release(KEY_LEFT_SHIFT);
            }
          }
        }
        else
        {
         Keyboard.press(KEY_LEFT_SHIFT);
        }
      }
      else if (number == 48 && !toggle_mode)
      {
        is_pressed[1] = false;
        for (uint8_t i = 0; i < 5; ++i)
        {
          Keyboard.release(KEY_LEFT_SHIFT);
        }
      }
      else if (number == 27)
      {
        if (toggle_mode)
        {
          Mouse.press(MOUSE_RIGHT);
        }
        else
        {
          Mouse.click(MOUSE_RIGHT);
        }
      }
      else if (number == 47 && !toggle_mode)
      {
        Mouse.release(MOUSE_RIGHT);
      }
      else if (number == 16)
      {
        if (toggle_mode)
        {
          Keyboard.press(KEY_BACKSPACE);
        }
        else
        {
          Keyboard.press(KEY_BACKSPACE);
        }
      }
      else if (number == 36 && !toggle_mode)
      {
        for (uint8_t i = 0; i < 5; ++i)
        {
          Keyboard.release(KEY_BACKSPACE);
        }
      }
      else
      {
        pushButt(number);
      }
    }
    if (mk2.available())
    {
      number = mk2.read();
      Serial.print("2: ");
      Serial.println(number);
      if (number == 27)
      {
        if (toggle_mode)
        {
          Keyboard.press(KEY_LEFT_CTRL);
        }
        else
        {
          Keyboard.press(KEY_LEFT_CTRL);
        }
      }
      else if (number == 47 && !toggle_mode)
      {
        for (uint8_t i = 0; i < 5; ++i)
        {
          Keyboard.release(KEY_LEFT_CTRL);
        }
      }
      else if (number == 26)
      {
        if (!is_pressed[2])
        {
          is_pressed[2] = true;
          toggle_mode = true;
        }
        else
        {
          is_pressed[2] = false;
          toggle_mode = false;
        }
      }
      else if (number == 16)
      {
        if (toggle_mode)
        {
          Keyboard.press(' ');
        }
        else
        {
          Keyboard.press(' ');
        }
      }
      else if (number == 36 && !toggle_mode)
      {
        for (uint8_t i = 0; i < 5; ++i)
        {
          Keyboard.release(' ');
        }
      }
      else if (number == 17)
      {
        if (toggle_mode)
        {
          Keyboard.press(KEY_TAB);
        }
        else
        {
          Keyboard.press(KEY_TAB);
        }
      }
      else if (number == 37 && !toggle_mode)
      {
        for (uint8_t i = 0; i < 5; ++i)
        {
          Keyboard.release(KEY_TAB);
        }
      }
      else if (number == 30)
      {
        Mouse.move(0, 0, -1);
      }
      else if (number == 23)
      {
        Mouse.move(0, 0, 1);
      }
      else
      {
        pushButt(number + 40);
      }
    }
  }
  Keyboard.end();
  Mouse.end();
}
