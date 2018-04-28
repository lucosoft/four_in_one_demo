//å››å’Œä¸€ (å¯»çº¿ é�¿éšœ çº¢å¤–é�¥æŽ§ è“�ç‰™é�¥æŽ§)å¤šåŠŸèƒ½ç¨‹åº�
//Cuatro y uno (infrarrojo remoto Bluetooth remoto caza evasiÃ³n de obstÃ¡culos) Programa multifunciÃ³n

//******************************
//#include <IRremote.h>  
#include "IRremote.h"  
//#include <Servo.h>
#include "Servo.h"

#include "DateTime.h"
#include "DateTimeStrings.h"

//***********************Defines de DateTime*************************

#define TIME_MSG_LEN  11   // time sync to PC is HEADER and unix time_t as ten ascii digits
#define TIME_HEADER  255   // Header tag for serial time sync message


//***********************å®šç¾©é¦¬é�”è…³ä½�*************************
//***********************DefiniciÃ³n de los pines del motor*************************

int MotorRight1=5;
int MotorRight2=6;
int MotorLeft1=10;
int MotorLeft2=11;

int counter=0;
const int irReceiverPin = 2; //ç´…å¤–ç·šæŽ¥æ”¶å™¨ OUTPUT è¨Šè™ŸæŽ¥åœ¨ pin 2
                             //Receptor de infrarrojos conectado al pin 2 seÃ±ales de salida
  
char val; 
//***********************è¨­å®šæ‰€å�µæ¸¬åˆ°çš„IRcode*************************
//***********************Ajuste el IRcode detectado*************************
long IRfront= 0x00FFA25D;        //å‰�é€²ç¢¼
long IRback=0x00FF629D;         //å¾Œé€€
long IRturnright=0x00FFC23D;    //å�³è½‰
long IRturnleft= 0x00FF02FD;     //å·¦è½‰
long IRstop=0x00FFE21D;         //å�œæ­¢
long IRcny70=0x00FFA857;        //CNY70è‡ªèµ°æ¨¡å¼�
long IRAutorun=0x00FF906F;      //è¶…éŸ³æ³¢è‡ªèµ°æ¨¡å¼�
long IRturnsmallleft= 0x00FF22DD; 
//*************************å®šç¾©CNY70è…³ä½�************************************
const int SensorLeft = 5;      //å·¦æ„Ÿæ¸¬å™¨è¼¸å…¥è…³
const int SensorMiddle= 4 ;    //ä¸­æ„Ÿæ¸¬å™¨è¼¸å…¥è…³
const int SensorRight = 3;     //å�³æ„Ÿæ¸¬å™¨è¼¸å…¥è…³
int SL;    //å·¦æ„Ÿæ¸¬å™¨ç‹€æ…‹
int SM;    //ä¸­æ„Ÿæ¸¬å™¨ç‹€æ…‹
int SR;    //å�³æ„Ÿæ¸¬å™¨ç‹€æ…‹
IRrecv irrecv(irReceiverPin);  // å®šç¾© IRrecv ç‰©ä»¶ä¾†æŽ¥æ”¶ç´…å¤–ç·šè¨Šè™Ÿ
//IRrecv irrecv;  // å®šç¾© IRrecv ç‰©ä»¶ä¾†æŽ¥æ”¶ç´…å¤–ç·šè¨Šè™Ÿ
                               // IRrecv objeto definido para recibir seÃ±ales infrarrojas 
