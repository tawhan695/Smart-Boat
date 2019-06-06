-
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>


char ssid[] = "Tawhan-Studio";
char pass[] = "T0655577688";
char auth[] = "f0e9eb6e24124db1b7abeed355c46eda";

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
}
