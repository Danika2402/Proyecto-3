
#include <WiFi.h>
#include <WebServer.h>
#include "SevSeg.h"
#include "config.h"

SevSeg sevseg;

#define RXD1 4
#define TXD1 2

const char* ssid = "iPhone (139)";  // Enter your SSID here
const char* password = "constantinopla";  //Enter your Password here

WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)

char Td[4];
char Te[4];
//char Te;
int Disp;

void setup() {
  
  Serial.begin(115200);
  Serial2.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, RXD1, TXD1);
  //*****************************************************Coneccion DISPLAY*****************************************************
  
  byte sevenSegments = 1;
  byte CommonPins[] = {}; //a  b  c  d  e  f  g
  byte Ledsegmentpins[] = {33,32,27,14,12,25,26};
  
  bool resistoronsegment = true;

  sevseg.begin(COMMON_ANODE, sevenSegments, CommonPins, Ledsegmentpins,resistoronsegment);
  sevseg.setBrightness(80);
  
  //*************************************************Coneccion WIFI************************************************************
  Serial.println("Try Connecting to ");
  Serial.println(ssid);

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
  
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
  delay(100);
  
}

//*****************************************************************************************************************************
void loop() {
  server.handleClient();
  Serial1.write('h');
  if(Serial1.available()>0){
    //Td = Serial2.read();
    Serial1.readBytesUntil('\n',Td,4);
  }
  
  Serial2.write('k');
  if(Serial2.available()>0){
    //Te = Serial2.read();
    Serial2.readBytesUntil('\n',Te,4);
  }


  Disp = 8 - !((int)Td[0]) - !(((int)Td[1])/2) - !(((int)Td[2])/4) - !(((int)Td[3])/8)
             - !((int)Te[0]) - !(((int)Te[1])/2) - !(((int)Te[2])/4) - !(((int)Te[3])/8);
  
  sevseg.setNumber(Disp);
  sevseg.refreshDisplay();
   
  Disp = 0;
    
  delay(10); 
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
String SendHTML(char SensorD[4],char SensorE[4]) {
  String ptr = "<!DOCTYPE html>\n";
  ptr += "<html><head><title>Monitoreo de Parqueo</title>\n";
  ptr += "<meta http-equiv=refresh content=10>\n";
  ptr += "<style>*{box-sizing:border-box}body{font-family:Helvetica}Line{float:left;width:100%;height:10px;background:#ff0}header{background-color:#666;padding:30px;text-align:center;font-size:35px;color:black}nav{border-right:5px solid Tomato;border-left:5px solid Tomato;float:left;width:50%;height:200px;background:#ccc;font-size:150px;text-align:center}</style></head><body>\n";
  ptr += "<header><h2>&#128664 Parqueo &#128664</h2></header>\n";
  ptr += "<Line></Line><section>\n";

  if((int)SensorD[0] == 0){
    ptr += "<nav>&#128665</nav>\n";
  }else{
    ptr += "<nav></nav>\n";
  }
  
  if((int)SensorD[1] == 0){
    ptr += "<nav>&#128665</nav>\n";
  }else{
    ptr += "<nav></nav>\n";
  }
  
  ptr += "</section><Line></Line><section>\n";
  
  if((int)SensorD[2] == 0){
    ptr += "<nav>&#128665</nav>\n";
  }else{
    ptr += "<nav></nav>\n";
  }

  if((int)SensorD[3] == 0){
    ptr += "<nav>&#128665</nav>\n";
  }else{
    ptr += "<nav></nav>\n";
  }
  
  ptr += "</section><Line></Line><section>\n";
  
  if((int)SensorE[0] == 0){
    ptr += "<nav>&#128665</nav>\n";
  }else{
    ptr += "<nav></nav>\n";
  }
  
  if((int)SensorE[1] == 0){
    ptr += "<nav>&#128665</nav>\n";
  }else{
    ptr += "<nav></nav>\n";
  }
  
  ptr += "</section><Line></Line><section>\n";
  
  if((int)SensorE[2] == 0){
    ptr += "<nav>&#128665</nav>\n";
  }else{
    ptr += "<nav></nav>\n";
  }
  
  if((int)SensorE[3] == 0){
    ptr += "<nav>&#128665</nav>\n";
  }else{
    ptr += "<nav></nav>\n";
  }
  
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
