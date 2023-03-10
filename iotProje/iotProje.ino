
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ThingSpeak.h>
#include <BlynkSimpleEsp8266.h> 
#include <Arduino.h>
//blynk
#define BLYNK_TEMPLATE_ID "BLYNK_TEMPLATE_ID"
#define BLYNK_DEVICE_NAME "BLYNK_DEVICE_NAME"
char auth[] = "AUTH_KEY";
#define BLYNK_PRINT Serial 

// Wifi bilgileri
const char* ssid = "WIFI_NAME";
const char* password = "WIFI_PASSWORD";

// telegram kurulumu
#define BOTtoken "BOT_TOKEN"
#define CHAT_ID "CHAT_ID"

//pin tanimlamalari
#define gazSensoru A0
#define pirSensor D0
#define LEDR D1
#define Buzzer D7

// degisken tanimlamalari
int gazSensoruDegeri;
int x;



X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

void setup() {
  Serial.begin(9600);
  configTime(0, 0, "pool.ntp.org");      
  client.setTrustAnchors(&cert);

  pinMode(pirSensor, INPUT);
  pinMode(LEDR, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  int a = 0;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
    a++;
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  delay(500);
  
  bot.sendMessage(CHAT_ID, "Akıllı Hareket Sensörü Aktifleştirildi", "");
  Serial.println("Akilli Hareket Sensörü Aktiflestirildi");
  delay(100);
  Blynk.begin(auth, ssid, password,"blynk.cloud",80); 

  
}
int GazSensoru(){
 { 
  gazSensoruDegeri = analogRead(gazSensoru); 
;
  if(gazSensoruDegeri > 600)
  { Serial.println("Gaz Sensoru degeri 600 den buyuk");
    Serial.println("Gaz kacagi tespit edildi, evinizi kontrol ediniz.");
    bot.sendMessage(CHAT_ID, "Gaz kacagi tespit edildi, evinizi kontrol ediniz.", "");
  }

  return gazSensoruDegeri;
}
  }
void pirSensoru(){
  bool pirDegeri = digitalRead(pirSensor);
  if (pirDegeri == 1) {
    Serial.println("Hareket Algilandi ve Mesajiniz Gonderildi..");
    digitalWrite(LEDR, HIGH);
   digitalWrite(Buzzer, HIGH);
   
    bot.sendMessage(CHAT_ID, "Evinizde Hareket Algılandı!!", "");
  } 
  else if (pirDegeri == 0) {
    
    digitalWrite(LEDR, LOW);
    digitalWrite(Buzzer, LOW);
  }
  delay(1000);
  }
void GazMiktari(){
  gazSensoruDegeri = GazSensoru();
  Serial.print("blynk Gonderilen Gaz Değeri: "); 
  Serial.println(gazSensoruDegeri);
  
  delay(600);
    
   
}
  
void loop() {
  pirSensoru();
  GazMiktari();
  Blynk.run(); 
}
