#include <WiFiManager.h>
#include "FirebaseESP32.h"

const int AirValue = 4095;    //you need to replace this value with Value_1
const int WaterValue = 1850;  //you need to replace this value with Value_2
const int SensorPin = A0;
float soilMoistureValue = 0;
float soilmoisturepercent = 0;

// Host servidor
#define FIREBASE_HOST "aipjackboys-default-rtdb.firebaseio.com"

// Secretos de la base de datos
#define FIREBASE_AUTH "kRW9DIgbPhTIBXuFgFxiaMLPYwQJ94DYHXbNlZ28"

WiFiClient client;
FirebaseData firebaseData;

void setup() {
  Serial.begin(9600);
  WiFiManager wifiManager;
  wifiManager.autoConnect("AutoConnectAP");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Se conectó al wifi!");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {

  Firebase.getInt(firebaseData, "/state");
  int value = firebaseData.intData();
  soilMoistureValue = analogRead(A0);
  digitalWrite(soilMoistureValue, value);

   //put Sensor insert into soil
  Serial.println(soilMoistureValue);
  soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);
  //soilmoisturepercent = constrain(soilmoisturepercent, 0, 100);
  if (soilmoisturepercent >= 100) {
    //Serial.println("100 %");
    Firebase.setFloat(firebaseData,"humidity", soilmoisturepercent);

  } else if (soilmoisturepercent <= 0) {
    //Serial.println("0 %");
    Firebase.setFloat(firebaseData,"humidity", 0);

  } else if (soilmoisturepercent > 0 && soilmoisturepercent < 100) {
    //Serial.print(soilmoisturepercent);
    //Serial.println("%");
    Firebase.setFloat(firebaseData,"humidity", soilmoisturepercent);
  }
  delay(2000);
}