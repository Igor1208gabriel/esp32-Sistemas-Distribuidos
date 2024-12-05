#include <WiFi.h>
#include <WiFiUdp.h>


const char* ssid = "Isgro";
const char* password = "12345678";

WiFiUDP udpClient;
WiFiServer tcpServer(3000); 
const char* udpServerIP = "192.168.164.255"; // IP do Desktop
const int udpServerPort = 4000;           // Porta do servidor UDP

const int ledPin = 2;   // GPIO do LED
const int trigPin = 13; // GPIO do Trig do sensor ultrassônico
const int echoPin = 12; // GPIO do Echo do sensor ultrassônico

void setup() {
    Serial.begin(115200);

    // Conectar ao Wi-Fi
    WiFi.begin(ssid, password);
    Serial.println("Conectando ao Wi-Fi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }

    Serial.println("\nWi-Fi conectado!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    tcpServer.begin();
    Serial.println("Servidor TCP iniciado.");

    pinMode(ledPin, OUTPUT);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

void loop() {
  bool led=false;
    WiFiClient client = tcpServer.available(); // TCP
    if (client) {
        Serial.println("Cliente conectado!");

        while (client.connected()) {
            if (client.available()) {

                String command = client.readStringUntil('\n');
                command.trim();
                Serial.println("Comando recebido: " + command);

                if (command == "LED_ON") {
                    ligaled();
                    //client.println("LED ligado."); TCP
                    Enviar("LED foi ligado!");
                    led=true;
                }   
                
                else if (command == "LED_OFF"){
                    desligaled();
                    //client.println("LED desligado."); TCP
                    Enviar("LED foi desligado!");
                    led=false;
                } 

                else if (command == "LED"){
                  if (led) {
                    desligaled();
                    //client.println("LED desligado."); TCP
                    Enviar("LED foi desligado!");
                    led=false;
                  }
                  else {
                    ligaled();
                    //client.println("LED ligado."); TCP
                    Enviar("LED foi ligado!");
                    led=true;
                  }
                }
                
                else if (command == "SENSOR") {
                    float distancia = leituraUS();
                    //client.printf("Distância: %.2f cm\n", distancia);   TCP
                    Enviar(std::to_string(distancia).c_str());
                }

                else if (command.startsWith("MORSE:")){
                  String mensagem = command.substring(6);
                  tradutor(mensagem);
                  Enviar("Morse concluído");
                }
                
                else {
                    client.println("Comando não encontrado.");
                }
            }
        }
        client.stop();
        Serial.println("Cliente desconectado.");
    }
}

// Função para leitura do sensor ultrassônico
float leituraUS() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH);
    float distance = (duration / 2.0) * 0.0343;
    return distance;
}

// Função para enviar mensagens de diagnóstico via UDP
void Enviar(const char* message) {
    udpClient.beginPacket(udpServerIP, udpServerPort);
    udpClient.print(message);
    udpClient.endPacket();
    Serial.println("Diagnóstico enviado via UDP.\n");
}

void ponto(){
  ligaled();
  delay(200);
  desligaled();
  delay(200);
}

void traco(){
  ligaled();
  delay(600);
  desligaled();
  delay(200);
}

void ligaled(){
  digitalWrite(ledPin, HIGH);
}

void desligaled(){
  digitalWrite(ledPin, LOW);
}

void tradutor(const String& mensagem) {
    for (char letra : mensagem) {
        switch (tolower(letra)) {
            case 'a': ponto(); traco(); break;
            case 'b': traco(); ponto(); ponto(); ponto(); break;
            case 'c': traco(); ponto(); traco(); ponto(); break;
            case 'd': traco(); ponto(); ponto(); break;
            case 'e': ponto(); break;
            case 'f': ponto(); ponto(); traco(); ponto(); break;
            case 'g': traco(); traco(); ponto(); break;
            case 'h': ponto(); ponto(); ponto(); ponto(); break;
            case 'i': ponto(); ponto(); break;
            case 'j': ponto(); traco(); traco(); traco(); break;
            case 'k': traco(); ponto(); traco(); break;
            case 'l': ponto(); traco(); ponto(); ponto(); break;
            case 'm': traco(); traco(); break;
            case 'n': traco(); ponto(); break;
            case 'o': traco(); traco(); traco(); break;
            case 'p': ponto(); traco(); traco(); ponto(); break;
            case 'q': traco(); traco(); ponto(); traco(); break;
            case 'r': ponto(); traco(); ponto(); break;
            case 's': ponto(); ponto(); ponto(); break;
            case 't': traco(); break;
            case 'u': ponto(); ponto(); traco(); break;
            case 'v': ponto(); ponto(); ponto(); traco(); break;
            case 'w': ponto(); traco(); traco(); break;
            case 'x': traco(); ponto(); ponto(); traco(); break;
            case 'y': traco(); ponto(); traco(); traco(); break;
            case 'z': traco(); traco(); ponto(); ponto(); break;
            case '1': ponto(); traco(); traco(); traco(); traco(); break;
            case '2': ponto(); ponto(); traco(); traco(); traco(); break;
            case '3': ponto(); ponto(); ponto(); traco(); traco(); break;
            case '4': ponto(); ponto(); ponto(); ponto(); traco(); break;
            case '5': ponto(); ponto(); ponto(); ponto(); ponto(); break;
            case '6': traco(); ponto(); ponto(); ponto(); ponto(); break;
            case '7': traco(); traco(); ponto(); ponto(); ponto(); break;
            case '8': traco(); traco(); traco(); ponto(); ponto(); break;
            case '9': traco(); traco(); traco(); traco(); ponto(); break;
            case '0': traco(); traco(); traco(); traco(); traco(); break;
            case '?': ponto(); ponto(); traco(); traco(); ponto(); ponto(); break;
            case '!': traco(); ponto(); traco(); ponto(); traco(); traco(); break;
            case '.': ponto(); traco(); ponto(); traco(); ponto(); traco(); break;
            case ',': traco(); traco(); ponto(); ponto(); traco(); traco(); break;
            case ';': traco(); ponto(); traco(); ponto(); traco(); ponto(); break;
            case ':': traco(); traco(); traco(); ponto(); ponto(); ponto(); break;
            case '+': ponto(); traco(); ponto(); traco(); ponto(); break;
            case '-': traco(); ponto(); ponto(); ponto(); ponto(); traco(); break;
            case '/': traco(); ponto(); ponto(); traco(); ponto(); break;
            case '=': traco(); ponto(); ponto(); ponto(); traco(); break;
            case ' ': delay(1400); break;
        }
        delay(600);
    }
}
