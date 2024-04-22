void BulbAuto(int8_t Light_Sensor_Pin, uint8_t Relay_Pin) {
  int D_Value = digitalRead(Light_Sensor_Pin);
  relayBulbState = digitalRead(Relay_Pin);
  if ((D_Value == HIGH) && (relayBulbState == LOW)) {
    //digitalWrite(Relay_Pin, HIGH);
    Serial.println("AUTO MODE: DEN ON");
    if (Firebase.setInt(fbdo, "/Do_An_1/DEN/STATUS", 1)) {
      Serial.println("Upload bulb state success");
    }
  } else if ((D_Value == LOW) && (relayBulbState == HIGH)) {
    //digitalWrite(Relay_Pin, LOW);
    Serial.println("AUTO MODE: DEN OFF");
    if (WiFi.status() != WL_CONNECTED) {

    } else {
      if (Firebase.setInt(fbdo, "/Do_An_1/DEN/STATUS", 0))
        Serial.println("Upload bulb state success");
    }
  }

  relayBulbState = D_Value;

  if (relayBulbState == HIGH) {
    digitalWrite(Relay_Pin, HIGH);
  } else {
    digitalWrite(Relay_Pin, LOW);
  }

  sendFanState();
}

//

void sendFanState() {
  relayFanState = digitalRead(RELAY2);
  if (WiFi.status() != WL_CONNECTED) {

  } else {
    if (MODE == 1) {
      if (millis() - lastTimeUpdateFan > 2000) {
        if (relayFanState == HIGH) {

          if (Firebase.setInt(fbdo, "/Do_An_1/QUAT/STATUS", 1))
            //Serial.println("Upload fan state on success");
            return;
        } else if (relayFanState == LOW) {

          if (Firebase.setInt(fbdo, "/Do_An_1/QUAT/STATUS", 0))
            //Serial.println("Upload fan state off success");
            return;
        }

        lastTimeUpdateFan = millis();
      }
    }
  }
}
