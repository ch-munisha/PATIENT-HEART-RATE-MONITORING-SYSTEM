
const int pulsePin = A0;
const int buzzerPin = 2;

const int threshold = 550;
const int lowLimit = 60;
const int highLimit = 100;

int signalValue = 0;
int bpm = 0;

unsigned long lastBeatTime = 0;
unsigned long lastPrintTime = 0;

bool aboveThreshold = false;

void setup() {
  Serial.begin(9600);
  pinMode(buzzerPin, OUTPUT);

  Serial.println("Patient Heart Rate Monitoring");
  Serial.println("Place your finger on the sensor.");
}

void loop() {
  signalValue = analogRead(pulsePin);

  // Detect a pulse when the signal crosses
  // the threshold from below to above.
  if (signalValue > threshold && !aboveThreshold) {
    unsigned long currentTime = millis();

    if (lastBeatTime != 0) {
      unsigned long interval = currentTime - lastBeatTime;

      // Accept plausible beat intervals only.
      if (interval >= 300 && interval <= 2000) {
        bpm = 60000 / interval;
      }
    }

    lastBeatTime = currentTime;
    aboveThreshold = true;
  }

  if (signalValue < threshold - 20) {
    aboveThreshold = false;
  }

  // Update the display every second.
  if (millis() - lastPrintTime >= 1000) {
    lastPrintTime = millis();

    if (lastBeatTime == 0 ||
        millis() - lastBeatTime > 3000) {
      Serial.println("No pulse detected");
      bpm = 0;
    } else {
      Serial.print("Heart Rate: ");
      Serial.print(bpm);
      Serial.println(" BPM");

      if (bpm < lowLimit || bpm > highLimit) {
        Serial.println("WARNING: Check heart rate!");
        digitalWrite(buzzerPin, HIGH);
      } else {
        digitalWrite(buzzerPin, LOW);
      }
    }
  }
}
