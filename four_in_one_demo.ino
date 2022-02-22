/* Cuatro en Uno Programa Multifuncion:
- Infrarrojo Remoto 
- Bluetooth Remoto 
- Evasion de Obstaculos
- Seguidor de Linea 
*/

#include "IRremote.h"  
#include <Servo.h>
#include <SoftwareSerial.h>


// Definicion de los Modos de Funcionamiento
#define L298
//#define IR
//#define SERVO
#define HC06
#define TRACKING_CAR

#if defined(L298)
// Definicion de los Pines del Motor
//MR
int pinI1=8;    // Definicion de la interfaz I1
int pinI2=9;    // Definicion la interfaz I2
int ena=10;     // Definicion de interfaz EA (regulación de velocidad PWM)
//ML
int pinI3=5;    // Definicion de la interfaz I3
int pinI4=6;    // Definicion de la interfaz I4
int enb=11;     // Definicion de interfaz EB (regulación de velocidad PWM)
#endif

#if defined(IR)
// Ajuste el IRcode detectado
const int irReceiverPin = 2;    //Receptor de infrarrojos conectado al pin 2 seÃ±ales de salida
// IR codes
long IRfront= 0xFF629D;         
long IRback= 0xFFA857;          
long IRturnright=0xFFC23D;    
long IRturnleft= 0xFF22DD;    
long IRstop=0xFF02FD;         
long IRcny70=0x00FFA857;       
long IRAutorun=0x00FF906F;     
long IRturnsmallleft= 0x00FF22DD; 
IRrecv irrecv(irReceiverPin);       // IRrecv objeto definido para recibir señales infrarrojas 
decode_results results;       
#endif

#if defined(SERVO)
Servo myservo;        // myservo
int myservoStart = 0; // flag for start moving
#endif

#if defined(HC06)
int incomingByte = 0; // Para Datos en Serie Entrantes
#endif

#if defined(TRACKING_CAR)
const int SensorLeft = A3;      // Pin de entrada del sensor izquierdo
const int SensorMiddle= A4 ;    // Pin de entrada del sensor medio
const int SensorRight = A5;     // Pin de entrada del sensor derecho
int SL;    // estado del sensor izquierdo
int SM;    // estado del sensor medio
int SR;    // estado del sensor derecho
int trackingCarModeOn = LOW;
#endif

// software serial
const int softwareSerialRx = 4;    // Rx pinout
const int softwareSerialTx = 7;    // Tx pinout
SoftwareSerial mySerial(softwareSerialRx, softwareSerialTx); // RX, TX

// SETUP
void setup()
{  

#if defined(L298)
  pinMode(pinI1,OUTPUT);
  pinMode(pinI2,OUTPUT);
  pinMode(ena,OUTPUT);
  pinMode(pinI3,OUTPUT);
  pinMode(pinI4,OUTPUT);
  pinMode(enb,OUTPUT);

  ML_parada();
  MR_parada();
#endif

#if defined(IR)
  irrecv.enableIRIn();     
#endif

#if defined(SERVO)
  myservo.attach(3);    // Definicion de pin 3 de salida del servomotor (PWM)
#endif

#if defined(HC06)  
  Serial.begin(9600);
  Serial.println("Inicializado");
#endif

  // Inicializacion del pin digital LED_BUILTIN como salida
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Establecer la tasa de datos para el puerto SoftwareSerial
  mySerial.begin(9600);

#if defined(TRACKING_CAR)
  pinMode(SensorLeft, INPUT); // Definicion modo sensor izquierdo
  pinMode(SensorMiddle, INPUT);// Definicion modo sensor medio
  pinMode(SensorRight, INPUT); // Definicion modo sensor derecho
#endif
}

