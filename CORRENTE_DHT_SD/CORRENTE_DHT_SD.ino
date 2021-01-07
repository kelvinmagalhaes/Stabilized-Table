//**************************Inclusão de bibliotecas***********************************
// Biblioteca Módulo SD CARD
#include <SD.h>     
// Biblioteca DTH22
#include <dht.h>
//  Biblioteca SENSOR DE CORRENTE
#include "EmonLib.h"

//*************************Declaracao de funcoes  e objetos**************************************
void inicia_SD();
dht DHT; // Cria um objeto da classe dhtdht DHT;
EnergyMonitor SCT013;

//************************Declaracao de variaveis Globais*********************************
int pinSCT = A3;   //Pino analógico conectado ao SCT-013-20
int tensao = 127;  //Tensao  da rede
int potencia;      //Variavel para armazenar a potência obtida

//***********************SETUP***********************************************************
void setup()
{  Serial.begin(9600);
  SCT013.current(pinSCT, 20);//valor calibrado Valor_de_Calibração = 20 / 1 - expiras/saida
  inicia_SD();// Função que inicia o módulo sd.
}

//**********************LOOP************************************************************
void loop() {
//variaveis
  float tempDHT=0,humDHT=0;             // temperatura e umidade
  float Irms = SCT013.calcIrms(1480);   // Calcula o valor da Corrente
  potencia = Irms * tensao;             // Calcula o valor da Potencia Instantanea
// sensor de corrente  
    Serial.print("Corrente = ");
    Serial.print(Irms);
    Serial.println("A");
    Serial.print("Potencia = ");
    Serial.print(potencia);
    Serial.println(" W");
  
//DHT
  DHT.read22(A0); // chama método de leitura da classe dht, com o pino de transmissão de dados ligado no pino A1
  tempDHT = DHT.temperature;
  humDHT = DHT.humidity;
    Serial.print("Temperatura = ");
    Serial.print(tempDHT);
    Serial.println("ºC");
    Serial.print("Umidade Rel.= ");
    Serial.print(humDHT);
    Serial.println("%");
    
    delay(1000);
    Serial.print("--------------------------------------");
    Serial.print("\n");

   //SD
   File dataFile = SD.open("temphum.csv", FILE_WRITE);// abre arquivo para escrita.
   if (dataFile)  {
      dataFile.print(Irms);
      dataFile.print(" A;");
      dataFile.print(potencia);
      dataFile.print(" W;");  
      dataFile.print(tempDHT);
      dataFile.print(" C;");      
      dataFile.print(humDHT);
      dataFile.print(" %;");  
      dataFile.println();
      dataFile.close();      
    }
    else
    {
      Serial.print("Erro na gravacao");
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
