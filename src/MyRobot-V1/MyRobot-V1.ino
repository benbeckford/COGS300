#include "WiFiS3.h"
#include <WebSocketsServer.h>

const int enA = 9;
const int in1 = 8;
const int in2 = 7;
const int enB = 10;
const int in3 = 6;
const int in4 = 5;

IPAddress ip(192, 168, 4, 1);
WiFiServer server(80);
WebSocketsServer webSocket(81);


void setup() {
    Serial.begin(9600);

    pinMode(enA, OUTPUT);
    pinMode(enB, OUTPUT);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);

    WiFi.config(ip);
    WiFi.beginAP("COGS 300 Robot", "");

    delay(5000);

    server.begin();
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    if (type == WStype_BIN && length == 2) {
        uint8_t motor = *payload & 0xF0;
        uint8_t dir = *payload & 0xF;

        digitalWrite(motor ? in3 : in1, dir ? HIGH : LOW);
        digitalWrite(motor ? in4 : in2, dir ? LOW : HIGH);
        analogWrite(motor ? enB : enA, payload[1]);
    }
}

void loop() {
    webSocket.loop();

    WiFiClient client = server.available();

    if (client) {
        char last = 0;

        while (client.connected()) {
            if (client.available()) {
                char c = client.read();

                if ((c == '\r' || c == '\n') && (last == '\r' || last == '\n')) {
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-type:text/html");
                    client.println();

                    client.print("Hello, world!");

                    client.println();
                    break;
                }

                last = c;
            }
        }
        
        client.stop();
    }
}
