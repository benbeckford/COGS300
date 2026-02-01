#include "WiFiS3.h"
#include <WebSocketsServer.h>
#include <ArduinoMDNS.h>

const int enA = 9;
const int in1 = 8;
const int in2 = 7;
const int enB = 10;
const int in3 = 6;
const int in4 = 5;

WiFiServer server(80);
WebSocketsServer webSocket(81);
WiFiUDP udp;
MDNS mdns(udp);

void setup() {
    pinMode(enA, OUTPUT);
    pinMode(enB, OUTPUT);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);

    while (WiFi.status() != WL_CONNECTED) {
        WiFi.begin("robot300", "robot300");
        delay(5000);
    }

    server.begin();
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);

    mdns.begin(WiFi.localIP(), "arduino");
    mdns.addServiceRecord("server._http", 80, MDNSServiceTCP);
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
    mdns.run();
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

                    client.print("<script>fetch(\"https://benbeckford.github.io/COGS300/\").then(r=>r.text()).then(d=>{document.open();document.write(d);document.close();})</script>");

                    client.println();
                    break;
                }

                last = c;
            }
        }
        
        client.stop();
    }
}
