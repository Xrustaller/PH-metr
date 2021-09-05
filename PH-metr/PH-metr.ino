
#define BTN_PIN_START 2 // Кнопка СТАРТ
#define BEEP_PIN 9      // Пин пьезо (при выборе 9 пина, 10 - недоступен из-за шим)
#define DATCHIK_PIN A0

#define CALIBRATION_VALUE 21.34

#define LED 13 

#define DEBUG 0 // Дебаг

#include <Wire.h>
#include <GyverButton.h>
#include <DTM1650.h>


DTM1650 display;

GButton button_start(BTN_PIN_START);

bool is_start = false;

int phval = 0;
unsigned long int avgval;
int buffer_arr[20], temp;

void button_tick()
{
	button_start.tick();

	if (!is_start)
	{
		if (button_start.isSingle())
		{
			display.send_digit(DTM1650_S_MINUS, 0);
			display.send_digit(DTM1650_S_MINUS, 1);
			display.send_digit(DTM1650_S_MINUS, 2);
			display.send_digit(DTM1650_S_MINUS, 3);
			is_start = true;
			digitalWrite(LED, 255);
			tone(BEEP_PIN, 3000, 100);
		}
		return;
}
}

void setup()
{
	Wire.begin();
#if DEBUG == 1
	Serial.begin(9600);
	Serial.println("START");
#endif
	
	display.init();
	display.set_brightness(DTM1650_BRIGHTNESS_MAX);
	
	display.send_digit(DTM1650_S_MINUS, 0);
	display.send_digit(DTM1650_S_MINUS, 1);
	display.send_digit(DTM1650_S_MINUS, 2);
	display.send_digit(DTM1650_S_MINUS, 3);
	//write_display(100);
	delay(1000);
}

void measuring_tick()
{
	for (int i = 0; i < 20; i++)
	{
		buffer_arr[i] = analogRead(DATCHIK_PIN);
		delay(50);
	}

	for (int i = 0; i < 19; i++)
	{
		for (int j = i + 1; j < 20; j++)
		{
			if (buffer_arr[i] > buffer_arr[j])
			{
				temp = buffer_arr[i];
				buffer_arr[i] = buffer_arr[j];
				buffer_arr[j] = temp;
			}
		}
	}

	avgval = 0;
	for (int i = 2; i < 13; i++)
	{
		avgval += buffer_arr[i];
	}

	float voltage = ((float)avgval * 10 / 6144);
	float ph_act = -5.70 * voltage + CALIBRATION_VALUE;
	tone(BEEP_PIN, 3000, 1000);
	display.write_num(ph_act);
	digitalWrite(LED, 0);

#if DEBUG == 1
	Serial.println(ph_act);
#endif
}

void loop()
{
	button_tick();

	if (is_start)
	{
		delay(30000);
		measuring_tick();
		is_start = false;
	}
}