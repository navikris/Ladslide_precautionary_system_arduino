#define BLYNK_PRINT DebugSerial

#include <SoftwareSerial.h>
SoftwareSerial DebugSerial(2, 3);

#include <BlynkSimpleStream.h>

char auth[] = "u7Uu7k9FUovuOsGmJMWEty_NDa1nY6kU";

#include <math.h>

const int x_out = A1;
const int y_out = A2;
const int z_out = A3;
const int sensor_pin = A0;
float moisture_percentage;
int sensor_value;
int x_adc_value, y_adc_value, z_adc_value; 
double x_g_value, y_g_value, z_g_value;
int limit = 50;

BlynkTimer timer;

void setup()
{
  DebugSerial.begin(9600);
  Serial.begin(9600);
  Blynk.begin(Serial, auth);
  Blynk.notify("Device started");

  // Setup a function to be called every minute
  timer.setInterval(1000L, sensorDataSend);
}

void sensorDataSend()
{
  sensor_value = analogRead(sensor_pin);
  moisture_percentage = ( 70 - ( (sensor_value/1023.00) * 100 ) );
  Serial.print("Moisture Percentage = ");
  Serial.print(moisture_percentage);
  Serial.print("%\n\n");
  
  Blynk.virtualWrite(V1, moisture_percentage);
  
  if (moisture_percentage>limit) 
  {
     Blynk.notify("Warning:Critical Moisture Level");
     Blynk.email("navi.kris2001@gmail.com", "Subject: Warning", "Soil moisture level CRITICAL");
  }
  
  x_adc_value = analogRead(x_out);
  y_adc_value = analogRead(y_out);
  z_adc_value = analogRead(z_out);
  
  x_g_value = ( ( ( (double)(x_adc_value * 5)/1024) - 1.65 ) / 0.330 );
  y_g_value = ( ( ( (double)(y_adc_value * 5)/1024) - 1.65 ) / 0.330 );
  z_g_value = ( ( ( (double)(z_adc_value * 5)/1024) - 1.80 ) / 0.330 );
  
  Blynk.virtualWrite(V2, x_g_value);
  Blynk.virtualWrite(V3, y_g_value);
  Blynk.virtualWrite(V4, z_g_value);
}

void loop()
{
  Blynk.run();
  timer.run();
}
