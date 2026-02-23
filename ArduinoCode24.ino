#include <WiFi.h>
#include <WebServer.h>

/* -------- WiFi Credentials -------- */
const char* ssid = "SIMRAN";
const char* password = "simran24";

/* -------- LED PIN CONFIG -------- */
// CAM-1
#define C1_RED     13
#define C1_YELLOW  4
#define C1_GREEN   12

// CAM-2
#define C2_RED     14
#define C2_YELLOW  18
#define C2_GREEN   27

// CAM-3
#define C3_RED     26
#define C3_YELLOW  19
#define C3_GREEN   25

// CAM-4
#define C4_RED     33
#define C4_YELLOW  21
#define C4_GREEN   32

WebServer server(80);

/* -------- SIGNAL FUNCTION -------- */
void setSignal(int r, int y, int g, int state) {
  digitalWrite(r, state == 0 ? HIGH : LOW);
  digitalWrite(y, state == 1 ? HIGH : LOW);
  digitalWrite(g, state == 2 ? HIGH : LOW);
}

/* -------- ROUTES -------- */
void setupRoutes() {

  // MAIN WEB PAGE (Buttons UI)
  server.on("/", []() {
    String html =
      "<!DOCTYPE html><html><head>"
      "<meta name='viewport' content='width=device-width, initial-scale=1'>"
      "<title>ESP32 Traffic</title>"
      "<style>body{font-family:Arial;text-align:center}"
      "button{padding:15px 25px;margin:5px;font-size:18px}</style>"
      "</head><body><h1>ESP32 Traffic Controller</h1>";

    html += "<h2>CAM-1</h2>";
    html += "<a href='/cam-1/red'><button>RED</button></a>";
    html += "<a href='/cam-1/yellow'><button>YELLOW</button></a>";
    html += "<a href='/cam-1/green'><button>GREEN</button></a>";

    html += "<h2>CAM-2</h2>";
    html += "<a href='/cam-2/red'><button>RED</button></a>";
    html += "<a href='/cam-2/yellow'><button>YELLOW</button></a>";
    html += "<a href='/cam-2/green'><button>GREEN</button></a>";

    html += "<h2>CAM-3</h2>";
    html += "<a href='/cam-3/red'><button>RED</button></a>";
    html += "<a href='/cam-3/yellow'><button>YELLOW</button></a>";
    html += "<a href='/cam-3/green'><button>GREEN</button></a>";

    html += "<h2>CAM-4</h2>";
    html += "<a href='/cam-4/red'><button>RED</button></a>";
    html += "<a href='/cam-4/yellow'><button>YELLOW</button></a>";
    html += "<a href='/cam-4/green'><button>GREEN</button></a>";

    html += "</body></html>";
    server.send(200, "text/html", html);
  });

  // CAM ROUTES
  server.on("/cam-1/red",    [](){ setSignal(C1_RED,C1_YELLOW,C1_GREEN,0); server.send(200,"text/plain","CAM-1 RED"); });
  server.on("/cam-1/yellow", [](){ setSignal(C1_RED,C1_YELLOW,C1_GREEN,1); server.send(200,"text/plain","CAM-1 YELLOW"); });
  server.on("/cam-1/green",  [](){ setSignal(C1_RED,C1_YELLOW,C1_GREEN,2); server.send(200,"text/plain","CAM-1 GREEN"); });

  server.on("/cam-2/red",    [](){ setSignal(C2_RED,C2_YELLOW,C2_GREEN,0); server.send(200,"text/plain","CAM-2 RED"); });
  server.on("/cam-2/yellow", [](){ setSignal(C2_RED,C2_YELLOW,C2_GREEN,1); server.send(200,"text/plain","CAM-2 YELLOW"); });
  server.on("/cam-2/green",  [](){ setSignal(C2_RED,C2_YELLOW,C2_GREEN,2); server.send(200,"text/plain","CAM-2 GREEN"); });

  server.on("/cam-3/red",    [](){ setSignal(C3_RED,C3_YELLOW,C3_GREEN,0); server.send(200,"text/plain","CAM-3 RED"); });
  server.on("/cam-3/yellow", [](){ setSignal(C3_RED,C3_YELLOW,C3_GREEN,1); server.send(200,"text/plain","CAM-3 YELLOW"); });
  server.on("/cam-3/green",  [](){ setSignal(C3_RED,C3_YELLOW,C3_GREEN,2); server.send(200,"text/plain","CAM-3 GREEN"); });

  server.on("/cam-4/red",    [](){ setSignal(C4_RED,C4_YELLOW,C4_GREEN,0); server.send(200,"text/plain","CAM-4 RED"); });
  server.on("/cam-4/yellow", [](){ setSignal(C4_RED,C4_YELLOW,C4_GREEN,1); server.send(200,"text/plain","CAM-4 YELLOW"); });
  server.on("/cam-4/green",  [](){ setSignal(C4_RED,C4_YELLOW,C4_GREEN,2); server.send(200,"text/plain","CAM-4 GREEN"); });
}

/* -------- SETUP -------- */
void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(C1_RED,OUTPUT); pinMode(C1_YELLOW,OUTPUT); pinMode(C1_GREEN,OUTPUT);
  pinMode(C2_RED,OUTPUT); pinMode(C2_YELLOW,OUTPUT); pinMode(C2_GREEN,OUTPUT);
  pinMode(C3_RED,OUTPUT); pinMode(C3_YELLOW,OUTPUT); pinMode(C3_GREEN,OUTPUT);
  pinMode(C4_RED,OUTPUT); pinMode(C4_YELLOW,OUTPUT); pinMode(C4_GREEN,OUTPUT);

  // Default ALL RED
  setSignal(C1_RED,C1_YELLOW,C1_GREEN,0);
  setSignal(C2_RED,C2_YELLOW,C2_GREEN,0);
  setSignal(C3_RED,C3_YELLOW,C3_GREEN,0);
  setSignal(C4_RED,C4_YELLOW,C4_GREEN,0);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());

  setupRoutes();
  server.begin();
  Serial.println("HTTP Server Started");
}

/* -------- LOOP -------- */
void loop() {
  server.handleClient();
}
