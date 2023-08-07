#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>

#include <Wire.h>
#include <Adafruit_MLX90614.h>

#include <Arduino.h>
#include <SoftwareSerial.h>

#define WIFI_SSID "<WIFI_NAME>"
#define WIFI_PASSWORD "<WIFI_PASSWORD>"

#define FIREBASE_HOST "limit-people-19e80-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_KEY "u5CwTtfZPrItD0pzhyTSAM0cj5DO1pbrnFe1p7Ar"

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
FirebaseData firebaseData;

const byte RX = D6;
const byte TX = D7;
SoftwareSerial mySerial = SoftwareSerial(RX, TX);

long lastUART = 0;
void Read_Uart();    // UART STM
int human = 0;
void setup() {
    connectWifi();
    Serial.begin(115200);
    mySerial.begin(115200);

    Serial.println("UART Start");

    lastUART = millis();
    Firebase.begin(FIREBASE_HOST, FIREBASE_KEY);
    mlx.begin();
}

void loop() {
    Read_Uart();
    if (millis() - lastUART > 1000)
    {
      mySerial.print(human);
      lastUART = millis();
    }
    callTemp();
}

String old_st = "start_default";

void Read_Uart()
{
  String st = ""; // จำนวนคนที่รับมาจาก STM32 : "1X, 2X"
  int cnt = 0;
  // oldst != st -> firebase
  while (mySerial.available())
  {
    char inChar = (char)mySerial.read();
    st +=  inChar;
    cnt++;
    if (inChar == 'X')
    {
      Serial.println(st);
      if(old_st != st){
        old_st = st;
        String fetchPeopleNumber = st;
        fetchPeopleNumber[cnt - 1] = '\0';
        postValueTofirebase(fetchPeopleNumber);
      }
      break;
    }
    
  }

}

void callTemp(){
    Serial.print("Ambient = "); 
    Serial.print(mlx.readAmbientTempC()); 
    Serial.print("*C\tObject = "); 
    Serial.print(mlx.readObjectTempC()); Serial.println("*C");
    if(mlx.readObjectTempC() > 30){
      Serial.println("HUMAN DETECT");
      human = 1;
    } else {
      Serial.println("NOTHING");
      human = 0;
    }
    Serial.println();
    delay(500);
}

void postValueTofirebase(String val){
    if(Firebase.set(firebaseData, "/count", val)) {
        Serial.println("Added");
    } else {
        Serial.println("Error : " + firebaseData.errorReason());
    }
}

void connectWifi() {
    Serial.begin(115200);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("connecting");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.println();
    Serial.print("connected: ");
    Serial.println(WiFi.localIP());
}
