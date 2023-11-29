#include <ESP32Servo.h>
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Botbomber";
const char* password = "kall8900";

WebServer server(80);

String command;

//Motores izquierdos
//Motor de adelante 
int IN1I = 18;
int IN2I = 19;
int ENAI = 21;
//Motor de atrás
int IN3I = 15;
int IN4I = 2;
int ENBI = 4;
//Motores derechos
//Motor de adelante
int IN1D = 26;
int IN2D = 27;
int ENAD = 25;
//Motor de atrás 
int IN3D = 14;
int IN4D = 12;
int ENBD = 13;


//Servo arriba
Servo servo_up;
int pinservo_up=22;
//Servo abajo
Servo servo_down;
int pinservo_down = 23;

int last_value = 0;

int pin_sensor = 5;

void setup(){
  pinMode (IN1D, OUTPUT);    
  pinMode (IN2D, OUTPUT);    
  pinMode (IN3D, OUTPUT);
  pinMode (IN4D, OUTPUT);    
  pinMode (ENAD, OUTPUT);
  pinMode (ENBD, OUTPUT);
  pinMode (IN1I, OUTPUT);    
  pinMode (IN2I, OUTPUT);    
  pinMode (IN3I, OUTPUT);
  pinMode (IN4I, OUTPUT);
  pinMode (ENAI, OUTPUT);
  pinMode (ENBI, OUTPUT);
  servo_up.attach(pinservo_up, 500, 2500);
  servo_down.attach(pinservo_down, 500, 2500);
  servo_down.write(last_value);

  
  Serial.begin(57600);
  WiFi.mode(WIFI_AP);           
  WiFi.softAP(ssid, password); 

  IPAddress myIP = WiFi.softAPIP();
  //Serial.println("AP IP address: ");
  //Serial.println(myIP);
 
 // Starting WEB-server 
  server.on( "/", HTTP_handleRoot);
  server.onNotFound(HTTP_handleRoot);
  server.begin();
  Serial.println("Server inicializado");
}

void loop(){
  server.handleClient(); //escuchamos las conexiones entrantes:
  
  command = server.arg("State");
  
  if (command=="1"){
    adelante();
    Serial.println("Adelante");
    Serial.println(command);
  }
  else if(command=="2"){
    //atrasI();
    atras();
    Serial.println("Atras");
  }
  else if(command=="3"){
    parar();
    Serial.println("Parar");
  }
  else if(command=="4"){
    derecha();
    Serial.println("derecha");
  }
  else if(command=="5"){
    izquierda();
    Serial.println("izquierda");
  }
  else if(command=="9"){
    servo_up_up();
    Serial.println("Servo up up");
  }
  else if (command=="10"){
    servo_up_down();
    Serial.println("Servo up down");
  }
  else if (command=="11"){
    servo_down_up();
    Serial.println("Servo down up");
  }
  else if (command=="12"){
    servo_down_down();
    Serial.println("Servo down down");
  }
  delay(50);
}


void HTTP_handleRoot(void) {
  if( server.hasArg("State") ){
      Serial.println(server.arg("State"));
  }
  bool state_sensor = digitalRead(pin_sensor);
  if (state_sensor == LOW) {
    Serial.println("Detecta");
    enviarNotificacionMetal();
    parar();
  }
  server.send(200, "text/html", "No detecta");
  delay(1);
}

void enviarNotificacionMetal() {
  server.send(200, "text/plain", "Detectando");
  delay(1);
}

void atras(){
  digitalWrite (IN1I, LOW);
  digitalWrite (IN2I, HIGH);
  analogWrite(ENAI,150);
  digitalWrite (IN3I, LOW);
  digitalWrite (IN4I,HIGH);
  analogWrite(ENBI,150);
  digitalWrite (IN1D, HIGH);
  digitalWrite (IN2D, LOW);
  analogWrite(ENAD,150);
  digitalWrite (IN3D, HIGH);
  digitalWrite (IN4D,LOW);
  analogWrite(ENBD,150);
  delay(200);
}
void adelante(){
  digitalWrite (IN1D, LOW);
  digitalWrite (IN2D, HIGH);
  analogWrite(ENAD,150);
  digitalWrite (IN3D, LOW);
  digitalWrite (IN4D,HIGH);
  analogWrite(ENBD,150);
  digitalWrite (IN1I, HIGH);
  digitalWrite (IN2I, LOW);
  analogWrite(ENAI,150);
  digitalWrite (IN3I, HIGH);
  digitalWrite (IN4I,LOW);
  analogWrite(ENBI,150);
  delay(200);
}

void derecha(){
  digitalWrite (IN1I, HIGH);
  digitalWrite (IN2I, LOW);
  analogWrite(ENAI,150);
  digitalWrite (IN3I, HIGH);
  digitalWrite (IN4I,LOW);
  analogWrite(ENBI,150);
  delay(200);
}

void izquierda(){
  digitalWrite (IN1D, LOW);
  digitalWrite (IN2D, HIGH);
  analogWrite(ENAD,150);
  digitalWrite (IN3D, LOW);
  digitalWrite (IN4D,HIGH);
  analogWrite(ENBD,150);
}

void parar(){
  digitalWrite (IN1D, LOW);
  digitalWrite (IN2D, LOW);
  digitalWrite (IN3D, LOW);
  digitalWrite (IN4D,LOW);
  digitalWrite (IN1I, LOW);
  digitalWrite (IN2I, LOW);
  digitalWrite (IN3I, LOW);
  digitalWrite (IN4I,LOW);
  delay(200);
}
void servo_up_down(){
  for (int pos = 90; pos >= 10; pos -= 1) {
    servo_up.write(pos);
    delay(20);
  }
}
void servo_up_up(){
  for (int pos = 10; pos <= 90; pos += 1) {
    servo_up.write(pos);
    delay(20);
  }
}
void servo_down_up(){
    int pos = last_value+20;
    servo_down.write(pos);
    last_value=pos;
    Serial.println(last_value);
}
void servo_down_down(){
  int pos = last_value-20;
  servo_down.write(pos);
  last_value=pos;
  Serial.println(last_value);
}