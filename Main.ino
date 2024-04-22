#include <WiFi.h>
#include <FirebaseESP32.h>
#include <string.h>

#define Btn_BULB 13
#define Btn_FAN 12
#define Btn_MODE 25
#define RELAY1 26
#define RELAY2 27
#define LIGHT_D 14
#define LED_MODE 33
#define TIME_BETWEEN_BNT 50
#define TIME_BETWEEN_AUTO_FAN 6000000
#define FIREBASE_HOST "https://esp32-51b05-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "pFOw8cuDHnyYHrEtZt5BWQGbaWKzaLOW4k8IEkhP"

// #define WIFI_SSID "Phong 7A"
// #define WIFI_PASSWORD "phong7a4nguoi"

#define  WIFI_SSID "KIM PHUNG"
#define  WIFI_PASSWORD "68686868"

// #define WIFI_SSID "Khoamutou"
// #define WIFI_PASSWORD "Mechan11"

FirebaseData fbdo;

int MODE = 1;

int relayBulbState = LOW;
int relayFanState = HIGH;

int Btn_mode_state = HIGH;
int last_Btn_mode_state = HIGH;

int btn_bulb_state;
int last_btn_bulb_state = LOW;

int btn_fan_state;
int last_btn_fan_state = LOW;
unsigned long lastDebounceTime = 0;  // Thời điểm debounce gần nhất

unsigned long lastTimeBtnMode = 0;
unsigned long lastTimeBtnBulb = 0;
unsigned long lastTimeBtnFan = 0;

unsigned long lastTimeUpdateFan = 0;

hw_timer_t *My_timer = NULL;

//portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
void IRAM_ATTR onTimer() {
  //portENTER_CRITICAL_ISR(&timerMux); //vào chế độ tránh xung đột
  digitalWrite(RELAY2, !digitalRead(RELAY2));
  relayFanState = digitalRead(RELAY2);
  // display fan state
  if (relayFanState == HIGH) {
    Serial.println("NOW AUTO: FAN IS ON");
  } else if (relayFanState == LOW) {
    Serial.println("NOW AUTO: FAN IS OFF");
  }
  //portEXIT_CRITICAL_ISR(&timerMux); // thoát
}

void setup() {

  Serial.begin(115200);

  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(LED_MODE, OUTPUT);
  pinMode(LIGHT_D, INPUT);

  digitalWrite(RELAY2, relayFanState);
  digitalWrite(LED_MODE, HIGH);

  pinMode(Btn_MODE, INPUT_PULLUP);
  last_Btn_mode_state = digitalRead(Btn_MODE);  

  pinMode(Btn_BULB, INPUT_PULLUP);
  last_btn_bulb_state = digitalRead(Btn_BULB); 

  pinMode(Btn_FAN, INPUT_PULLUP);
  last_btn_fan_state = digitalRead(Btn_FAN); 

  ConnectWifi();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  Firebase.setReadTimeout(fbdo, 1000 * 60);
  Firebase.setwriteSizeLimit(fbdo, "tiny");

  Firebase.setInt(fbdo, "/Do_An_1/AUTO_MODE/STATUS", 1);
  // timer
  My_timer = timerBegin(0, 80, true);
  timerAttachInterrupt(My_timer, &onTimer, true);
  timerAlarmWrite(My_timer, TIME_BETWEEN_AUTO_FAN, true);
  timerAlarmEnable(My_timer);  
}

void loop() {
  ReadMode();
  RunChoiceMode();
  sendFanState();
}
