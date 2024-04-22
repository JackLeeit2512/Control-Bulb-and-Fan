void FanManual() {

  if (WiFi.status() != WL_CONNECTED) {

  } else {
    if (Firebase.getInt(fbdo, "/Do_An_1/QUAT/STATUS")) {
      int FanFB = fbdo.intData();

      if (FanFB == 0) {
        digitalWrite(RELAY2, LOW);
      } else {
        digitalWrite(RELAY2, HIGH);
      }
    } else {
      Serial.println("Download fail: ");
    }
  }

  if (millis() - lastTimeBtnFan > TIME_BETWEEN_BNT) {
    btn_fan_state = digitalRead(Btn_FAN);
    if (btn_fan_state != last_btn_fan_state) {
      lastTimeBtnFan = millis();
      last_btn_fan_state = btn_fan_state;
      if (btn_fan_state == LOW) {
        Ctrl_Fan_Manual(RELAY2);
      }
    }
  }
}

//

void Ctrl_Fan_Manual(uint8_t Relay_Pin) {
  relayFanState = !relayFanState;
  digitalWrite(Relay_Pin, relayFanState);

  // display FanManual state on terminal
  if (relayFanState == HIGH) {
    Serial.println("FAN IS ON IN MUNUAL");
    if (WiFi.status() != WL_CONNECTED) {

    } else {
      if (Firebase.setInt(fbdo, "/Do_An_1/QUAT/STATUS", 1))
        Serial.println("Upload fan state success");
    }
  }

  else if (relayFanState == LOW) {
    Serial.println("FAN IS OFF IN MUNUAL");
    if (WiFi.status() != WL_CONNECTED) {
    } else {
      if (Firebase.setInt(fbdo, "/Do_An_1/QUAT/STATUS", 0))
        Serial.println("Upload fan state success");
    }
  }
}
