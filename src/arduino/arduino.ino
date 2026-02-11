#include "WiFiS3.h"
#include <WebSocketsServer.h>
#include <ArduinoMDNS.h>

const int enA = 9; // right motor
const int in1 = 8;
const int in2 = 7;
const int enB = 10; // left motor
const int in3 = 6;
const int in4 = 5;
const int e1 = 12;
const int e2 = 13;

const int centre_pot = A0;
const int left_pot = A1;
const int right_pot = A2;

int left_rotations = 0;
bool left_rotating = false;
int right_rotations = 0;
bool right_rotating = false;

int centre_measurements[5] = { 0, 0, 0, 0, 0 };
int left_measurements[5] = { 0, 0, 0, 0, 0 };
int right_measurements[5] = { 0, 0, 0, 0, 0 };

WiFiServer server(80);
WebSocketsServer webSocket(81);
WiFiUDP udp;
MDNS mdns(udp);

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);

  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(e1, INPUT_PULLUP);
  pinMode(e2, INPUT_PULLUP);
  pinMode(centre_pot, INPUT);
  pinMode(left_pot, INPUT);
  pinMode(right_pot, INPUT);

  Serial.println("Hello");


  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  while (WiFi.status() != WL_CONNECTED) {
    WiFi.begin("robot300", "robot300");
    delay(1000);
  }  

  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  mdns.begin(WiFi.localIP(), "arduino");
  mdns.addServiceRecord("server._http", 80, MDNSServiceTCP);

  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
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
  int read1 = digitalRead(e2);
  if (read1 == HIGH && !left_rotating) {
    left_rotating = true;
  } else if (read1 == LOW && left_rotating) {
    left_rotating = false;
    left_rotations++;
    Serial.print("Left motor has turned ");
    Serial.print(left_rotations);
    Serial.println("cm");
    webSocket.broadcastTXT("L:" + String(left_rotations));
  }

  int read2 = digitalRead(e1);
  if (read2 == HIGH && !right_rotating) {
    right_rotating = true;
  } else if (read2 == LOW && right_rotating) {
    right_rotating = false;
    right_rotations++;
    Serial.print("Right motor has turned ");
    Serial.print(right_rotations);
    webSocket.broadcastTXT("R:" + String(right_rotations));
  }

  /*
  for (int i = 0; i < left_measurements.length - 1; i++)
    left_measurements[i + 1] = left_measurements[i];
  left_measurements[0] = left_measurements(left_pot);

  int left_average = 0;
  for (int i = 0; i < left_measurements.length; i++)
    left_average += left_measurements[i];
  left_average /= left_measurements.length;

  for (int i = 0; i < right_measurements.length - 1; i++)
    right_measurements[i + 1] = right_measurements[i];
  right_measurements[0] = analogRead(right_pot);

  int right_average = 0;
  for (int i = 0; i < right_measurements.length; i++)
    right_average += right_measurements[i];
  right_average /= right_measurements.length;


  for (int i = 0; i < centre_measurements.length - 1; i++)
    centre_measurements[i + 1] = centre_measurements[i];
  centre_measurements[0] = analogRead(centre_pot);

  int centre_average = 0;
  for (int i = 0; i < centre_measurements.length; i++)
    centre_average += centre_measurements[i];
  centre_average /= centre_measurements.length;

  if (centre_average >= 950) {
    analogWrite(enA, 255);
    analogWrite(enB, 255);
  } else if (left_average >= 700) {
    analogWrite(enA, 63);
    analogWrite(enB, 255);
  } else if (right_average >= 800) {
    analogWrite(enA, 255);
    analogWrite(enB, 63);
  } else {
    analogWrite(enA, 0);
    analogWrite(enB, 0);
  }


  // left floor: 550
  // right floor: 950
  // centre floor: 900


  // left tape: 
  // right tape: 1015
  // centre tape: 970
  
  Serial.print("Left potentiometre:");
  Serial.print(analogRead(left_pot));
  Serial.println();

  Serial.print("Right potentiometre:");
  Serial.print(analogRead(right_pot));
  Serial.println();

  Serial.print("Centre potentiometre:");
  Serial.print(analogRead(centre_pot));
  Serial.println();
  return;

  delay(20);*/

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
