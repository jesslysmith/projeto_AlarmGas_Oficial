#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "Led.h"
#include "Buzzer.h"
#include <LiquidCrystal.h>
const int sensor = A0;
Led led(D1);
Buzzer buzzer(D2);
const char* SSID = "Kramer"; // Nome da rede WiFi
const char* PASS = "kramer123"; // Senha da red WiFi
HTTPClient http;
const String LOCATION = "http://alarmgas.herokuapp.com/"; // Link do servidor web
const String UPLOAD = "upload"; 
const String DOWNLOAD = "download";
const String LED = "\"LED\":";
const String BUZZER = "\"BUZZER\":";
const String T_OFF = "\"T_OFF\":";
const String SENSOR = "\"SENSOR\":";
const int gatilhosensor = 600; // Valor de gatilho
const int rs = D3, en = D0, d4 = D4, d5 = D5, d6 = D6, d7 = D7; // Declaração dos pinos para o LCD
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

String JSON_INDICATORS(){ // Função que retorna o estado do LED e do BUZZER

   return "{" +
      LED + String(led.status()) + "," +
      BUZZER + String(buzzer.status()) +
      "}";
 }
 
String JSON_SENSOR(){ // Função que retorna o valor medido pelo sensor MQ-2
 int sensor_value = analogRead(sensor);
 
    return "{" +
      SENSOR + String(sensor_value) +
      "}";
}

void desligarAlarme(String comando){ // Função para desligar o alarme
   if(comando.startsWith("{") && comando.endsWith("}\n")){
      if(comando.indexOf(T_OFF + " 0") > 0){
          led.apagar(); // Metódo da classe Led.h para apagar o LED
          buzzer.desligar(); // Metódo da classe Buzzer.h para desligar o Buzzer
          lcd.clear();
          lcd.print("Em funcionamento!");
      }
   }
}

void uploadINDICATORS(){ // Função que manda informações(atualiza) dos estados do LED e do BUZZER para o servidor WEB

   http.begin(LOCATION+UPLOAD);
   http.addHeader("Content-Type", "application/json");
   http.POST(JSON_INDICATORS());
   http.end();
}

void uploadSENSOR(){ // Função que manda informações(atualiza) dos valores medidos pelo sensor MQ-2 para o servidor WEB

   http.begin(LOCATION+UPLOAD);
   http.addHeader("Content-Type", "application/json");
   http.POST(JSON_SENSOR());
   http.end();
}

String downloadWEB(){ // Função que retorna informações do servidor WEB para o circuito 

   http.begin(LOCATION+DOWNLOAD);
   http.GET();
   String payload = http.getString();
   http.end();
   if(payload[0] == '0'){ // Condição para o alarme apagar através do servidor WEB
       led.apagar();
       buzzer.desligar();
       lcd.clear();
       lcd.print("Em funcionamento!");
   }
   return payload;

}

void setup() {
  pinMode(sensor, INPUT);
  Serial.begin(115200);
  lcd.begin(16,2);
  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Aguardando conexão");
  }
  delay(2000);
}

void loop() {
  int analogSensor = analogRead(sensor); // Leitura dos dados do sensor MQ-2
  if(Serial.available() > 0){ // Verifica se há algo na barra de comando serial
        String comando = Serial.readString();
        desligarAlarme(comando);
        Serial.print(JSON_SENSOR());
        uploadSENSOR();
        delay(10);
        Serial.print(JSON_INDICATORS());
        uploadINDICATORS();
  }
  lcd.clear(); // Limpa o display LCD
  lcd.print("Em funcionamento!"); // Escreve a mensagem "Em funcionamento" no display
  lcd.setCursor(0, 1); // Coloca o cursor do LCD na 1ª coluna e 2ª linha
  delay (200);
  Serial.print(JSON_SENSOR());
  uploadSENSOR(); // Atualiza o valor do sensor no servidor WEB
  delay(10);
  Serial.print(JSON_INDICATORS()); // Atualiza os estados do LED e do BUZZER no servidor WEB
  uploadINDICATORS();
  
  if(analogSensor > gatilhosensor) // confronto entre valor de gatilho e o valor recebido pelo sensor
  {
    led.acender(); // Metódo da classe Led.h para acender o LED
    buzzer.acionar(); // Metódo da classe Buzzer.h para acionar o BUZZER
    Serial.print(JSON_SENSOR());
    uploadSENSOR(); // Atualiza o valor do sensor no servidor WEB
    delay(10);
    Serial.print(JSON_INDICATORS());
    uploadINDICATORS(); // Atualiza os estados do LED e do BUZZER no servidor WEB
    delay(10);
    while(led.status() == 1){ // Estado do LED = 1
      lcd.clear(); // Limpa o display LCD
      lcd.print("PERIGO! NIVEL "); // Escreve a mensagem "PERIGO! NIVEL " no display
      lcd.setCursor(0, 1); // Coloca o cursor do LCD na 1ª coluna e 2ª linha
      lcd.print ("ALARMANTE!"); // Escreve a mensagem "ALARMANTE" no display
      Serial.print(JSON_SENSOR());
      uploadSENSOR(); // Atualiza o valor do sensor no servidor WEB
      delay(10);
      Serial.print(JSON_INDICATORS());
      uploadINDICATORS(); // Atualiza os estados do LED e do BUZZER no servidor WEB
      delay(1000);
      if(Serial.available() > 0){
        String comando = Serial.readString();
        desligarAlarme(comando);
        Serial.print(JSON_SENSOR());
        uploadSENSOR(); // Atualiza o valor do sensor no servidor WEB
        delay(10);
        Serial.print(JSON_INDICATORS());
        uploadINDICATORS(); // Atualiza os estados do LED e do BUZZER no servidor WEB
      }
      desligarAlarme(downloadWEB()); // Atualiza as informações mandadas pelo servidor WEB
    }
    analogSensor = analogRead(sensor); // Leitura dos dados do sensor MQ-2
    delay (1000);
  }
   desligarAlarme(downloadWEB()); // Atualiza as informações mandadas pelo servidor WEB
   uploadSENSOR(); // Atualiza o valor do sensor no servidor WEB
   delay(10);
   uploadINDICATORS(); // Atualiza os estados do LED e do BUZZER no servidor WEB
   
   delay (3000);
}
