//Only for Motomatic
//Do Not Copy
//Program includes Functions , MOTOR ON , MOTOR OFF, MOTOR STATE, SUPPLY STATE, MOTOR OFF SWITCH PHYSICAL etc

#include <SoftwareSerial.h>

SoftwareSerial SIM900A(9,10);



String textMessage;
String motorState = "OFF";
String supplystate = "FAULTY";
String voltagestate = "LOW";
int oflinemodeState = 0;
int onlinemodeState = 0;
int motoroffbuttonState = 0;



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Variables For Loop////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int a = 0;  // For while loop in motor on message
int b = 0;  // For use in while loop to stop motor in energency
int c = 0;  // For use in while loop in oflinemode
int d = 0;  // For emergency motor off
int e = 0;  // Currently Not in Use
int f = 0;  // Currently Not in Use


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




//Arduino Pin Connections

int VoltR = A0;
int VoltY = A1;
int VoltB = A2;
const int motoroff = 2; //Generally HIGH, If pushed becomes LOW.
const int motor = 3; //Generally LOW, To turn motor ON, Make it HIGH
const int oflinemode = 4;  //Generally HIGH , if pushed , it gets LOW.
const int onlinemode = 5;  //Generally HIGH , If pushed , it gets LOW.
const int motoronled = 6;  //Correct-Green Light
const int motoroffled = 7; //Correct - Red Light
const int linerled = A3;   //Generally LOW
const int lineyled = A4;   //Generally LOW
const int linebled = A5;   //Generally LOW
const int oflinemodeled =  11;
const int onlinemodeled =  12;





void setup() {


  SIM900A.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  Serial.println ("SIM900A Ready");
  pinMode(oflinemodeled, OUTPUT);
  pinMode(onlinemodeled, OUTPUT);
  pinMode(motoronled, OUTPUT);
  pinMode(motoroffled,OUTPUT);
  pinMode(onlinemode, INPUT);
  pinMode(oflinemode, INPUT);
  pinMode(linerled, OUTPUT);
  pinMode(lineyled, OUTPUT);
  pinMode(linebled, OUTPUT);
  pinMode(VoltR, INPUT);
  pinMode(VoltY, INPUT);
  pinMode(VoltB, INPUT);
  pinMode(motor, OUTPUT);
  pinMode(motoroff, INPUT);

  digitalWrite(oflinemodeled, HIGH);
  // digitalWrite(onlinemodeled, HIGH);
  digitalWrite(motoroffled, HIGH);
  // digitalWrite(motoronled, HIGH);
  digitalWrite(linerled, HIGH);
  digitalWrite(lineyled, HIGH);
  digitalWrite(linebled, HIGH);
  digitalWrite(motor, LOW);
  delay(3000);
  digitalWrite(oflinemodeled, LOW);
  digitalWrite(onlinemodeled, LOW);
  digitalWrite(motoroffled, HIGH);
  digitalWrite(motoronled, LOW);
  digitalWrite(linerled, LOW);
  digitalWrite(lineyled, LOW);
  digitalWrite(linebled, LOW);
  digitalWrite(motor, LOW);
  delay(2000);
  digitalWrite(oflinemodeled, HIGH);
  // digitalWrite(onlinemodeled, HIGH);
  digitalWrite(motoroffled, HIGH);
   // digitalWrite(motoronled, HIGH);
  digitalWrite(linerled, HIGH);
  digitalWrite(lineyled, HIGH);
  digitalWrite(linebled, HIGH);
  digitalWrite(motor, LOW);
  delay(3000);
  digitalWrite(oflinemodeled, LOW);
   // digitalWrite(onlinemodeled, LOW);
  digitalWrite(motoroffled, HIGH);
   // digitalWrite(motoronled, LOW);
  digitalWrite(linerled, LOW);
  digitalWrite(lineyled, LOW);
  digitalWrite(linebled, LOW);
  digitalWrite(motor, LOW);
  delay(2000);
  digitalWrite(oflinemodeled, HIGH);
    //digitalWrite(onlinemodeled, HIGH);
  digitalWrite(motoroffled, HIGH);
   // digitalWrite(motoronled, HIGH);
  digitalWrite(linerled, HIGH);
  digitalWrite(lineyled, HIGH);
  digitalWrite(linebled, HIGH);
  digitalWrite(motor, LOW);
  delay(6000);
  digitalWrite(oflinemodeled, LOW);
  digitalWrite(motoroffled, HIGH);
  digitalWrite(linerled, LOW);
  digitalWrite(lineyled, LOW);
  digitalWrite(linebled, LOW);
  digitalWrite(motor, LOW);
  
  // Initializing serial commmunication
  SIM900A.begin(9600);
  blinkLED();
  // Give time to your GSM shield log on to network
  delay(80000);


  // AT command to set SIM900 to SMS mode
  SIM900A.print("AT+CMGF=1\r"); 
  delay(100);
  // Set module to send SMS data to serial out upon receipt 
  SIM900A.print("AT+CNMI=2,2,0,0,0\r");
  delay(100);

  Serial.println("GSM IS READY");
}



 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////Void Setup Ends Here//////