decode_results results;       // è§£ç¢¼çµ�æžœå°‡æ”¾åœ¨ decode_results çµ�æ§‹çš„ result è®Šæ•¸è£�
//*************************å®šç¾©è¶…éŸ³æ³¢è…³ä½�******************************
int inputPin =13 ; // å®šç¾©è¶…éŸ³æ³¢ä¿¡è™ŸæŽ¥æ”¶è…³ä½�rx
int outputPin =12; // å®šç¾©è¶…éŸ³æ³¢ä¿¡è™Ÿç™¼å°„è…³ä½�'tx
int Fspeedd = 0; // å‰�æ–¹è·�é›¢
int Rspeedd = 0; // å�³æ–¹è·�é›¢
int Lspeedd = 0; // å·¦æ–¹è·�é›¢
int directionn = 0; // å‰�=8 å¾Œ=2 å·¦=4 å�³=6 
Servo myservo; // è¨­ myservo
int delay_time = 250; // ä¼ºæœ�é¦¬é�”è½‰å�‘å¾Œçš„ç©©å®šæ™‚é–“
int Fgo = 8; // å‰�é€²
int Rgo = 6; // å�³è½‰
int Lgo = 4; // å·¦è½‰
int Bgo = 2; // å€’è»Š
//********************************************************************(SETUP)
void setup()
{  
  Serial.begin(9600);
  /*
  pinMode(MotorRight1, OUTPUT);  // è…³ä½� 8 (PWM)
  pinMode(MotorRight2, OUTPUT);  // è…³ä½� 9 (PWM)
  pinMode(MotorLeft1,  OUTPUT);  // è…³ä½� 10 (PWM) 
  pinMode(MotorLeft2,  OUTPUT);  // è…³ä½� 11 (PWM)
  */
  irrecv.enableIRIn();     // å•Ÿå‹•ç´…å¤–ç·šè§£ç¢¼
  /*
  pinMode(SensorLeft, INPUT); //å®šç¾©å·¦æ„Ÿæ¸¬å™¨
  pinMode(SensorMiddle, INPUT);//å®šç¾©ä¸­æ„Ÿæ¸¬å™¨
  pinMode(SensorRight, INPUT); //å®šç¾©å�³æ„Ÿæ¸¬å™¨
  */
  digitalWrite(2,HIGH);
  /*
  pinMode(inputPin, INPUT); // å®šç¾©è¶…éŸ³æ³¢è¼¸å…¥è…³ä½�
  pinMode(outputPin, OUTPUT); // å®šç¾©è¶…éŸ³æ³¢è¼¸å‡ºè…³ä½� 
  */
  myservo.attach(1); // Servo pin de salida del motor SecciÃ³n 5 se define (PWM)

  Serial.print("INFO LOGGER: "); //INFO LOGGER
  Serial.println("Inicializado");
 }