//******************************************************************************(LOOP)
void loop() 
{
// Modo de funcionamiento con servo por bluetooth remoto  
#if defined(SERVO) && defined(HC06)
    if(myservoStart == 1)
    {
        myservo.write(5);   // Seteo del servomotor a la posición 5 grados 
        delay(500);
        myservo.write(90);  // Seteo del servomotor a la posición 90 grados 
        delay(300);
        myservo.write(70);  // Seteo del servomotor a la posición 70 grados 
        delay(600);
        myservo.write(60);  // Seteo del servomotor a la posición 60 grados 
        delay(100);
        myservo.write(10);  // Seteo del servomotor a la posición 10 grados 
        delay(600);
        myservo.write(60);  // Seteo del servomotor a la posición 60 grados 
        delay(100);
    }

    // Enviar datos solo cuando reciba datos:
    if (Serial.available() > 0) {
    // Leer el byte entrante:
    incomingByte = Serial.read();

    if (incomingByte == '1')
    {    
      Serial.println("Encendido");
      incomingByte = 0;
      myservoStart = 1;
    }
    if (incomingByte == '0')
    {    
      Serial.println("Apagado");
      incomingByte = 0;
      myservoStart = 0;
    }
  }
#endif

// Modo de funcionamiento con manejo de motores por control remoto ir    
#if defined(L298) && defined(IR)

  if (irrecv.decode(&results)) 
    {
	  if (results.value == IRfront)
       {
        Serial.print("advance");
        MR_marcha(100);
        ML_marcha(100);
        ML_avance();
        MR_avance();
       }
    if (results.value ==  IRback)
     {
      Serial.print("back");
      MR_marcha(100);
      ML_marcha(100);
      ML_retroceso();
      MR_retroceso();
     }

    if (results.value == IRturnright)
    {
      Serial.print("right");
      ML_parada();
      MR_marcha(70);
      //ML_marcha(100);
      //ML_retroceso();
      MR_retroceso();
    }

     if (results.value == IRturnleft)
     {
        Serial.print("left");
        MR_parada();
        //MR_marcha(100);
        ML_marcha(70);
        ML_retroceso();
        //MR_retroceso();
     }

    if (results.value == IRstop)
   {
     Serial.print("stop");
     ML_parada();
     MR_parada();
    }

   irrecv.resume(); 
}
#endif

  // Modo de funcionamiento con manejo de motores por bluetooth remoto
  #if defined(L298) && defined(HC06)
  // Enviar datos solo cuando reciba datos:
  if (mySerial.available() > 0) {
    // Leer el byte entrante:
    incomingByte = mySerial.read();

    if (incomingByte == 'F') {    
      digitalWrite(LED_BUILTIN, HIGH);   // Enciende el LED (HIGH es el nivel alto de voltaje)

      incomingByte = 0;
      Serial.println("Avance");
      MR_marcha(100);
      ML_marcha(100);
      ML_avance();
      MR_avance();
    }

    if (incomingByte == 'S') {    
     digitalWrite(LED_BUILTIN, LOW);   // Apaga el LED (LOW es el nivel bajo de voltaje)

     incomingByte = 0;
// comentado para evitar spam     
//     Serial.println("Freno"); 
     ML_parada();
     MR_parada();
    }

    if (incomingByte == 'B') {
      Serial.println("Atras");
      MR_marcha(100);
      ML_marcha(100);
      ML_retroceso();
      MR_retroceso();
     }

    if (incomingByte == 'R') {
      Serial.println("Derecha");
      MR_marcha(70);
      ML_marcha(100);
      ML_avance();
      MR_avance();
    }

    if (incomingByte == 'L') {
      Serial.println("Izquierda");
      MR_marcha(100);
      ML_marcha(70);
      ML_avance();
      MR_avance();
     }

    if (incomingByte == 'X') {
      Serial.println("TrackingCarModeOn");
      trackingCarModeOn = HIGH;
    }

    if (incomingByte == 'x') {
      Serial.println("TrackingCarModeOff");
      trackingCarModeOn = LOW;
    }
  }
  #endif

    // Modo de funcionamiento con seguidor de linea
  #if defined(TRACKING_CAR)
  if (trackingCarModeOn) {
    SL = digitalRead(SensorLeft);
    SM = digitalRead(SensorMiddle);
    SR = digitalRead(SensorRight);

    if (SM == HIGH)	// El sensor central está en el área negra.
    {
      if (SL == LOW & SR == HIGH) // Izquierda negra derecha blanca, girar a la izquierda
      {
        Serial.println("Izquierda");
        MR_marcha(100);
        ML_marcha(70);
        ML_avance();
        MR_avance();
      } else if (SR == LOW & SL == HIGH) // Izquierda blanca derecha negra, girar a la derecha
      {
        Serial.println("Derecha");
        MR_marcha(70);
        ML_marcha(100);
        ML_avance();
        MR_avance();
      } else  // Blanco en ambos lados, directo
      {
        Serial.println("Avance");
        MR_marcha(100);
        ML_marcha(100);
        ML_avance();
        MR_avance();
      }
    } else // El sensor central está en el área blanca.
    {
      if (SL == HIGH & SR == LOW) // Izquierda negra derecha blanca, giro rápido a la izquierda
      {
        Serial.println("Izquierda Rapida");
        MR_marcha(100);
        ML_marcha(50);
        ML_avance();
        MR_avance();
      } else if (SR == HIGH & SL == LOW) // Izquierda blanca y derecha negra, giro rápido a la derecha
      {
        Serial.println("Derecha Rapida");
        MR_marcha(70);
        ML_marcha(100);
        ML_avance();
        MR_avance();
      } else // todo blanco, para
      {
// comentado para evitar spam     
//     Serial.println("Freno"); 
        ML_parada();
        MR_parada();
      }
    }
  }
  #endif

}

void MR_marcha(int sp) {
// Introduzca el valor analógico para establecer la velocidad  
   analogWrite(ena,sp);   
}

void MR_parada() {
// Frena el motor DC (derecha)  
   digitalWrite(pinI1,HIGH);  
   digitalWrite(pinI2,HIGH);
}

void MR_retroceso() {
// Gire el motor de CC (derecha) en sentido contrario a las agujas del reloj    
  digitalWrite(pinI2,LOW);
  digitalWrite(pinI1,HIGH);
}

void MR_avance() {
// Haga que el motor de CC (derecha) gire en el sentido de las agujas del reloj  
  digitalWrite(pinI1,LOW);    
  digitalWrite(pinI2,HIGH);
}

void ML_marcha(int sp) {
// Introduzca el valor analógico para establecer la velocidad    
  analogWrite(enb,sp);
}

void ML_parada() {
// Frena el motor DC (izquierda)   
   digitalWrite(pinI3,HIGH);
   digitalWrite(pinI4,HIGH);
}

void ML_retroceso() {
// Gire el motor de CC (izquierda) en sentido contrario a las agujas del reloj  
   digitalWrite(pinI3,LOW);
   digitalWrite(pinI4,HIGH);
}

void ML_avance() {
// Haga que el motor de CC (izquierda) gire en el sentido de las agujas del reloj   
   digitalWrite(pinI4,LOW);
   digitalWrite(pinI3,HIGH);
}