//Vanessa Moser - Quellcode für den Slave

#include <Arduino.h>
#include <Wire.h>

// Pin-Belegung für LEDs
#define LED_Gruen PC12  // Grüne LED 
#define LED_Gelb PC11   // Gelbe LED 
#define LED_Rot PC10    // Rote LED 


#define SLAVE_ADDR 0x12 // Slave-Adresse für I2C-Kommunikation

#define ALLE_AUS 0b00
#define GRUEN_EIN 0b01
#define GRUEN_AUS 0b10
#define GELB_EIN 0b11
#define GELB_AUS 0b100
#define ROT_EIN 0b101
#define ROT_AUS 0b110
#define ALLE_AN 0b111

// Prototypen der Funktionen
void requestEvent();
void receiveEvent(int numBytes);

void setup() {
  Wire.begin(SLAVE_ADDR);         // Als I2C-Slave mit angegebener Adresse initialisieren
  Wire.onRequest(requestEvent);   // Handler-Funktion für Anfragen registrieren
  Wire.onReceive(receiveEvent);   // Handler-Funktion für empfangene Daten registrieren
  
  // Pin-Modus für LEDs konfigurieren
  pinMode(LED_Gruen, OUTPUT);    
  pinMode(LED_Gelb, OUTPUT);
  pinMode(LED_Rot, OUTPUT);

  // Serielle Kommunikation starten (für Debug-Zwecke)
  Serial.begin(115200);             
  Serial.println("Gruppennummer: 1 ");
  Serial.println("Mikrokontroller ist der SLAVE! Addresse 0x12");
 
}

void loop() 
{
  delay(100);  // Kurze Verzögerung in der Hauptschleife
}

// Funktion, die aufgerufen wird, wenn der Master Daten anfordert
void requestEvent() {
  // Antwortdaten, die an den Master gesendet werden (Beispiel: "Hello")
  char response[] = "H";
  Wire.write(response);  // Antwortdaten an den Master senden
}

// Funktion, die aufgerufen wird, wenn der Master Daten sendet
void receiveEvent(int numBytes) 
{
  Serial.println("Test!");
  while (Wire.available() > 0) 
  {
    char receivedDataMaster = Wire.read(); // Einzelnes Zeichen aus dem I2C-Puffer lesen
    Serial.print("Received character: ");
    Serial.println(receivedDataMaster);    // Empfangenes Zeichen über die serielle Schnittstelle ausgeben
   
    if(receivedDataMaster == ROT_EIN){ // Rot LED einschalten
        digitalWrite(LED_Rot, HIGH); 
        Serial.println("Rot ist an");
    }
    else if(receivedDataMaster == ROT_AUS){ // Rot LED ausschalten
       digitalWrite(LED_Rot, LOW);
       Serial.println("Rot ist aus");
    }
    else if(receivedDataMaster == GELB_EIN){ // Gelbe LED einschalten
        digitalWrite(LED_Gelb, HIGH); 
        Serial.println("gelb ist an");
    }
    else if(receivedDataMaster == GELB_AUS){ // Gelbe LED ausschalten
        digitalWrite(LED_Gelb, LOW);
        Serial.println("gelb ist aus");
    }
    else if(receivedDataMaster == GRUEN_EIN){
        digitalWrite(LED_Gruen, HIGH);  // Grüne LED einschalten
        Serial.println("gruen ist an");
    }

    else if(receivedDataMaster == GRUEN_AUS){
      digitalWrite(LED_Gruen, LOW);  // Grüne LED ausschalten
      Serial.println("gruen ist aus");
    }

  }
}