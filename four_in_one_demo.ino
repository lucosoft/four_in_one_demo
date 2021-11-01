//Cuatro en uno (infrarrojo remoto Bluetooth remoto caza evasion de obstaculos) Programa multifuncion

//******************************
#include "IRremote.h"  
#include <Servo.h>

#define L298
//#define IR
//#define SERVO
#define HC06
//***********************Definicion de los pines del motor*************************

//MR
int pinI1=5;//定义I1接口
int pinI2=6;//定义I2接口
int ena=11;//定义EA(PWM调速)接口
//ML
//int pinI3=0;//定义I3接口
//int pinI4=1;//定义I4接口
int pinI3=8;//定义I3接口
int pinI4=9;//定义I4接口
int enb=10;//定义EB(PWM调速)接口

int counter=0;
const int irReceiverPin = 2; //ç´…å¤–ç·šæŽ¥æ”¶å™¨ OUTPUT è¨Šè™ŸæŽ¥åœ¨ pin 2
                             //Receptor de infrarrojos conectado al pin 2 seÃ±ales de salida
  
char val; 
//***********************è¨­å®šæ‰€å�µæ¸¬åˆ°çš„IRcode*************************
//***********************Ajuste el IRcode detectado*************************
long IRfront= 0xFF629D;        // FF629D
long IRback= 0xFFA857;             //å¾Œé€€
long IRturnright=0xFFC23D;    //å�³è½‰
long IRturnleft= 0xFF22DD;     //å·¦è½‰
long IRstop=0xFF02FD;         //å�œæ­¢
long IRcny70=0x00FFA857;        //CNY70è‡ªèµ°æ¨¡å¼�
long IRAutorun=0x00FF906F;      //è¶…éŸ³æ³¢è‡ªèµ°æ¨¡å¼�
long IRturnsmallleft= 0x00FF22DD; 
//*************************å®šç¾©CNY70è…³ä½�************************************

IRrecv irrecv(irReceiverPin);  // å®šç¾© IRrecv ç‰©ä»¶ä¾†æŽ¥æ”¶ç´…å¤–ç·šè¨Šè™Ÿ
//IRrecv irrecv;  // å®šç¾© IRrecv ç‰©ä»¶ä¾†æŽ¥æ”¶ç´…å¤–ç·šè¨Šè™Ÿ
                               // IRrecv objeto definido para recibir seÃ±ales infrarrojas 
decode_results results;       // è§£ç¢¼çµ�æžœå°‡æ”¾åœ¨ decode_results çµ�æ§‹çš„ result è®Šæ•¸è£�

// servo
Servo myservo;        // 設 myservo
int myservoStart = 0; // flag for start moving

// hc06
int incomingByte = 0; // for incoming serial data

//********************************************************************(SETUP)
void setup()
{  

#if defined(L298)
  pinMode(pinI1,OUTPUT);
  pinMode(pinI2,OUTPUT);
  pinMode(ena,OUTPUT);
  pinMode(pinI3,OUTPUT);
  pinMode(pinI4,OUTPUT);
  pinMode(enb,OUTPUT);
#endif

#if defined(IR)
  irrecv.enableIRIn();     // å•Ÿå‹•ç´…å¤–ç·šè§£ç¢¼
#endif

#if defined(SERVO)
  myservo.attach(3);    // 定義伺服馬達輸出第5腳位(PWM)
#endif

#if defined(HC06)  
  Serial.begin(9600);
  Serial.println("Inicializado");
#endif
}

