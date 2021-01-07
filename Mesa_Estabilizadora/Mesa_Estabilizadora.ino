//code original of Transductor, modified by Kelvin Magalhaes- UFMT - adapt of http://elxedelectronics.blogspot.mx
//***********************Bibliotecas************************************************
#include <Wire.h> // biblioteca para MPU
#include <Servo.h> //biblioteca para servo morotes

//*********************** Definindo Objetos , Classes e variaveis*******************
Servo servo1;
Servo servo2;

#define MPU 0x68// Endereco I2C do MPU 6050

#define A_R 16384.0 // Constante A_R , servem como ponto de referncia
#define G_R 131.0   // Constante G_R
#define RAD_A_DEG = 57.295779//Conversao e radianos em graus 180/pi

int16_t AcX, AcY, AcZ, GyX, GyY, GyZ;  //MPU 6050 valores de 16 bits
int val1;
int val2;

float Acc[2];
float Gy[2];
float Angle[2];

void setup()
{
	Wire.begin();
	Wire.beginTransmission(MPU); // inicia  MPU
	Wire.write(0x6B);
	Wire.write(0);
	Wire.endTransmission(true);
	servo1.attach(5);// servo 1 (motor inferior),conectado a porta 5 do microcontrolador, responsavel pelo eixo X(Horizontal).
	servo2.attach(6);// servo 1 (motor inferior),conectado a porta 5 do microcontrolador, responsavel pelo eixo Y(Vertical).
	Serial.begin(38400);// definicao do baund rate  para monitor serial
}

void loop()
{
	Wire.beginTransmission(MPU);
	Wire.write(0x3B);               // Registrador 0x3B - acelerometro
	Wire.endTransmission(false);
	Wire.requestFrom(MPU,6,true);   //3 leituras(AcX, AcY, AcZ, GyX, GyY, GyZ)
	AcX=Wire.read()<<8|Wire.read(); 
	AcY=Wire.read()<<8|Wire.read();
	AcZ=Wire.read()<<8|Wire.read();

	//angulos Y, X 
	Acc[1] = atan((AcX/A_R)/sqrt(pow((AcY/A_R),2) + pow((AcZ/A_R),2)))*RAD_TO_DEG;
	Acc[0] = atan((AcY/A_R)/sqrt(pow((AcX/A_R),2) + pow((AcZ/A_R),2)))*RAD_TO_DEG;
	
	Wire.beginTransmission(MPU);
	Wire.write(0x43);             // Registrador 0x4B - giroscopio
	Wire.endTransmission(false);  //3 leituras(GyX, GyY, GyZ)
	Wire.requestFrom(MPU,4,true); 
	GyX=Wire.read()<<8|Wire.read();
	GyY=Wire.read()<<8|Wire.read();
  //GyZ=Wire.read()<<8|Wire.read();
  
	//Calculando os angulos do gisroscopio:
	Gy[0] = GyX/G_R;
	Gy[1] = GyY/G_R;

	//Filtro Complementar, implica em melhor resposta do  sistema
	Angle[0] = 0.98 *(Angle[0]+Gy[0]*0.010) + 0.02*Acc[0];
	Angle[1] = 0.98 *(Angle[1]+Gy[1]*0.010) + 0.02*Acc[1];
	
	int Uangle0 = Angle[0] * 100; // Angle X
	int Uangle1 = Angle[1] * 100; // Angle Y

	//Serial.print("Angle X: "); Serial.println(Uangle0);
	//Serial.print("Angle Y: "); Serial.println(Uangle1); Serial.println("------------");

  // alterar os valores  da funcao map para definir  uma  posicao desejada,  
	float val1 = Uangle0; // val1 corresponde a posicao desejada do motor 1(Eixo Horizontal)
	val1 = map(val1, -4300, 7000, 31, 135); // -3500, 4500, 48, 132
	//Serial.println(val1);
	servo1.write(val1);

	float val2 = Uangle1; // val1 corresponde a posicao desejada do motor 2(Eixo Ventical)
	val2= map(val2, -7500, 10000, 31, 135); //-25, 32, 12, 220
	//Serial.println(val2);
	servo2.write(val2);
	
	delay(5);
	
}


