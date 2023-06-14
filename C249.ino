#include <WiFi.h>
#include <Adafruit_BMP085.h>
//message queue telemetry transport
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#define WLAN_SSID "WRG3005N-7575"
#define WLAN_PASS "password"
#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883
#define AIO_USERNAME "niyatisingh"
#define AIO_KEY "aio_Hlxa99DNA90Cg3VZ9E45dcJYQmHc"
const int led1=18;
const int led2=19;
float p;
String s1,s2;
Adafruit_BMP085 bmp;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(10);
  pinMode(led1,OUTPUT);
  pinMode(led2,INPUT);
  digitalWrite(led1,LOW);
  digitalWrite(led2,LOW);
  Serial.println(F("AdafruitMQTT"));
  Serial.print("connecting");
  Serial.println(WLAN_SSID);
  WiFi.begin(WLAN_SSID,WLAN_PASS);
  while(WiFi.status()!=WL_CONNECTED){
    delay(500);

  }
  Serial.println("WiFi connected");
  Serial.print("ipaddress");
  Serial.print(WiFi.localIP());
  mqtt.subscribe(&sw1);
  mqtt.subscribe(&sw2);
  if(!bmp.begin()){
    Serial.println("bmp180 sersor not found");
    while(1){
      
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  p=bmp.readPresssure();
  q=bmp.readTemperature();
  Serial.println(p);
  Serial.println(q);
  MQTT_connect();

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &sw1) {
      //Serial.print(F("Got: "));
      //Serial.println((char *)sw1.lastread);
      //digitalWrite(led1, HIGH);
      
      stringOne = (char *)sw1.lastread;
      Serial.print(F("stringOne: "));
      Serial.println(stringOne);

      if (stringOne == "ON") {
        digitalWrite(led1, HIGH);
      }
      if (stringOne == "OFF") {
        digitalWrite(led1, LOW);
      }
    }

    if (subscription == &sw2) {
      //Serial.print(F("Got: "));
      //Serial.println((char *)sw2.lastread);
      //digitalWrite(led2, HIGH);

      stringTwo = (char *)sw2.lastread;
      Serial.print(F("stringTwo: "));
      Serial.println(stringTwo);

      if (stringTwo == "ON") {
        digitalWrite(led2, HIGH);
      }
      if (stringTwo == "OFF") {
        digitalWrite(led2, LOW);
      }

    }
  }


  if (! temperature.publish(q)) {
    //Serial.println(F("Temp Failed"));
  } else {
    //Serial.println(F("Temp OK!"));
  }

  if (! level.publish(p)) {
    //Serial.println(F("pressure Level Failed"));
  } else {
    //Serial.println(F("pressure Level OK!"));
  }


}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  //Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    //Serial.println(mqtt.connectErrorString(ret));
    //Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }

}
