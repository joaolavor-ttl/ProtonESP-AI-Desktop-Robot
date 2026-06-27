#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "FluxGarage_RoboEyes.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1

// Pinos I2C ESP32-S3 Zero
#define OLED_SDA 8
#define OLED_SCL 9

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
RoboEyes<Adafruit_SSD1306> roboEyes(display);

// Variáveis para o controle aleatório
unsigned long tempoUltimaTroca = 0;
unsigned long intervaloAtual = 4000; // Começa com 4 segundos
int humorAtual = 0;                  // 0:DEFAULT, 1:HAPPY, 2:ANGRY, 3:TIRED

void setup() {
  Serial.begin(115200);
  Wire.begin(OLED_SDA, OLED_SCL);

  // Alimenta o gerador aleatório usando o ruído do clock interno
  randomSeed(micros());

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Erro: OLED não encontrado."));
    for(;;); 
  }
  
  display.clearDisplay();
  display.display();

  // 1. Inicializa o motor interno de animação em 128x64
  roboEyes.begin(SCREEN_WIDTH, SCREEN_HEIGHT, 100);

  // 2. CONFIGURAÇÃO DE OLHOS MENORES E PROPORCIONAIS
  roboEyes.setWidth(32, 32);       
  roboEyes.setHeight(32, 32);      
  roboEyes.setSpacebetween(12);    
  roboEyes.setBorderradius(6, 6);  

  // 3. Centralização automática nativa
  roboEyes.setMood(DEFAULT);       
  roboEyes.setPosition(DEFAULT);   

  // 4. Ativação das automações
  roboEyes.setAutoblinker(ON, 3, 2); 
  roboEyes.setIdleMode(ON, 2, 2);    
  
  Serial.println("RoboEyes Inicializado em modo Sorteio Aleatório!");
}

void loop() {
  // Roda as animações mantendo os novos tamanhos salvos
  roboEyes.update();

  // Verifica se o tempo aleatório determinado para essa expressão acabou
  if (millis() - tempoUltimaTroca >= intervaloAtual) {
    tempoUltimaTroca = millis();
    
    int novoHumor;
    
    // Sorteia um novo humor até que ele seja DIFERENTE do humor atual
    do {
      novoHumor = random(4); // Sorteia de 0 a 3
    } while (novoHumor == humorAtual);
    
    humorAtual = novoHumor; // Atualiza o humor atual

    // Aplica o humor sorteado
    switch (humorAtual) {
      case 0:
        roboEyes.setMood(DEFAULT);
        Serial.println("Sorteio: DEFAULT 👀");
        break;
      /*case 1:
        roboEyes.setMood(HAPPY);
        Serial.println("Sorteio: HAPPY 😊");
        break;
      case 2:
        roboEyes.setMood(ANGRY);
        Serial.println("Sorteio: ANGRY 😠");
        break;
      case 3:
        roboEyes.setMood(TIRED);
        Serial.println("Sorteio: TIRED 😑");
        break;*/
    }

    // Sorteia quanto tempo (em milissegundos) ele vai ficar nesse humor
    // No exemplo abaixo: entre 3000ms (3s) e 7000ms (7s)
    intervaloAtual = random(3000, 7000);
    
    Serial.print("Próxima mudança em: ");
    Serial.print(intervaloAtual / 1000.0);
    Serial.println(" segundos.");
  }
}
