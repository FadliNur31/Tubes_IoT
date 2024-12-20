#define BLYNK_TEMPLATE_ID "TMPL69uV3Mzxj"
#define BLYNK_TEMPLATE_NAME "Yoi Bro"
#define BLYNK_AUTH_TOKEN "kxG9SAHBWKYN0wvn64Vai7Ywj1MMatiz"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define TDS_PIN A0
#define ONE_WIRE_BUS D2

const float VREF = 3.3;
const int ADC_RESOLUTION = 1024;
const float TDS_FACTOR = 5;
const float TEMP_COEFFICIENT = 0.02;

char ssid[] = "";
char pass[] = "";

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

float tdsValue = 0;
float temperature = 25.0;

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  sensors.begin();
  Serial.println("Start Reading...");

}

void loop() {
  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);

  int sensorValue = analogRead(TDS_PIN);
  float voltage = sensorValue * (VREF / ADC_RESOLUTION);
  float ec = voltage / TDS_FACTOR;

  float tempCoefficient = 1.0 + TEMP_COEFFICIENT * (temperature - 25.0);
  float correctedEc = ec * tempCoefficient;

  tdsValue = (133.42 * correctedEc * correctedEc * correctedEc-
              255.86 * correctedEc * correctedEc +
              857.39 * correctedEc);

  Blynk.virtualWrite(V1, temperature);
  Blynk.virtualWrite(V2, tdsValue);

  Serial.print("Suhu : ");
  Serial.print(temperature, 2);
  Serial.print(" C");
  Serial.print("TDS : ");
  Serial.print(tdsValue, 2);
  Serial.print(" ppm");

  delay(1000);

}
