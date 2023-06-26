#include "EmonLib.h"
#include <WiFi.h>
#include <WiFiManager.h>
#include <HTTPClient.h>
#include <ThingerESP32.h>

EnergyMonitor emon1;
EnergyMonitor emon2;
EnergyMonitor emon3;

TaskHandle_t Task1;
TaskHandle_t Task2;
TaskHandle_t Task3;
TaskHandle_t Task4;

void TaskHandleThinger( void *pvParameters );
void TaskGetSensor( void *pvParameters );
void TaskHandleSendToSheets (void *pvParameters );
void TaskHandleNotifLed (void *pvParameters );


// Konfigurasi Variabel pin
#define pinArusR 34
#define pinArusS 35
#define pinArusT 32
#define pinLedM  25
#define pinLedH  26
#define pinLedB  27

const char* ssid       = "IoT MONITORING";
const char* password   = "1234567890";

//const char* ssid       = "82 III baru";
//const char* password   = "#BayarDonk#";

//Konfigurasi Thinger.io
#define USERNAME "takahada"
#define DEVICE_ID "ESP32_monitoring"
#define DEVICE_CREDENTIAL "f5M#vb8TJR#8CzQO"

// Variabel thinger.io
ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

//Konfigurasi Google Sheet key
String GOOGLE_SCRIPT_ID = "AKfycbzNhDYJisL5nHn6wuRwf2654ybA44eyX9vGT_dtkZDBzgCgRd6WF2Yo7VxYBDTd5A8";

// Variabel pembacaan sensor
float X1 = 0;
float X2 = 0;
float N = 0;
float Pers25 = 0;
float Pers50 = 0;
float Irata2 = 0;
float Na = 0;
float Nb = 0;
float PersBeban = 0;
float HL25 = 113.5;
float HL50 = 227;
int persen = 100;

unsigned long interval2 = 21600000;
unsigned long waktuSebelum2 = 0;

unsigned long interval1 = 50000;
unsigned long waktuSebelum1 = 0;

bool StateLedM = true;
bool StateData = false;
bool Statefirst = false;

#include "InitialSetup.h"
#include "sendDataToSheet.h"
#include "sendDataToSheets.h"

WiFiClientSecure client;

void setup() {
  Serial.begin(115200);
  InitialSetup();
}

void loop() {

}

/*--------------------------------------------------*/
/*-------------- Tasks Handle Thinger --------------*/
/*--------------------------------------------------*/

void TaskHandleThinger(void *pvParameters) {
  (void) pvParameters;

  thing["Monitoring_Daya"] >> [] (pson & out) {
    out["X1"] = X1;
    out["X2"] = X2;
    out["N"] = N;
    out["Pers25"] = Pers25;
    out["Pers50"] = Pers50;
    out["Pers2"] = PersBeban;
  };

  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());

  for (;;) {

    thing.handle();
    Serial.print("Task1 berhasil ");
    Serial.println(xPortGetCoreID());
    Serial.println("");
    vTaskDelay(500);

  }
}

/*--------------------------------------------------*/
/*-------------- Tasks Get Data Sensor -------------*/
/*--------------------------------------------------*/

void TaskGetSensor(void *pvParameters) {
  (void) pvParameters;

  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());

  for (;;) {

    X1 = emon1.calcIrms(1480);
    X2 = emon2.calcIrms(1480);
    N = emon3.calcIrms(1480);
    Irata2 = (X1 + X2)/2;
    Pers25 = (Irata2 / HL25) * persen;
    Pers50 = (Irata2 / HL50) * persen;
    Na = X1 / Irata2;
    Nb = X2 / Irata2;
    float PersB = (abs(Na - 1) + abs(Nb - 1));
    PersBeban = (PersB / 2)*persen;

    Serial.print("Task2 berhasil ");
    Serial.println(xPortGetCoreID());
    Serial.println("");
    vTaskDelay(1000);
  }
}

/*--------------------------------------------------*/
/*---------- Tasks Handle Send To Sheets -----------*/
/*--------------------------------------------------*/

void TaskHandleSendToSheets (void *pvParameters ) {
  (void) pvParameters;

  for (;;) {
    unsigned long waktuSekarang1 = millis();
    unsigned long waktuSekarang2 = millis();
    
    String a(X1);
    String b(X2);
    String c(N);
    String d(Pers25);
    String e(Pers50);
    String f(PersBeban);

    if (((waktuSekarang1 - waktuSebelum1) >= interval1) && ( Statefirst == true)){
      waktuSebelum1 = waktuSekarang1;
      sendDataToSheets(a, b, c, d, e, f);
      Statefirst = false;
    }
    if ((waktuSekarang2 - waktuSebelum2) >= interval2){
      waktuSebelum2 = waktuSekarang2;
      sendDataToSheets(a, b, c, d, e, f);
      StateData = false;
      Serial.println(StateData);
    }
    
    vTaskDelay(1000);
  }
}

/*--------------------------------------------------*/
/*------------- Tasks Handle Notif Led -------------*/
/*--------------------------------------------------*/

void TaskHandleNotifLed (void *pvParameters ) {
  (void) pvParameters;

  for (;;) {
    if (StateLedM == false) {
      digitalWrite(pinLedH, HIGH);
      delay(150);
      digitalWrite(pinLedH, LOW);
      delay(150);
      digitalWrite(pinLedH, HIGH);
      delay(150);
      digitalWrite(pinLedH, LOW);
      delay(150);
      digitalWrite(pinLedH, HIGH);
      delay(150);
      digitalWrite(pinLedH, LOW);
      vTaskDelay(500);
    }
  }
}