//******************************************************************************(LOOP)
void loop() 
{
  
#if defined(SERVO) && defined(HC06)
    if(myservoStart == 1)
    {
        myservo.write(5);  //讓伺服馬達回歸 預備位置 準備下一次的測量
        delay(500);
        myservo.write(90);  //讓伺服馬達回歸 預備位置 準備下一次的測量
        delay(300);
        myservo.write(70);  //讓伺服馬達回歸 預備位置 準備下一次的測量
        delay(600);
        myservo.write(60);  //讓伺服馬達回歸 預備位置 準備下一次的測量
        delay(100);
        myservo.write(10);  //讓伺服馬達回歸 預備位置 準備下一次的測量
        delay(600);
        myservo.write(60);  //讓伺服馬達回歸 預備位置 準備下一次的測量
        delay(100);
    }
#endif
    
#if defined(L298) && defined(IR)

  if (irrecv.decode(&results)) 
    {
	  if (results.value == IRfront)//å‰�é€²
       {
        Serial.print("advance");
        MR_marcha(100);
        ML_marcha(100);
        ML_avance();
        MR_avance();
       }
    if (results.value ==  IRback)//å¾Œé€€
     {
      Serial.print("back");
      MR_marcha(100);
      ML_marcha(100);
      ML_retroceso();
      MR_retroceso();
     }

    if (results.value == IRturnright)//å�³è½‰
    {
      Serial.print("right");
      ML_parada();
      MR_marcha(70);
      //ML_marcha(100);
      //ML_retroceso();
      MR_retroceso();
    }

     if (results.value == IRturnleft)//å·¦è½‰
     {
        Serial.print("left");
        MR_parada();
        //MR_marcha(100);
        ML_marcha(70);
        ML_retroceso();
        //MR_retroceso();
     }

    if (results.value == IRstop)//å�œæ­¢
   {
     Serial.print("stop");
     ML_parada();
     MR_parada();
    }

   irrecv.resume(); 
}
#endif

#if defined(HC06) && defined(SERVO)
  // send data only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
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

#if defined(L298) && defined(HC06)

  // send data only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    if (incomingByte == '1')
    {    
      incomingByte = 0;
      Serial.println("advance");
      MR_marcha(100);
      ML_marcha(100);
      ML_avance();
      MR_avance();
    }
    if (incomingByte == '0')
    {    
     incomingByte = 0;
     Serial.println("stop");
     ML_parada();
     MR_parada();
    }
    if (incomingByte == '2')
     {
      Serial.print("back");
      MR_marcha(100);
      ML_marcha(100);
      ML_retroceso();
      MR_retroceso();
     }

    if (incomingByte == '4')
    {
      Serial.print("right");
      ML_parada();
      MR_marcha(70);
      //ML_marcha(100);
      //ML_retroceso();
      MR_retroceso();
    }

    if (incomingByte == '3')
     {
        Serial.print("left");
        MR_parada();
        //MR_marcha(100);
        ML_marcha(70);
        ML_retroceso();
        //MR_retroceso();
     }
 
}
#endif

}

void MR_marcha(int sp) 
{
//  analogWrite(ena,sp);//输入模拟值进行设定速度
   digitalWrite(ena,HIGH);//使直流电机（左）刹车

}

void MR_parada() 
{
   digitalWrite(pinI1,HIGH);//使直流电机（左）刹车
   digitalWrite(pinI2,HIGH);
}

void MR_retroceso()
{
  digitalWrite(pinI1,LOW);//使直流电机（左）顺时针转
  digitalWrite(pinI2,HIGH);
}
void MR_avance()
{
  digitalWrite(pinI2,LOW);//使直流电机（左）顺时针转
  digitalWrite(pinI1,HIGH);
}

void ML_marcha(int sp)
{
//  analogWrite(enb,sp);//输入模拟值进行设定速度
  digitalWrite(enb,HIGH);
}

void ML_parada() 
{
   digitalWrite(pinI3,HIGH);//使直流电机（左）刹车
   digitalWrite(pinI4,HIGH);
}
void ML_retroceso()
{
   digitalWrite(pinI4,LOW);//使直流电机（右）逆时针转
   digitalWrite(pinI3,HIGH);
}

void ML_avance()
{
   digitalWrite(pinI3,LOW);//使直流电机（右）逆时针转
   digitalWrite(pinI4,HIGH);
}
