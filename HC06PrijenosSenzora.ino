#include <SoftwareSerial.h>

SoftwareSerial BT(9, 8); // RX, TX

const int ldrPins[4] = {A0, A1, A2, A3};

void setup() {
  Serial.begin(9600);
  BT.begin(9600);
  delay(1000);
  Serial.println("Bluetooth povezan.");
  BT.println("0.00|0.00"); // testni početak
}

void loop() {
  float ukupniLux = 0;

  for (int i = 0; i < 4; i++) {
    int analogValue = analogRead(ldrPins[i]);
    float voltage = analogValue * (5.0 / 1023.0);
    float resistance = (5.0 - voltage) * 10000.0 / voltage;
    float lux = 12518931 * pow(resistance, -1.405);
    ukupniLux += lux;
  }

  float prosjecniLux = ukupniLux / 4.0;
  float solarnaSnaga = prosjecniLux * 0.2;
  float outputVoltage = solarnaSnaga * 5.0 / 1000.0;

  // Šalji samo prosječnu jačinu svjetlosti i napon, odvojeno "|"
  BT.print(prosjecniLux, 2);
  BT.print("|");
  BT.println(outputVoltage, 2);

  Serial.print(prosjecniLux, 2);
  Serial.print("|");
  Serial.println(outputVoltage, 2);

  delay(1000);
}
