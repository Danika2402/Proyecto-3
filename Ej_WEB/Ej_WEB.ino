#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "iPhone (139)";  // Enter your SSID here
const char* password = "constantinopla";  //Enter your Password here

WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)

int infrarrojo = 26;
int Td;
int Te;

void setup() {
  Serial.begin(115200);
  Serial.println("Try Connecting to ");
  Serial.println(ssid);

  pinMode(infrarrojo, INPUT);
  // Connect to your wi-fi modem
  WiFi.begin(ssid, password);

  // Check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected successfully");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());  //Show ESP32 IP on serial

  server.on("/", handle_ON);
  //Serial.println("GPIO2 Status: ON");
  
  server.onNotFound(handle_NotFound);


  server.begin();
  Serial.println("HTTP server started");
  delay(100);
}

void loop() {
  server.handleClient();
  Td = digitalRead(infrarrojo);

  
}

//************************************************************************************************
// Handler de led1on
//************************************************************************************************
void handle_ON() {
  //Serial.println("GPIO2 Status: ON");
  server.send(200, "text/html", SendHTML(Td,Te));
}

//************************************************************************************************
// Procesador de HTML
//************************************************************************************************
String SendHTML(int SensorD,int SensorE) {
  String ptr = "<!DOCTYPE html>\n";
  ptr += "<html><head><title>Monitoreo de Parqueo</title>\n";
  ptr += "<meta http-equiv=refresh content=10>\n";
  ptr += "<style>*{box-sizing:border-box}body{font-family:Helvetica}Line{float:left;width:100%;height:10px;background:#ff0}header{background-color:#666;padding:30px;text-align:center;font-size:35px;color:black}nav{border-right:5px solid Tomato;border-left:5px solid Tomato;float:left;width:50%;height:200px;background:#ccc;font-size:150px;text-align:center}</style></head><body>\n";
  ptr += "<header><h2>&#128664 Parqueo &#128664</h2></header>\n";
  ptr += "<Line></Line><section>\n";

  if(SensorD == 0){
    ptr += "<nav>&#128665</nav>\n";
  }else{
    ptr += "<nav></nav>\n";
  }
  
  ptr += "<nav>&#128665</nav>\n";
  
  ptr += "</section><Line></Line><section>\n";
  
  ptr += "<nav>&#128665</nav>\n";
  ptr += "<nav>&#128665</nav>\n";
  
  ptr += "</section><Line></Line><section>\n";
  
  ptr += "<nav>&#128665</nav>\n";
  ptr += "<nav>&#128665</nav>\n";
  
  ptr += "</section><Line></Line><section>\n";
  
  ptr += "<nav>&#128665</nav>\n";
  ptr += "<nav>&#128665</nav>\n";
  
  ptr += "</section><Line></Line>\n";
  ptr += "</body></html>";

  return ptr;
}
//************************************************************************************************
// Handler de not found
//************************************************************************************************
void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}
