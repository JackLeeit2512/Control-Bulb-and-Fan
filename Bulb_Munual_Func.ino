void BulbManual() {
  if (WiFi.status() != WL_CONNECTED) {

  } else {
    if (Firebase.getInt(fbdo, "/Do_An_1/DEN/STATUS")) {

      int BulbFB = fbdo.intData();

      if (BulbFB == 0) {
        digitalWrite(RELAY1, LOW);
      } else {
        digitalWrite(RELAY1, HIGH);
      }
    } else {
      Serial.println("Download fail: ");
    }
  }

  if (millis() - lastTimeBtnBulb > TIME_BETWEEN_BNT) {
    btn_bulb_state = digitalRead(Btn_BULB);
    if (btn_bulb_state != last_btn_bulb_state) {
      lastTimeBtnBulb = millis();
      last_btn_bulb_state = btn_bulb_state;
      if (btn_bulb_state == LOW) {
        Ctrl_Bulb_Manual(RELAY1);
      }
    }
  }
}

//

void Ctrl_Bulb_Manual(uint8_t Relay_Pin) {
  relayBulbState = !relayBulbState;
  digitalWrite(Relay_Pin, relayBulbState);

  if (relayBulbState == HIGH) {
    Serial.println("DEN IS ON IN MUNUAL");
    if (WiFi.status() != WL_CONNECTED) {

    } else {
      if (Firebase.setInt(fbdo, "/Do_An_1/DEN/STATUS", 1))
        Serial.println("Upload bulb state success");
    }
  } else if (relayBulbState == LOW) {
    Serial.println("DEN IS OFF IN MANUAL");
    if (WiFi.status() != WL_CONNECTED) {

    } else {
      if (Firebase.setInt(fbdo, "/Do_An_1/DEN/STATUS", 0))
        Serial.println("Upload bulb state success");
    }
  }
}


