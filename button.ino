 #include <SoftwareSerial.h>

SoftwareSerial mySerial(1, 0); // RX, TX
int pushButton = 2;

// the setup routine runs once when you press reset:
void setup() {
  mySerial.begin(115200);
  mySerial.println("hello");
  pinMode(pushButton, INPUT);
  digitalWrite(pushButton, HIGH);
}

int previousState = 1;


void loop() {
  int buttonState = digitalRead(pushButton);
  if (buttonState == 0 && buttonState != previousState) {
    mySerial.println("POST /hits.json");
    mySerial.println("{\".sv\": \"timestamp\"}");
  }
  previousState = buttonState;
}



