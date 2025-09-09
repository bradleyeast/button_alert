#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid     = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* apiBase  = "https://serviceapi.azurewebsites.net/api";

const int buttonPin = 13;  // button (to GND)
const int ledPin    = 15;  // LED

enum State { IDLE, CALLING, ACKED };
State state = IDLE;

unsigned long lastButtonChange = 0;
bool lastButton = HIGH;
bool ledState = false;
unsigned long lastBlink = 0;

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.printf("\nConnected. IP: %s\n", WiFi.localIP().toString().c_str());
}

void loop() {
  handleButton();
  handleState();
}

// --- Button debounce ---
void handleButton() {
  bool reading = digitalRead(buttonPin);
  unsigned long now = millis();

  if (reading != lastButton && (now - lastButtonChange > 50)) { // debounce 50 ms
    lastButtonChange = now;
    lastButton = reading;

    if (reading == LOW) { // pressed
      if (state == IDLE) {
        if (postCall()) {
          state = CALLING;
        }
      } else if (state == ACKED) {
        if (deleteCall()) {
          state = IDLE;
          digitalWrite(ledPin, LOW);
        }
      }
      // If state == CALLING (blinking), ignore presses
    }
  }
}

// --- State machine ---
void handleState() {
  unsigned long now = millis();

  if (state == CALLING) {
    // Blink LED
    if (now - lastBlink > 500) {
      ledState = !ledState;
      digitalWrite(ledPin, ledState);
      lastBlink = now;
    }
    if (getAcknowledge()) {
      state = ACKED;
      digitalWrite(ledPin, HIGH); // solid
    }
  }
}

// --- API helpers ---
bool postCall() {
  HTTPClient http;
  String url = String(apiBase) + "/call";
  http.begin(url);
  int code = http.POST("");
  http.end();
  Serial.printf("POST /call -> %d\n", code);
  return (code >= 200 && code < 300);
}

bool deleteCall() {
  HTTPClient http;
  String url = String(apiBase) + "/call";
  http.begin(url);
  int code = http.sendRequest("DELETE");
  http.end();
  Serial.printf("DELETE /call -> %d\n", code);
  return (code >= 200 && code < 300);
}

bool getAcknowledge() {
  HTTPClient http;
  String url = String(apiBase) + "/acknowledge";
  http.begin(url);
  int code = http.GET();
  if (code == 200) {
    String payload = http.getString();
    http.end();
    payload.trim();
    return payload == "1";
  }
  http.end();
  return false;
}
