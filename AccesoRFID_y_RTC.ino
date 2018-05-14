#include <SPI.h>
#include <MFRC522.h>
#include <DS3231.h>
#include <Wire.h>
DS3231 Clock;
#define RST_PIN  9    //Pin 9 para el reset del RC522
#define SS_PIN  2   //Pin 2 para el SS (SDA) del RC522
MFRC522 mfrc522(SS_PIN, RST_PIN); ///Creamos el objeto para el RC522
byte year, month, date, DoW, hour, minute, second;

void setup() {

  Serial.begin(9600); //Iniciamos La comunicacion serial
  SPI.begin();        //Iniciamos el Bus SPI
  mfrc522.PCD_Init(); // Iniciamos el MFRC522
  Serial.println("Control de acceso:");
  pinMode(4,OUTPUT);
}

byte ActualUID[4]; //almacenará el código del Tag leído
byte Usuario1[4]= {0x77, 0x8E, 0x87, 0xAB} ; //código del usuario 1
byte Usuario2[4]= {0xA6, 0xFF, 0xCE, 0xA1} ; //código del usuario 2
void loop() { 

  // Revisamos si hay nuevas tarjetas  presentes
  if ( mfrc522.PICC_IsNewCardPresent()) 
        {  
      //Seleccionamos una tarjeta
            if ( mfrc522.PICC_ReadCardSerial()) 
            {
                hora();
                    
                  // Enviamos serialemente su UID
                  Serial.print(F("USUARIO:"));
                  for (byte i = 0; i < mfrc522.uid.size; i++) {
                          Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
                          Serial.print(mfrc522.uid.uidByte[i], HEX);   
                          ActualUID[i]=mfrc522.uid.uidByte[i];          
                  } 
                  Serial.print("     ");                 
                  //comparamos los UID para determinar si es uno de nuestros usuarios  
                  if(compareArray(ActualUID,Usuario1)){
                    
                  Serial.println("Acceso concedido. ");
                   digitalWrite(4,HIGH);
                   delay(2000);
                   digitalWrite(4,LOW);
                  }
                  else {
                  if(compareArray(ActualUID,Usuario2)){
                   Serial.println("Acceso concedido. ");
                   
                  }
                  else{
                    Serial.println("Acceso denegado. ");
                  }
                  }
                  // Terminamos la lectura de la tarjeta tarjeta  actual
                  mfrc522.PICC_HaltA();
          
            }
      
  }
  
}

//Función para comparar dos vectores
 boolean compareArray(byte array1[],byte array2[])
{
  if(array1[0] != array2[0])return(false);
  if(array1[1] != array2[1])return(false);
  if(array1[2] != array2[2])return(false);
  if(array1[3] != array2[3])return(false);
  return(true);
}

void hora() {
Wire.begin();
Clock.getTime(year, month, date, DoW, hour, minute, second);
                   Serial.print(date, DEC);
                    Serial.print("/");
                    Serial.print(month, DEC);
                    Serial.print("/");
                    Serial.print(year, DEC);
                    Serial.print("  ");
                    Serial.print(hour, DEC);
                    Serial.print(":");
                    Serial.print(minute, DEC);
                    Serial.print(":");
                    Serial.print(second, DEC);
                    Serial.print("  ");
                   
Wire.end();
  
}

