/*
 *  This sketch sends a message to a TCP server
 *
 */

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

ESP8266WiFiMulti WiFiMulti;

const uint16_t port = 8888; // port tương ứng với server
const char * host = "192.168.0.169"; // ip của raspberry
int a[8][2] = {{5,0} ,{4,0} ,{0,0} ,{2,0} ,{14,0} ,{12,0} ,{13,0} ,{15,0}};
void setup() {
    for(int i = 0 ; i< 8 ;i++){
        pinMode(a[i][0],OUTPUT);
      }  
    Serial.begin(115200);
    delay(10);

    // We start by connecting to a WiFi network
    WiFiMulti.addAP("TP-LINK_M5360_D515D0", "19971008");

    Serial.println();
    Serial.println();
    Serial.print("Wait for WiFi... ");

    while(WiFiMulti.run() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    delay(500);
}


void loop() {

    Serial.print("connecting to ");
    Serial.println(host);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;

    if (!client.connect(host, port)) {
        Serial.println("connection failed");
        Serial.println("wait 5 sec...");
        delay(5000);
        return;
    }
      while(1)
     {
      //doc message tu server
      if(client.available()){
      String str = client.readString();
        int led = (int)str[0]-'0';
        int stt = (int)str[1]-'0';
        // thay doi trang thai led 
        a[led-1][1] = stt;
        digitalWrite(a[led-1][0],a[led-1][1]);
      }
      //gui mang trang thai led server
      String s = "";
      for(int i = 0 ; i<8;i++){
        s += a[i][1];
      }
      for(int i = 0 ; i< 8 ;i++){
        Serial.print(a[i][1]);
      }
      // gui message cho server
      client.print(s); 
      }

    Serial.println("closing connection");
    client.stop();
    
    Serial.println("wait 5 sec...");
    delay(5000);
}

