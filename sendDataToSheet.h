//void sendDataToSheet(String a, String b, String c)
//{
//  String url = server + "/trigger/" + eventName + "/with/key/" + IFTTT_Key + "?value1=" + a + "&value2="+ b +"&value3=" + c;  
//  Serial.println(url);
//  //Start to send data to IFTTT
//  HTTPClient http;
//  Serial.print("[HTTP] begin...\n");
//  http.begin(url); //HTTP
//
//  Serial.print("[HTTP] GET...\n");
//  // start connection and send HTTP header
//  int httpCode = http.GET();
//  // httpCode will be negative on error
//  if(httpCode > 0) {
//    // HTTP header has been send and Server response header has been handled
//    Serial.printf("[HTTP] GET... code: %d\n", httpCode);
//    // file found at server
//    if(httpCode == HTTP_CODE_OK) {
//      digitalWrite(pinLedB, HIGH);
//      delay(200);
//      digitalWrite(pinLedB, LOW);
//      String payload = http.getString();
//      Serial.println(payload);
//    }
//  } else {
//    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
//  }
//  http.end();
//
//}