void loop(){
  motoroffbuttonState = digitalRead(motoroff);
  if(motoroffbuttonState == LOW){
    d = 0; 
    while(d == 0){
      digitalWrite(motor, LOW);
      digitalWrite(motoroffled, HIGH);
      digitalWrite(motoronled, LOW);
      motorState = "OFF"; 
      textMessage = "";
      motoroffbuttonState = digitalRead(motoroff);
      if(motoroffbuttonState == HIGH){
        d=1;
      }
        
      int AcVoltR = analogRead(VoltR);
      int AcVoltY = analogRead(VoltY);
      int AcVoltB = analogRead(VoltB);
      int AcVoltROut = (AcVoltR * (5.0 / 1023))*212.25;
      int AcVoltYOut = (AcVoltY * (5.0 / 1023))*53.68;
      int AcVoltBOut = (AcVoltB * (5.0 / 1023))*203.41;

      // Serial.print("R= ");
      // Serial.println(AcVoltROut);
      // Serial.print("Y= ");
      // Serial.println(AcVoltYOut);
      // Serial.print("B= ");
      // Serial.println(AcVoltBOut);
      // Serial.println("==============================================");

      if(AcVoltROut>125){digitalWrite(linerled, HIGH);}else{digitalWrite(linerled, LOW);}
      if(AcVoltYOut>125){digitalWrite(lineyled, HIGH);}else{digitalWrite(lineyled, LOW);}
      if(AcVoltBOut>125){digitalWrite(linebled, HIGH);}else{digitalWrite(linebled, LOW);}

      if(AcVoltROut<125||AcVoltYOut<125||AcVoltBOut<125){
        digitalWrite(motor, LOW);
        digitalWrite(motoroffled, HIGH);
        digitalWrite(motoronled, LOW);
        motorState = "OFF"; 
        textMessage = ""; 
        supplystate = "Supply is Faulty";
      }

      if(AcVoltROut>125&&AcVoltYOut>125&&AcVoltBOut>125){supplystate = "Supply is OK";}else{supplystate = "Supply is Faulty";}
      if(SIM900A.available()>0){textMessage = SIM900A.readString();textMessage.toUpperCase();delay(10);}
      if(textMessage.indexOf("MOTORSTATE")>=0){
        String message = supplystate + ", Motor is currently " + motorState;
        sendSMS(message);textMessage = "";
        delMessage();
      }
      if(textMessage.indexOf("SUPPLYSTATE")>=0){
        String message = supplystate;
        sendSMS(message);
        textMessage = "";
        delMessage();
      }
      blinkLED();
    }
      
  }
  if(SIM900A.available()>0){
    textMessage = SIM900A.readString();
    textMessage.toUpperCase();
    delay(10);}

//Voltage Checking Starts Here
  int AcVoltR = analogRead(VoltR);
  int AcVoltY = analogRead(VoltY);
  int AcVoltB = analogRead(VoltB);
      int AcVoltROut = (AcVoltR * (5.0 / 1023))*212.25;
      int AcVoltYOut = (AcVoltY * (5.0 / 1023))*53.68;
      int AcVoltBOut = (AcVoltB * (5.0 / 1023))*203.41;

  if(AcVoltROut>125){digitalWrite(linerled, HIGH);}else{digitalWrite(linerled, LOW);}
  if(AcVoltYOut>125){digitalWrite(lineyled, HIGH);}else{digitalWrite(lineyled, LOW);}
  if(AcVoltBOut>125){digitalWrite(linebled, HIGH);}else{digitalWrite(linebled, LOW);}
  
  if(AcVoltROut<125||AcVoltYOut<125||AcVoltBOut<125){
    digitalWrite(motor, LOW);
    digitalWrite(motoroffled, HIGH);
    digitalWrite(motoronled, LOW);
    motorState = "OFF"; 
    textMessage = ""; 
    supplystate = "Supply is Faulty";
  }
  
  if(AcVoltROut>125&&AcVoltYOut>125&&AcVoltBOut>125){supplystate = "Supply is OK";}else{supplystate = "Supply is Faulty";}

//Voltage Checking Ends here
//Ofline Mode Starts here      
  oflinemodeState = digitalRead(oflinemode);
  if (oflinemodeState == LOW){
    digitalWrite(oflinemodeled, HIGH);
    digitalWrite(onlinemodeled, LOW);
    c = 0;
    while(c == 0){
      motoroffbuttonState = digitalRead(motoroff);
      if(motoroffbuttonState == LOW){
        c = 1;
        digitalWrite(motor, LOW);
        digitalWrite(motoroffled, HIGH);
        digitalWrite(motoronled, LOW);
        motorState = "OFF"; 
        textMessage = "";
      }
      oflinemodeState = digitalRead(oflinemode);
      if(oflinemodeState == HIGH){
        c = 1;
        digitalWrite(oflinemodeled, LOW);
      }

      if(SIM900A.available()>0){textMessage = SIM900A.readString();textMessage.toUpperCase();delay(10);}
      if(textMessage.indexOf("MOTORSTATE")>=0){String message = supplystate + ", Motor is currently " + motorState;sendSMS(message);textMessage = "";}
      if(textMessage.indexOf("SUPPLYSTATE")>=0){String message = supplystate;sendSMS(message);textMessage = "";}

      int AcVoltR = analogRead(VoltR);
      int AcVoltY = analogRead(VoltY);
      int AcVoltB = analogRead(VoltB);
      int AcVoltROut = (AcVoltR * (5.0 / 1023))*212.25;
      int AcVoltYOut = (AcVoltY * (5.0 / 1023))*53.68;
      int AcVoltBOut = (AcVoltB * (5.0 / 1023))*203.41;
      if(AcVoltROut>125){digitalWrite(linerled, HIGH);}else{digitalWrite(linerled, LOW);}
      if(AcVoltYOut>125){digitalWrite(lineyled, HIGH);}else{digitalWrite(lineyled, LOW);}
      if(AcVoltBOut>125){digitalWrite(linebled, HIGH);}else{digitalWrite(linebled, LOW);}
      
      if(AcVoltROut<125||AcVoltYOut<125||AcVoltBOut<125){
        digitalWrite(motor, LOW);
        digitalWrite(motoroffled, HIGH);
        digitalWrite(motoronled, LOW);
        motorState = "OFF"; 
        textMessage = ""; 
        supplystate = "Supply is Faulty";
      }
      if(AcVoltROut>125&&AcVoltYOut>125&&AcVoltBOut>125){supplystate = "Supply is OK";}else{supplystate = "Supply is Faulty";}
      
      if(AcVoltROut>125&&AcVoltYOut>125&&AcVoltBOut>125){
        if(SIM900A.available()>0){textMessage = SIM900A.readString();textMessage.toUpperCase();delay(10);}
        if(textMessage.indexOf("MOTORSTATE")>=0){String message = supplystate + ", Motor is currently " + motorState;sendSMS(message);textMessage = "";}
        if(textMessage.indexOf("SUPPLYSTATE")>=0){String message = supplystate;sendSMS(message);textMessage = "";}
        digitalWrite(motoroffled, LOW);
        digitalWrite(motoronled, HIGH);
        digitalWrite(motor, HIGH);
        motorState = "ON";
        supplystate = "Supply is OK, ";
        textMessage = "";
        
        oflinemodeState = digitalRead(oflinemode);
        if (oflinemodeState == HIGH){
          c = 1;
          digitalWrite(oflinemodeled, LOW);
        }
      }
    }
  }else{
    digitalWrite(oflinemodeled, LOW);
  }

//Oflinemode Ends Here//
//OnlineMode Starts Here//

  onlinemodeState = digitalRead(onlinemode);
  if(onlinemodeState == LOW){digitalWrite(onlinemodeled, HIGH);}else{digitalWrite(onlinemodeled, LOW);}

///OnlineMode Ends Here//
///Messages Action Starts Here//



  if(SIM900A.available()>0){textMessage = SIM900A.readString();textMessage.toUpperCase();delay(10);}
  if(textMessage.indexOf("MOTORON")>=0){
    String message = "Motor is set to turn ON, Earlier it was " + motorState +".  "+ supplystate + ". "+ "R= "+AcVoltROut + "V Y= "+AcVoltYOut+"V B= "+AcVoltBOut+"V." ;
    sendSMS(message);
    textMessage = "";
    a = 0;
    delMessage();
    while(a == 0){
      motoroffbuttonState = digitalRead(motoroff);
      if(motoroffbuttonState == LOW){
        Serial.println("MotorOFF signal received from EMERGENCY SWITCH, MOTOR TURNED OFF");
        a = 1;
        digitalWrite(motor, LOW);
        digitalWrite(motoroffled, HIGH);
        digitalWrite(motoronled, LOW);
        motorState = "OFF";
        String message = "MotorOFF signal received from EMERGENCY SWITCH, MOTOR TURNED OFF. Earlier it was ON ";
        sendSMS(message);
        textMessage = "";
        blinkLED();
        break;
      }

      if(textMessage.indexOf("MOTOROFF")>=0){
        Serial.println("Command received from GSM, Motor turned OFF !");
        digitalWrite(motor, LOW);
        digitalWrite(motoroffled, HIGH);
        digitalWrite(motoronled, LOW);
        a = 1;
        motorState = "OFF"; 
        textMessage = "";
        String message = "Motor is set to turned OFF. Earlier it was " + motorState +".  "+ supplystate + ". "+ "R= "+AcVoltROut + "V Y= "+AcVoltYOut+"V B= "+AcVoltBOut+"V." ;
        sendSMS(message);
        textMessage = "";
      }
      int AcVoltR = analogRead(VoltR);
      int AcVoltY = analogRead(VoltY);
      int AcVoltB = analogRead(VoltB);
      int AcVoltROut = (AcVoltR * (5.0 / 1023))*212.25;
      int AcVoltYOut = (AcVoltY * (5.0 / 1023))*53.68;
      int AcVoltBOut = (AcVoltB * (5.0 / 1023))*203.41;
      if(AcVoltROut>125){digitalWrite(linerled, HIGH);}else{digitalWrite(linerled, LOW);}
      if(AcVoltYOut>125){digitalWrite(lineyled, HIGH);}else{digitalWrite(lineyled, LOW);}
      if(AcVoltBOut>125){digitalWrite(linebled, HIGH);}else{digitalWrite(linebled, LOW);}

      if(AcVoltROut<125||AcVoltYOut<125||AcVoltBOut<125){
        digitalWrite(motor, LOW);
        digitalWrite(motoroffled, HIGH);
        digitalWrite(motoronled, LOW);
        motorState = "OFF"; 
        textMessage = ""; 
        supplystate = "Supply is Faulty";
        Serial.println("Faulty supply detected !, Motor powered OFF, Once Healthy supply detected, Motor will be turned ON");
        Serial.println("Note: 10 Seconds delay initiated !");
        Serial.print("R= ");
        Serial.println(AcVoltROut);
        Serial.print("Y= ");
        Serial.println(AcVoltYOut);
        Serial.print("B= ");
        Serial.println(AcVoltBOut);
        Serial.println("==============================================");
        delay(10000);
      }
      if(AcVoltROut>125&&AcVoltYOut>125&&AcVoltBOut>125){supplystate = "Supply is OK";}else{supplystate = "Supply is Faulty";}

      if(SIM900A.available()>0){textMessage = SIM900A.readString();textMessage.toUpperCase();delay(10);}
      if(textMessage.indexOf("MOTORSTATE")>=0){String message = supplystate + ", Motor is currently " + motorState;sendSMS(message);textMessage = "";}
      if(textMessage.indexOf("SUPPLYSTATE")>=0){String message = supplystate;sendSMS(message);textMessage = "";}

      if(AcVoltROut>125&&AcVoltYOut>125&&AcVoltBOut>125){
        int AcVoltR = analogRead(VoltR);
        int AcVoltY = analogRead(VoltY);
        int AcVoltB = analogRead(VoltB);
      int AcVoltROut = (AcVoltR * (5.0 / 1023))*212.25;
      int AcVoltYOut = (AcVoltY * (5.0 / 1023))*53.68;
      int AcVoltBOut = (AcVoltB * (5.0 / 1023))*203.41;
        
        if(AcVoltROut>125){digitalWrite(linerled, HIGH);}else{digitalWrite(linerled, LOW);}
        if(AcVoltYOut>125){digitalWrite(lineyled, HIGH);}else{digitalWrite(lineyled, LOW);}
        if(AcVoltBOut>125){digitalWrite(linebled, HIGH);}else{digitalWrite(linebled, LOW);}
        
        if(AcVoltROut<125||AcVoltYOut<125||AcVoltBOut<125){
          digitalWrite(motor, LOW);
          digitalWrite(motoroffled, HIGH);
          digitalWrite(motoronled, LOW);
          motorState = "OFF"; 
          textMessage = ""; 
          supplystate = "Supply is Faulty";
        }
        if(AcVoltROut>125&&AcVoltYOut>125&&AcVoltBOut>125){supplystate = "Supply is OK";}else{supplystate = "Supply is Faulty";}
        if(SIM900A.available()>0){textMessage = SIM900A.readString();textMessage.toUpperCase();delay(10);}
        if(textMessage.indexOf("MOTORSTATE")>=0){String message = supplystate + ", Motor is currently " + motorState;sendSMS(message);textMessage = "";}
        if(textMessage.indexOf("SUPPLYSTATE")>=0){String message = supplystate;sendSMS(message);textMessage = "";}
        if(textMessage.indexOf("MOTOROFF")>=0){
          digitalWrite(motor, LOW);
          a = 1;
          digitalWrite(motoroffled, HIGH);
          digitalWrite(motoronled, LOW);
          motorState = "OFF"; 
          textMessage = ""; 
          String message = "Motor has been turned " + motorState;
          sendSMS(message);
          textMessage = "";
          a=1;
          Serial.println("Command received from GSM, Motor turned OFF !");
          Serial.println(a);
          break;

        }
        
        digitalWrite(motoroffled, LOW);
        digitalWrite(motoronled, HIGH);
        delay(1000);
        digitalWrite(motor, HIGH);
        motorState = "ON";
        supplystate = "Supply is OK ,"; 
        textMessage = ""; 
      }
    }
  }
  if(AcVoltROut<125||AcVoltYOut<125||AcVoltBOut<125){
    digitalWrite(motor, LOW);
    digitalWrite(motoroffled, HIGH);
    digitalWrite(motoronled, LOW);
    motorState = "OFF";
    textMessage = "";
    supplystate = "Supply is Faulty";
  }

  if(textMessage.indexOf("MOTOROFF")>=0){
    Serial.println("2 Breaked the loop");
    a=1;
    digitalWrite(motor, LOW);
    digitalWrite(motoroffled, HIGH);
    digitalWrite(motoronled, LOW);
    motorState = "OFF";
    textMessage = "";
    String message = "Motor has been turned OFF";
    sendSMS(message);
    textMessage = "";
    delMessage();
  }
  if(textMessage.indexOf("MOTORSTATE")>=0){String message = supplystate + ", Motor is currently " + motorState;sendSMS(message);textMessage = "";}
  if(textMessage.indexOf("SUPPLYSTATE")>=0){String message = supplystate;sendSMS(message);textMessage = "";}     
}

