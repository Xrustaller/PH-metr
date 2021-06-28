
#define CALIBRATION_VALUE 21.34
#define DATCHIK_PIN A0

#define DEBUG 0 // Дебаг

#include <Wire.h>
#include <DTM1650.h>


DTM1650 display;

int phval = 0;
unsigned long int avgval;
int buffer_arr[10], temp;

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
void loop()
{
	for (int i = 0; i < 10; i++)
	{
		buffer_arr[i] = analogRead(DATCHIK_PIN);
		delay(30);
	}
	
	for (int i = 0; i < 9; i++)
	{
		for (int j = i + 1; j < 10; j++)
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
	for (int i = 2; i < 8; i++)
	{
		avgval += buffer_arr[i];
	}
	
	float voltage = ((float)avgval * 5 / 6144);
	float ph_act = -5.70 * voltage + CALIBRATION_VALUE;
	display.write_num(ph_act);
	
#if DEBUG == 1
	Serial.println(ph_act);
#endif
	
	delay(1000);
}