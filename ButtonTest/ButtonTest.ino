const int buttonPins[4] = {2, 3, 4, 5};
const char* buttonNames[4] = {"K1", "K2", "K3", "K4"};
bool lastState[4] = {HIGH, HIGH, HIGH, HIGH};

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 2; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
}

void loop() {
  for (int i = 0; i < 2; i++) {
    bool state = digitalRead(buttonPins[i]);

    if (state == LOW && lastState[i] == HIGH) {
      Serial.print("Botón pulsado: ");
      Serial.println(buttonNames[i]);
      delay(50); // simple debounce
    }

    lastState[i] = state;
  }
}
