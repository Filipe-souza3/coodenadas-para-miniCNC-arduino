#include <Stepper.h>
#define steps 40

String var;
int PinUpMotorSpeed3=3;
int PinDownMotorSpeed8=8;
int PinPen=2;
bool ActPen=false;
char letra;

Stepper motorUp(steps,7,6,5,4);
Stepper motorDown(steps,12,11,10,9);

int AcharVirgula(){
  for(int i=0;i<var.length();i++){
      if(var[i]==','){return i;}
    }  
  return 0;
}

void PegarSerial(){
var="";
letra="";
while (Serial.available() > 0) {
    letra=Serial.read();
    if(letra!='\0'){var.concat(letra);}
    delay(2);
  }
}

void setup() { 
  //motorUp
  pinMode(PinUpMotorSpeed3,OUTPUT);
  digitalWrite(PinUpMotorSpeed3,HIGH);
  motorUp.setSpeed(180);
  //motorDown
  pinMode(PinDownMotorSpeed8,OUTPUT);
  digitalWrite(PinDownMotorSpeed8,HIGH);
  motorDown.setSpeed(180);
  //Pen
  pinMode(PinPen,OUTPUT);
  Serial.begin(9600);
}

void loop() {

  delay(50);
  int intx,inty;
  if(Serial.available() > 0){
    PegarSerial();
    int localVirg = AcharVirgula();
    
    if(var[0]!='+'){
      intx = var.substring(0,localVirg).toInt();
      inty = var.substring(localVirg+1,var.length()-1).toInt();
      if(intx!=321){
        motorUp.step(intx);
        motorDown.step(inty);

        if(ActPen==true){digitalWrite(PinPen,LOW); ActPen=false;}
      }else{
        digitalWrite(PinPen,HIGH);
        ActPen=true;
      }
      
      Serial.println();
      Serial.print(intx);
      Serial.print(" ");
      Serial.print(inty);
    }else{
      
      intx = var.substring(1,localVirg).toInt()*(-1);
      inty = var.substring(localVirg+1,var.length()-1).toInt()*(-1);

      digitalWrite(PinPen,HIGH);
      motorUp.step(intx);
      motorDown.step(inty);
      digitalWrite(PinPen,LOW);
      
      Serial.println();
      Serial.print(intx);
      Serial.print(" -- ");
      Serial.print(inty);
    }
  }
}
