void InitialSetup() {

  emon1.current(pinArusR, 11.9);
  emon2.current(pinArusS, 11.9);
  emon3.current(pinArusT, 11.9);       // Current: input pin, calibration.

  pinMode(pinLedM, OUTPUT);
  pinMode(pinLedH, OUTPUT);
  pinMode(pinLedB, OUTPUT);

  Statefirst = true;

    Serial.printf("Connecting to %s ", ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
  
   while (WiFi.status() != WL_CONNECTED)
   {
     StateLedM = true;
     Serial.print(".");
     digitalWrite(pinLedM,HIGH);
     delay(100);
     digitalWrite(pinLedM,LOW);
     delay(100);
     digitalWrite(pinLedM,HIGH);
     delay(100);
     digitalWrite(pinLedM,LOW);
     delay(100);
   }
  
   if (WiFi.status() == WL_CONNECTED){
     StateLedM = false;
   }

  xTaskCreatePinnedToCore(
    TaskHandleThinger,
    "TaskHandleThinger",
    7168,
    NULL,
    1,
    &Task1,
    1);

  delay (200);

  xTaskCreatePinnedToCore(
    TaskGetSensor,
    "TaskGetSensor",
    7168,
    NULL,
    2,
    &Task2,
    1);

  delay (200);

  xTaskCreatePinnedToCore(
    TaskHandleSendToSheets,
    "TaskHandleSendToSheets",
    7168,
    NULL,
    1,
    &Task3,
    0);

  delay (200);

  xTaskCreatePinnedToCore(
    TaskHandleNotifLed,
    "TaskHandleNotifLed",
    1024,
    NULL,
    2,
    &Task4,
    0);

  delay (200);

  thing["Monitoring_Daya"] >> [] (pson & out) {
    out["X1"] = X1;
    out["X2"] = X2;
    out["N"] = N;
    out["Pers25"] = Pers25;
    out["Pers50"] = Pers50;
    out["Pers2"] = PersBeban;
  };

}