//******************************************************************(Void)
void advance(int a) // å‰�é€²
{
        digitalWrite(MotorRight1,LOW);
        digitalWrite(MotorRight2,HIGH);
        digitalWrite(MotorLeft1,LOW);
        digitalWrite(MotorLeft2,HIGH);
        delay(a * 100); 
}
void right(int b) //å�³è½‰(å–®è¼ª)
{
       digitalWrite(MotorLeft1,LOW);
       digitalWrite(MotorLeft2,HIGH);
       digitalWrite(MotorRight1,LOW);
       digitalWrite(MotorRight2,LOW);
       delay(b * 100);
}
void left(int c) //å·¦è½‰(å–®è¼ª)
{
      digitalWrite(MotorRight1,LOW);
      digitalWrite(MotorRight2,HIGH);
      digitalWrite(MotorLeft1,LOW);
      digitalWrite(MotorLeft2,LOW);
      delay(c * 100);
}
void turnR(int d) //å�³è½‰(é›™è¼ª)
{
      digitalWrite(MotorRight1,HIGH);
      digitalWrite(MotorRight2,LOW);
      digitalWrite(MotorLeft1,LOW);
      digitalWrite(MotorLeft2,HIGH);
      delay(d * 100);
}
void turnL(int e) //å·¦è½‰(é›™è¼ª)
{
      digitalWrite(MotorRight1,LOW);
      digitalWrite(MotorRight2,HIGH);
      digitalWrite(MotorLeft1,HIGH);
      digitalWrite(MotorLeft2,LOW);
      delay(e * 100);
} 
void stopp(int f) //å�œæ­¢
{
     digitalWrite(MotorRight1,LOW);
     digitalWrite(MotorRight2,LOW);
     digitalWrite(MotorLeft1,LOW);
     digitalWrite(MotorLeft2,LOW);
     delay(f * 100);
}
void back(int g) //å¾Œé€€
{
        digitalWrite(MotorRight1,HIGH);
        digitalWrite(MotorRight2,LOW);
        digitalWrite(MotorLeft1,HIGH);
        digitalWrite(MotorLeft2,LOW);;
        delay(g * 100); 
}
void detection() //æ¸¬é‡�3å€‹è§’åº¦(å‰�.å·¦.å�³)
{ 
    int delay_time = 250; // ä¼ºæœ�é¦¬é�”è½‰å�‘å¾Œçš„ç©©å®šæ™‚é–“
    ask_pin_F(); // è®€å�–å‰�æ–¹è·�é›¢

    if(Fspeedd < 10) // å�‡å¦‚å‰�æ–¹è·�é›¢å°�æ–¼10å…¬åˆ†
   {
      stopp(1); // æ¸…é™¤è¼¸å‡ºè³‡æ–™ 
      back(2); // å¾Œé€€ 0.2ç§’
   }
    if(Fspeedd < 25) // å�‡å¦‚å‰�æ–¹è·�é›¢å°�æ–¼25å…¬åˆ†
   {
      stopp(1); // æ¸…é™¤è¼¸å‡ºè³‡æ–™ 
      ask_pin_L(); // è®€å�–å·¦æ–¹è·�é›¢
      delay(delay_time); // ç­‰å¾…ä¼ºæœ�é¦¬é�”ç©©å®š
      ask_pin_R(); // è®€å�–å�³æ–¹è·�é›¢ 
      delay(delay_time); // ç­‰å¾…ä¼ºæœ�é¦¬é�”ç©©å®š 

      if(Lspeedd > Rspeedd) //å�‡å¦‚ å·¦é‚Šè·�é›¢å¤§æ–¼å�³é‚Šè·�é›¢
     {
        directionn = Lgo; //å�‘å·¦èµ°
     }

      if(Lspeedd <= Rspeedd) //å�‡å¦‚ å·¦é‚Šè·�é›¢å°�æ–¼æˆ–ç­‰æ–¼å�³é‚Šè·�é›¢
      {
        directionn = Rgo; //å�‘å�³èµ°
      } 

      if (Lspeedd < 15 && Rspeedd < 15) //å�‡å¦‚ å·¦é‚Šè·�é›¢å’Œå�³é‚Šè·�é›¢çš†å°�æ–¼10å…¬åˆ†
     {
        directionn = Bgo; //å�‘å¾Œèµ° 
      } 
    }
    else //åŠ å¦‚å‰�æ–¹å¤§æ–¼25å…¬åˆ† 
   {
      directionn = Fgo; //å�‘å‰�èµ° 
   }
}   
//*********************************************************************************
void ask_pin_F() // é‡�å‡ºå‰�æ–¹è·�é›¢ 
{
myservo.write(90);
digitalWrite(outputPin, LOW); // è®“è¶…è�²æ³¢ç™¼å°„ä½Žé›»å£“2Î¼s
delayMicroseconds(2);
digitalWrite(outputPin, HIGH); // è®“è¶…è�²æ³¢ç™¼å°„é«˜é›»å£“10Î¼sï¼Œé€™è£¡è‡³å°‘æ˜¯10Î¼s
delayMicroseconds(10);
digitalWrite(outputPin, LOW); // ç¶­æŒ�è¶…è�²æ³¢ç™¼å°„ä½Žé›»å£“
float Fdistance = pulseIn(inputPin, HIGH); // è®€å·®ç›¸å·®æ™‚é–“
Fdistance= Fdistance/5.8/10; // å°‡æ™‚é–“è½‰ç‚ºè·�é›¢è·�ç¦»ï¼ˆå–®ä½�ï¼šå…¬åˆ†ï¼‰
Serial.print("F distance:"); //è¼¸å‡ºè·�é›¢ï¼ˆå–®ä½�ï¼šå…¬åˆ†ï¼‰
Serial.println(Fdistance); //é¡¯ç¤ºè·�é›¢
Fspeedd = Fdistance; // å°‡è·�é›¢ è®€å…¥Fspeedd(å‰�é€Ÿ)
} 
//********************************************************************************
void ask_pin_L() // é‡�å‡ºå·¦é‚Šè·�é›¢ 
{
myservo.write(177);
delay(delay_time);
digitalWrite(outputPin, LOW); // è®“è¶…è�²æ³¢ç™¼å°„ä½Žé›»å£“2Î¼s
delayMicroseconds(2);
digitalWrite(outputPin, HIGH); // è®“è¶…è�²æ³¢ç™¼å°„é«˜é›»å£“10Î¼sï¼Œé€™è£¡è‡³å°‘æ˜¯10Î¼s
delayMicroseconds(10);
digitalWrite(outputPin, LOW); // ç¶­æŒ�è¶…è�²æ³¢ç™¼å°„ä½Žé›»å£“
float Ldistance = pulseIn(inputPin, HIGH); // è®€å·®ç›¸å·®æ™‚é–“
Ldistance= Ldistance/5.8/10; // å°‡æ™‚é–“è½‰ç‚ºè·�é›¢è·�ç¦»ï¼ˆå–®ä½�ï¼šå…¬åˆ†ï¼‰
Serial.print("L distance:"); //è¼¸å‡ºè·�é›¢ï¼ˆå–®ä½�ï¼šå…¬åˆ†ï¼‰
Serial.println(Ldistance); //é¡¯ç¤ºè·�é›¢
Lspeedd = Ldistance; // å°‡è·�é›¢ è®€å…¥Lspeedd(å·¦é€Ÿ)
} 
//******************************************************************************
void ask_pin_R() // é‡�å‡ºå�³é‚Šè·�é›¢ 
{
myservo.write(5);
delay(delay_time);
digitalWrite(outputPin, LOW); // è®“è¶…è�²æ³¢ç™¼å°„ä½Žé›»å£“2Î¼s
delayMicroseconds(2);
digitalWrite(outputPin, HIGH); // è®“è¶…è�²æ³¢ç™¼å°„é«˜é›»å£“10Î¼sï¼Œé€™è£¡è‡³å°‘æ˜¯10Î¼s
delayMicroseconds(10);
digitalWrite(outputPin, LOW); // ç¶­æŒ�è¶…è�²æ³¢ç™¼å°„ä½Žé›»å£“
float Rdistance = pulseIn(inputPin, HIGH); // è®€å·®ç›¸å·®æ™‚é–“
Rdistance= Rdistance/5.8/10; // å°‡æ™‚é–“è½‰ç‚ºè·�é›¢è·�ç¦»ï¼ˆå–®ä½�ï¼šå…¬åˆ†ï¼‰
Serial.print("R distance:"); //è¼¸å‡ºè·�é›¢ï¼ˆå–®ä½�ï¼šå…¬åˆ†ï¼‰
Serial.println(Rdistance); //é¡¯ç¤ºè·�é›¢
Rspeedd = Rdistance; // å°‡è·�é›¢ è®€å…¥Rspeedd(å�³é€Ÿ)
} 
//******************************************************************************(LOOP)
void loop() 
{
      SL = digitalRead(SensorLeft);
      SM = digitalRead(SensorMiddle);
      SR = digitalRead(SensorRight);
      performCommand();
//***************************************************************************æ­£å¸¸é�™æŽ§æ¨¡å¼�      
  if (irrecv.decode(&results)) 
    {         // è§£ç¢¼æˆ�åŠŸï¼Œæ”¶åˆ°ä¸€çµ„ç´…å¤–ç·šè¨Šè™Ÿ
/***********************************************************************/
	  Serial.print("INFO LOGGER:"); //INFO LOGGER
	  Serial.println(results.value);
	  if (results.value == IRfront)//å‰�é€²
       {
        advance(10);//å‰�é€²
       }
/***********************************************************************/ 
      if (results.value ==  IRback)//å¾Œé€€
       {
        back(10);//å¾Œé€€
       }
/***********************************************************************/
      if (results.value == IRturnright)//å�³è½‰
      {
        right(6); // å�³è½‰
      }
/***********************************************************************/
     if (results.value == IRturnleft)//å·¦è½‰
     {
       left(6); // å·¦è½‰);
     }
/***********************************************************************/    
    if (results.value == IRstop)//å�œæ­¢
   {
     digitalWrite(MotorRight1,LOW);
     digitalWrite(MotorRight2,LOW);
     digitalWrite(MotorLeft1,LOW);
     digitalWrite(MotorLeft2,LOW);
    }
//***********************************************************************cny70æ¨¡å¼�è‡ªèµ°æ¨¡å¼� é»‘:LOW ç™½:
    if (results.value == IRcny70)
   {                     
     while(IRcny70)
     {  
       SL = digitalRead(SensorLeft);
       SM = digitalRead(SensorMiddle);
       SR = digitalRead(SensorRight);
                   
       if (SM == HIGH)//ä¸­æ„Ÿæ¸¬å™¨åœ¨é»‘è‰²å�€åŸŸ
       { 
          if (SL == LOW & SR == HIGH) // å·¦é»‘å�³ç™½, å�‘å·¦è½‰å½Ž
          {  
             digitalWrite(MotorRight1,LOW);
             digitalWrite(MotorRight2,HIGH);
             analogWrite(MotorLeft1,0);
             analogWrite(MotorLeft2,80);
          } 
          else if (SR == LOW & SL == HIGH) //å·¦ç™½å�³é»‘, å�‘å�³è½‰å½Ž
          {  
             analogWrite(MotorRight1,0);//å�³è½‰
             analogWrite(MotorRight2,80);
             digitalWrite(MotorLeft1,LOW);
             digitalWrite(MotorLeft2,HIGH);
          }
         else  // å…©å�´å�‡ç‚ºç™½è‰², ç›´é€²
          { 
             digitalWrite(MotorRight1,LOW);
             digitalWrite(MotorRight2,HIGH);
             digitalWrite(MotorLeft1,LOW);
             digitalWrite(MotorLeft2,HIGH);
             analogWrite(MotorLeft1,200);
             analogWrite(MotorLeft2,200);
             analogWrite(MotorRight1,200);
             analogWrite(MotorRight2,200);
         }      
       } 
       else // ä¸­æ„Ÿæ¸¬å™¨åœ¨ç™½è‰²å�€åŸŸ
      {  
         if (SL == LOW & SR == HIGH)// å·¦é»‘å�³ç™½, å¿«é€Ÿå·¦è½‰ 
        {  
            digitalWrite(MotorRight1,LOW);
            digitalWrite(MotorRight2,HIGH);
            digitalWrite(MotorLeft1,LOW);
            digitalWrite(MotorLeft2,LOW);
        }
         else if (SR == LOW & SL == HIGH) // å·¦ç™½å�³é»‘, å¿«é€Ÿå�³è½‰
        {  
           digitalWrite(MotorRight1,LOW);
           digitalWrite(MotorRight2,LOW);
           digitalWrite(MotorLeft1,LOW);
           digitalWrite(MotorLeft2,HIGH);
        }
         else // éƒ½æ˜¯ç™½è‰², å�œæ­¢
        {    
        digitalWrite(MotorRight1,HIGH);
        digitalWrite(MotorRight2,LOW);
        digitalWrite(MotorLeft1,HIGH);
        digitalWrite(MotorLeft2,LOW);;
        }
      }
       if (irrecv.decode(&results))
       {
             irrecv.resume(); 
                  Serial.println(results.value,HEX);
             if(results.value ==IRstop)
             { 
               digitalWrite(MotorRight1,HIGH);
               digitalWrite(MotorRight2,HIGH);
               digitalWrite(MotorLeft1,HIGH);
               digitalWrite(MotorLeft2,HIGH);
               break;
             }
       }
     }
      results.value=0;
   }
//***********************************************************************è¶…éŸ³æ³¢è‡ªèµ°æ¨¡å¼�
 if (results.value ==IRAutorun )
      {
           while(IRAutorun)
        {
            myservo.write(90); //è®“ä¼ºæœ�é¦¬é�”å›žæ­¸ é �å‚™ä½�ç½® æº–å‚™ä¸‹ä¸€æ¬¡çš„æ¸¬é‡�
            detection(); //æ¸¬é‡�è§’åº¦ ä¸¦ä¸”åˆ¤æ–·è¦�å¾€å“ªä¸€æ–¹å�‘ç§»å‹•
             if(directionn == 8) //å�‡å¦‚directionn(æ–¹å�‘) = 8(å‰�é€²) 
            { 
              if (irrecv.decode(&results))
           {
             irrecv.resume(); 
             Serial.println(results.value,HEX);
             if(results.value ==IRstop)
             { 
               digitalWrite(MotorRight1,LOW); 
               digitalWrite(MotorRight2,LOW);
               digitalWrite(MotorLeft1,LOW);
               digitalWrite(MotorLeft2,LOW);
               break;
             }
           }
                results.value=0;
                advance(1); // æ­£å¸¸å‰�é€² 
                Serial.print(" Advance "); //é¡¯ç¤ºæ–¹å�‘(å‰�é€²)
                Serial.print(" "); 
            }
           if(directionn == 2) //å�‡å¦‚directionn(æ–¹å�‘) = 2(å€’è»Š) 
          {
            if (irrecv.decode(&results))
           {
             irrecv.resume(); 
             Serial.println(results.value,HEX);
             if(results.value ==IRstop)
             { 
               digitalWrite(MotorRight1,LOW); 
               digitalWrite(MotorRight2,LOW);
               digitalWrite(MotorLeft1,LOW);
               digitalWrite(MotorLeft2,LOW);
               break;
             }
           }
              results.value=0;
              back(8); // å€’é€€(è»Š)
              turnL(3); //äº›å¾®å�‘å·¦æ–¹ç§»å‹•(é˜²æ­¢å�¡åœ¨æ­»å··è£¡)
              Serial.print(" Reverse "); //é¡¯ç¤ºæ–¹å�‘(å€’é€€)
          }
            if(directionn == 6) //å�‡å¦‚directionn(æ–¹å�‘) = 6(å�³è½‰) 
          {
           if (irrecv.decode(&results))
           {
              irrecv.resume(); 
              Serial.println(results.value,HEX);
             if(results.value ==IRstop)
             { 
               digitalWrite(MotorRight1,LOW); 
               digitalWrite(MotorRight2,LOW);
               digitalWrite(MotorLeft1,LOW);
               digitalWrite(MotorLeft2,LOW);
               break;
             }
           }
             results.value=0;
               back(1); 
               turnR(6); // å�³è½‰
               Serial.print(" Right "); //é¡¯ç¤ºæ–¹å�‘(å·¦è½‰)
          }
            if(directionn == 4) //å�‡å¦‚directionn(æ–¹å�‘) = 4(å·¦è½‰) 
          { 
             if (irrecv.decode(&results))
           {
             irrecv.resume(); 
             Serial.println(results.value,HEX);
             if(results.value ==IRstop)
             { 
               digitalWrite(MotorRight1,LOW); 
               digitalWrite(MotorRight2,LOW);
               digitalWrite(MotorLeft1,LOW);
               digitalWrite(MotorLeft2,LOW);
               break;
             }
           }
                results.value=0;
                back(1); 
                turnL(6); // å·¦è½‰
                Serial.print(" Left "); //é¡¯ç¤ºæ–¹å�‘(å�³è½‰) 
           } 
            
             if (irrecv.decode(&results))
           {
             irrecv.resume(); 
             Serial.println(results.value,HEX);
             if(results.value ==IRstop)
             { 
               digitalWrite(MotorRight1,LOW); 
               digitalWrite(MotorRight2,LOW);
               digitalWrite(MotorLeft1,LOW);
               digitalWrite(MotorLeft2,LOW);
               break;
             }
           }
         }
               results.value=0;
       }
/***********************************************************************/    
     else
    {
           digitalWrite(MotorRight1,LOW);
           digitalWrite(MotorRight2,LOW);
           digitalWrite(MotorLeft1,LOW);
           digitalWrite(MotorLeft2,LOW);
     }
      

        irrecv.resume();    // ç¹¼çºŒæ”¶ä¸‹ä¸€çµ„ç´…å¤–ç·šè¨Šè™Ÿ        
   }  
}
   
void performCommand() {
  if (Serial.available()) {
    val = Serial.read();
  }
    if (val == 'f') { // Forward
      advance(10);
    } else if (val == 'z') { // Stop Forward     
      stopp(10) ;
    } else if (val == 'b') { // Backward
      back(10); 
    } else if (val == 'y') { // Stop Backward
       back(10); 
    }  else if (val == 'l') { // Right
      turnR(10);
    } else if (val == 'r') { // Left
      turnL(10);
    } else if (val == 'v') { // Stop Turn
      stopp(10) ;
    } else if (val == 's') { // Stop
      stopp(10) ;
    } 
  
}




