#define IR_USE_TIMER1  // Force IRremote to Timer1 to prevent timer conflict
#include <IRremote.h>
#include <Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

// define pins
#define TFT_CS    10
#define TFT_RST   9
#define TFT_DC    8
#define RECV_PIN  7
#define LDR_PIN   A0
#define STABLE_LED 3
#define ASTABLE_LED 4
#define OVERRIDE_LED 5
#define BUZZER_PIN 6
#define SERVO_PIN 2

// constants
const int LDR_THRESHOLD = 350;
const int BUZZER_FREQ = 1000;
const int BUZZER_DURATION = 100;
const int SERVO_ALERT_POS = 180;
const int SERVO_NORMAL_POS = 0;
const unsigned long BUZZER_INTERVAL = 200; // Time between beeps

// irremote hex codes
#define OVERRIDE_OFF_CODE 0xFF18E7
#define OVERRIDE_ON_CODE 0xFF38C7

// logical stated
enum SystemState {
  STATE_NORMAL,
  STATE_INTRUSION,
  STATE_OVERRIDE
};

// objects
Servo myServo;
IRrecv irrecv(RECV_PIN);
decode_results results;
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// variables
SystemState currentState = STATE_NORMAL;
unsigned long lastBuzzerToggle = 0;
bool buzzerState = false;

void setup() {
  Serial.begin(9600);
  pinMode(STABLE_LED, OUTPUT);
  pinMode(ASTABLE_LED, OUTPUT);
  pinMode(OVERRIDE_LED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  irrecv.enableIRIn();
  myServo.attach(SERVO_PIN);

  tft.initR(INITR_BLACKTAB);
  tft.setTextSize(2);
  displayStatus("SYSTEM", "READY");
}

void loop() {
  unsigned long currentMillis = millis();
  
  // Read sensors and check state
  int ldrValue = analogRead(LDR_PIN);
  Serial.println(ldrValue);

  if (currentState != STATE_OVERRIDE) {
    if (ldrValue < LDR_THRESHOLD) {
      currentState = STATE_INTRUSION;
    } else {
      currentState = STATE_NORMAL;
    }
  }

  // State handling
  switch(currentState) {
    case STATE_NORMAL:
      handleNormalState();
      break;
    case STATE_INTRUSION:
      handleIntrusionState(currentMillis);
      break;
    case STATE_OVERRIDE:
      handleOverrideState();
      break;
  }

  checkIRRemote();
}

// Custom tone function
void myTone(uint8_t pin, unsigned int frequency, unsigned long duration) {
  pinMode(pin, OUTPUT);
  analogWrite(pin, 128);  // 50% duty cycle
  delay(duration);
  analogWrite(pin, 0);
}


void myNoTone(uint8_t pin) {
  analogWrite(pin, 0);
}

void handleNormalState() {
  digitalWrite(ASTABLE_LED, LOW);
  digitalWrite(STABLE_LED, HIGH);
  digitalWrite(OVERRIDE_LED, LOW);
  myNoTone(BUZZER_PIN);
  myServo.write(SERVO_NORMAL_POS);
  displayStatus("STABLE", "SYSTEM");
}

void handleIntrusionState(unsigned long currentMillis) {
  digitalWrite(STABLE_LED, LOW);
  
  
  if (currentMillis - lastBuzzerToggle >= BUZZER_INTERVAL) {
    lastBuzzerToggle = currentMillis;
    buzzerState = !buzzerState;
    
    if (buzzerState) {
      myTone(BUZZER_PIN, BUZZER_FREQ, BUZZER_DURATION);
      digitalWrite(ASTABLE_LED, HIGH);
    } else {
      myNoTone(BUZZER_PIN);
      digitalWrite(ASTABLE_LED, LOW);
    }
  }

  myServo.write(SERVO_ALERT_POS);
  displayStatus("INTRUDER", "DETECTED");
}

void handleOverrideState() {
  digitalWrite(STABLE_LED, LOW);
  digitalWrite(ASTABLE_LED, LOW);
  digitalWrite(OVERRIDE_LED, HIGH);
  myNoTone(BUZZER_PIN);
  myServo.write(SERVO_ALERT_POS);
  displayStatus("SYSTEM", "OVERRIDE");
}

void checkIRRemote() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    switch(results.value) {
      case OVERRIDE_ON_CODE:
        currentState = STATE_OVERRIDE;
        digitalWrite(OVERRIDE_LED, HIGH);
        break;
      case OVERRIDE_OFF_CODE:
        currentState = STATE_NORMAL;
        digitalWrite(OVERRIDE_LED, LOW);  
        break;
    }
    irrecv.resume();
    delay(100); // Simple debounce
  }
}

void displayStatus(String line1, String line2) {
  static String lastLine1 = "";
  static String lastLine2 = "";
  
  // only update display if text has changed
  if (line1 != lastLine1 || line2 != lastLine2) {
    tft.fillScreen(ST7735_BLACK);
    tft.setTextColor(ST7735_WHITE);
    tft.setCursor(10, 50);
    tft.print(line1);
    tft.setCursor(10, 80);
    tft.print(line2);
    
    lastLine1 = line1;
    lastLine2 = line2;
  }
}
