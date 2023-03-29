#include <WiFiManager.h>
#include "FirebaseESP32.h"

int D2 = 2;
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

  pinMode(D2, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Firebase.getFloat(firebaseData, "/humidity");
  soilmoisturepercent = firebaseData.floatData();

  if(soilmoisturepercent >= 60 ) {
    digitalWrite(D2, HIGH);
  } else if (soilmoisturepercent < 60) {
    digitalWrite(D2, LOW);
  }

  delay(2000);
}
