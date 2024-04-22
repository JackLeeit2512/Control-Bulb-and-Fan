void RunChoiceMode() {
  if (MODE == 1) {

    BulbAuto(LIGHT_D, RELAY1);

  } else if (MODE == 0) {
    BulbManual();
    FanManual();
  }
}

//

void ReadMode() {
  if (WiFi.status() != WL_CONNECTED) {

  } else {
    if (Firebase.getInt(fbdo, "/Do_An_1/AUTO_MODE/STATUS")) {
      int ModeFB = fbdo.intData();
      //Serial.println("Download success: " + String(fbdb.stringData()));
      if (ModeFB == 0) {
        if (MODE == 1) {
          timerStop(My_timer);
          Serial.println("MUNUAL");
        }
        MODE = 0;
        digitalWrite(LED_MODE, LOW);
      } else if (ModeFB == 1) {
        if (MODE == LOW) {
          lastTimeUpdateFan = 0;
          timerRestart(My_timer);
          Serial.println("AUTO");
        }
        MODE = 1;
        digitalWrite(LED_MODE, HIGH);
      }
    } else {
      Serial.println("Download fail: ");
    }
  }

  //Kieu 1

  // int reading = digitalRead(Btn_MODE);

  // // Kiểm tra xem trạng thái nút nhấn có thay đổi hay không
  // if (reading != last_Btn_mode_state) {
  //   // Cập nhật thời điểm debounce gần nhất
  //   lastDebounceTime = millis();
  // }
  // last_Btn_mode_state = reading;
  // // Kiểm tra xem đã đủ thời gian debounce chưa
  // if ((millis() - lastDebounceTime) > TIME_BETWEEN_BNT) {
  //   // Kiểm tra trạng thái nút nhấn sau khi đã debounce
  //   if (reading != Btn_mode_state) {
  //     Btn_mode_state = reading;

  //     // Xử lý sự kiện khi nút nhấn được nhấn
  //     if (Btn_mode_state == LOW) {

  //       if (MODE == 1) {

  //         MODE = 0;
  //         digitalWrite(LED_MODE, LOW);
  //         timerStop(My_timer);
  //         Serial.println("AUTO MODE IS OFF");

  //         if (WiFi.status() != WL_CONNECTED) {

  //         } else {
  //           if (Firebase.setInt(fbdo, "/Do_An_1/AUTO_MODE/STATUS", 0))
  //             Serial.println("Upload mode state success");
  //         }

  //       } else if (MODE == 0) {
  //         MODE = 1;
  //         digitalWrite(LED_MODE, HIGH);
  //         timerRestart(My_timer);
  //         Serial.println("AUTO MODE IS ON");
  //         if (WiFi.status() != WL_CONNECTED) {

  //         } else {
  //           if (Firebase.setInt(fbdo, "/Do_An_1/AUTO_MODE/STATUS", 1))
  //             Serial.println("Upload mode state success");
  //         }
  //       }
  //     }
  //   }
  // }

  //Kieu khac
  if (millis() - lastTimeBtnMode > TIME_BETWEEN_BNT) {
    Btn_mode_state = digitalRead(Btn_MODE);
    if (Btn_mode_state != last_Btn_mode_state) {
      last_Btn_mode_state = Btn_mode_state;
      lastTimeBtnMode = millis();
      if (Btn_mode_state == LOW) {

        if (MODE == 1) {

          MODE = 0;
          digitalWrite(LED_MODE, LOW);
          timerStop(My_timer);
          Serial.println("AUTO MODE IS OFF");

          if (WiFi.status() != WL_CONNECTED) {

          } else {
            if (Firebase.setInt(fbdo, "/Do_An_1/AUTO_MODE/STATUS", 0))
              Serial.println("Upload mode state success");
          }

        } else if (MODE == 0) {
          MODE = 1;
          digitalWrite(LED_MODE, HIGH);
          timerRestart(My_timer);
          Serial.println("AUTO MODE IS ON");
          if (WiFi.status() != WL_CONNECTED) {

          } else {
            if (Firebase.setInt(fbdo, "/Do_An_1/AUTO_MODE/STATUS", 1))
              Serial.println("Upload mode state success");
          }
        }
      }
    }
  }
}