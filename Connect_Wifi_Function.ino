void ConnectWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");

  unsigned long StartConnect = millis();

  while ((WiFi.status() != WL_CONNECTED) && (millis() - StartConnect < 5000)) {
    Serial.print(".");
    delay(100);
  }

  if (WiFi.status() != WL_CONNECTED){
    Serial.println("Failed to connect Wifi !!!");
 }
  else{
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();
   }
}
