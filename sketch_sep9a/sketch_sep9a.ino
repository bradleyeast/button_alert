// Pin definitions
const int buttonPin = 13;  // GPIO13 (green wire, button to GND)
const int ledPin    = 15;  // GPIO15 (yellow wire, LED)

void setup() {
  // Button as input with internal pull-up
  pinMode(buttonPin, INPUT_PULLUP);

  // LED as output
  pinMode(ledPin, OUTPUT);

  // Start with LED off
  digitalWrite(ledPin, LOW);
}

void loop() {
  // Read button (pressed = LOW because wired to GND)
  int buttonState = digitalRead(buttonPin);

  if (buttonState == LOW) {
    // Button pressed → turn LED on
    digitalWrite(ledPin, HIGH);
  } else {
    // Button released → LED off
    digitalWrite(ledPin, LOW);
  }
}