//Void Loop Ends Here  


// void sendSMS(String message){
//   // AT command to set SIM900 to SMS mode
//   SIM900A.print("AT+CMGF=1\r"); 
//   delay(100);

//   // REPLACE THE X's WITH THE RECIPIENT'S MOBILE NUMBER
//   // USE INTERNATIONAL FORMAT CODE FOR MOBILE NUMBERS
//    SIM900A.println("AT + CMGS = \"+919067538813\""); 
//   delay(100);
//   // Send the SMS
//   SIM900A.println(message); 
//   delay(100);

//   // End AT command with a ^Z, ASCII code 26
//   SIM900A.println((char)26); 
//   delay(100);
//   SIM900A.println();
//   delay(5000);  
// }


void sendSMS(String message){
  Serial.println ("Sending Message");
  SIM900A.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);
  Serial.println ("Set SMS Number");
  SIM900A.println("AT+CMGS=\"+919067538813\"\r"); //Mobile phone number to send message
  delay(1000);
  Serial.print ("Set SMS Content = ");
  Serial.println (message);
  SIM900A.println(message);// Messsage content
  delay(100);
  Serial.println ("Finish");
  SIM900A.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  Serial.println ("Message has been sent.");
}

void delMessage(){
  Serial.println ("Deleting all messages = sent + received");
  SIM900A.println("AT+CMGDA=6");    //Sets the GSM Module in Text Mode
  delay(1000);
  Serial.println ("Deleted all messages");
  SIM900A.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}

void blinkLED(){
  for(int i = 0 ; i < 20 ; i++){
    digitalWrite(linerled, HIGH);
    digitalWrite(lineyled, HIGH);
    digitalWrite(linebled, HIGH);
    digitalWrite(motor, LOW);
    delay(100);
    digitalWrite(linerled, LOW);
    digitalWrite(lineyled, LOW);
    digitalWrite(linebled, LOW);
    digitalWrite(motor, LOW);
    delay(100);
  }
}

void blinkLED1(){
  digitalWrite(linerled, HIGH);
  digitalWrite(lineyled, HIGH);
  digitalWrite(linebled, HIGH);
  digitalWrite(motor, LOW);
  delay(100);
  digitalWrite(linerled, LOW);
  digitalWrite(lineyled, LOW);
  digitalWrite(linebled, LOW);
  digitalWrite(motor, LOW);
  delay(100);
  }