void sendDataToSheets(String a, String b, String c, String d, String e, String f) {
  while (StateData == false) {
    HTTPClient http;
    String url = "https://script.google.com/macros/s/" + GOOGLE_SCRIPT_ID + "/exec?value1=" + a + "&value2=" + b + "&value3=" + c + "&value4=" + d + "&value5=" + e + "&value6=" + f;
    //   Serial.print(url);
    Serial.print("Making a request");
    http.begin(url.c_str()); //Specify the URL and certificate
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET();
    String payload;
    if (httpCode > 0) { //Check for the returning code
      StateData = true;
      digitalWrite(pinLedB, HIGH);
      delay(2000);
      digitalWrite(pinLedB, LOW);
      payload = http.getString();

      Serial.println(httpCode);
      Serial.println(payload);
      Serial.println(StateData);
    }
    else {
      Serial.println("Error on HTTP request");
      Serial.println(StateData);
    }
    http.end();
  }
}
