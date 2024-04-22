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

#define API_KEY "AIzaSyCLejWktrbQkwhQumtWzRzrFyQ6gOpYvmI"
#define DATABASE_URL "esp-data-logging-a1f34-default-rtdb.asia-southeast1.firebasedatabase.app" 

#define USER_EMAIL "tamle01687375070@gmail.com"
#define USER_PASSWORD "Tamle2512" 

#define STORAGE_BUCKET_ID "esp-data-logging-a1f34.appspot.com"

#define  WIFI_SSID "KIM PHUNG"
#define  WIFI_PASSWORD "68686868"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

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

  config.api_key = API_KEY;

  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  config.database_url = DATABASE_URL;

  Firebase.begin(&config, &auth);
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
