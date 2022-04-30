/* Released into the public domain */

void setup() {
  Serial.begin(115200);
  delay(5000);
}

void loop() {
  Serial.printf("%2.1fC\n", analogReadTemp());
  delay(1000);
}
