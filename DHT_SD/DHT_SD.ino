//******************************Inclusão de bibliotecas***********************************
// Bibliotecas Módulo SD CARD
#include <SD.h>     
// Bibliotecas MPU6050
#include <Wire.h>
// Bibliotecas DTH22
#include <dht.h>

void inicia_SD();
//***********************************FIM DO BLOCO******************************************
/*Pinos
  * SD card attached to SPI bus as follows:
 ** MOSI- DI  - pin 11
 ** MISO- DO  - pin 12
 ** CLK       - pin 13
 ** CS        - pin 9
  
  *DTH22 
  *data - a0
*/
//*************** Definição de váriáveis Globais e Pinos *********************************

// DTH22
dht DHT; // Cria um objeto da classe dht
//***************************************FIM DO BLOCO************************************

void setup(){
  Serial.begin(9600);
  //Módulo SD CARD
  inicia_SD();// Função que inicia o módulo sd para todos os dispositivos.
 //************************* LOOP ******************************************

void loop() {
  int timer_sd = 0,timer_ds_dht = 0; // variaveis para contagem de tempo
   float tempDHT=0,humDHT=0;
   if(millis() - timer_ds_dht>= 10000){// defini tempo de 2 segundos para realizar a leitura novamente, sem deixar o arduino ocioso.
      
// DHT
     DHT.read22(A0); // chama método de leitura da classe dht, com o pino de transmissão de dados ligado no pino A1
     tempDHT = DHT.temperature;
     humDHT = DHT.humidity;
     timer_ds_dht = millis(); // Atualiza a referência de tempo , conta os segundos e zera novamente .
  }
 
 //if(millis() - timer_sd>= 2000){// defini tempo de 5 segundos para realizar a leitura novamente, sem deixar o arduino ocioso.
    File dataFile = SD.open("temphum.csv", FILE_WRITE);// abre arquivo para escrita.
    if (dataFile)  {
      dataFile.print(tempDHT);
      dataFile.print(";");      
      dataFile.print(humDHT);
      dataFile.print(";");  
      dataFile.println();
      dataFile.close();      
    }
    else
    {
      Serial.print("Erro na gravacao");
    }
    timer_sd = millis();// Atualiza a referência de tempo , conta os segundos e zera novamente .
  }
}


//------------------------------------------ Funções de Setup-------------------------------------------//
void inicia_SD(){
   // se retirar a parte serial desta funcao conseguimos mais 3% de economia de memoria
    while (!Serial) {
    ; // Aguarde pela conexao.
    }
    Serial.print("SD CARD…");
    pinMode(9, OUTPUT); // define o pino 9 como saída, ChipSelected.
     
    if (!SD.begin(9)){
    Serial.println("Falhou");
    return;
    }
    Serial.println("Ok.");
}
