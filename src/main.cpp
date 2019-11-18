#include <Arduino.h>

#include "WiFi.h"

#include "AsyncUDP.h"

#include <string.h>

const char * ssid = "TELLO-FE2F96";
const char * password = "";
const int udpPort = 8889;

AsyncUDP udp;
WiFiUDP udpSender;

void sendMessage(String ip, int port, String message)
{
   udpSender.beginPacket(ip.c_str(), port);
   udpSender.printf(message.c_str());
   udpSender.endPacket(); 
    
}

void setup()
{
    Serial.begin(9600);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println("WiFi Failed");
        while(1) {
            delay(1000);
        }
    }
    if(udp.listen(udpPort)) {
        Serial.print("UDP Listening on IP: ");
        Serial.println(WiFi.localIP());
        udp.onPacket([](AsyncUDPPacket packet) {
            Serial.print("UDP Packet Type: ");
            Serial.print(packet.isBroadcast()?"Broadcast":packet.isMulticast()?"Multicast":"Unicast");
            Serial.print(", From: ");
            Serial.print(packet.remoteIP());
            Serial.print(":");
            Serial.print(packet.remotePort());
            Serial.print(", To: ");
            Serial.print(packet.localIP());
            Serial.print(":");
            Serial.print(packet.localPort());
            Serial.print(", Length: ");
            Serial.print(packet.length());
            Serial.print(", Data: ");
            Serial.write(packet.data(), packet.length());
            Serial.println();
            //reply to the client
            packet.printf("Got %u bytes of data", packet.length());

            //send reply
            
        });

        sendMessage("192.168.10.1", udpPort, "command");
    }
}

void loop()
{
    delay(5000);
    //Send broadcast
    udp.broadcast("Anyone here?");

    Serial.print("Send something to: ");
    Serial.print(WiFi.localIP());
    Serial.print(" on port: ");
    Serial.println(udpPort);
} 

