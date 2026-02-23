#include "WiFiS3.h"
#include <WebSocketsServer.h>
#include <ArduinoMDNS.h>
#include "pins.h"
#include "mode.h"
#include "standby.h"
#include "rc.h"

WiFiServer server(80);
WebSocketsServer webSocket(81);
WiFiUDP udp;
MDNS mdns(udp);

Mode *mode;

void setup() {
  pinMode(LEFT_MOTOR_POWER, OUTPUT);
  pinMode(RIGHT_MOTOR_POWER, OUTPUT);
  pinMode(LEFT_MOTOR_FORWARD, OUTPUT);
  pinMode(LEFT_MOTOR_BACKWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_FORWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_BACKWARD, OUTPUT);
  pinMode(LEFT_ENCODER, INPUT_PULLUP);
  pinMode(RIGHT_ENCODER, INPUT_PULLUP);
  pinMode(LEFT_ULTRASONIC_TRIG, OUTPUT);
  pinMode(LEFT_ULTRASONIC_ECHO, INPUT);
  pinMode(FRONT_ULTRASONIC_TRIG, OUTPUT);
  pinMode(FRONT_ULTRASONIC_ECHO, INPUT);
  pinMode(LEFT_INFRARED, OUTPUT);
  pinMode(RIGHT_INFRARED, OUTPUT);

  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  mdns.addServiceRecord("server._http", 80, MDNSServiceTCP);

  mode = new Standby();
}

void loop() {
  mode->loop();

  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin("robot300", "robot300");
    delay(3000);

    if (WiFi.status() == WL_CONNECTED) {
      mdns.begin(WiFi.localIP(), "arduino");
      Serial.println("Connected successfully!");
    } else {
      return;
    }
  }

  mdns.run();
  webSocket.loop();
  serveClientBootstrap();
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  if (type != WStype_BIN)
    return;

  if (length == 3 && payload[0] == 0xCA && payload[1] == 0xFE) {
    switch (payload[2]) {
      case 0:
        delete mode;
        mode = new Standby();
        return;
      case 1:
        delete mode;
        mode = new RemoteControl();
        return;
      default:
        break;
    }
  }

  mode->event(payload, length);
}

void serveClientBootstrap() {
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
