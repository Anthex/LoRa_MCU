/* 
 *  LoRa_envoi_simple.ino : Lecture d'une distance par module ultrason, affichage sur 7-segements multiplexés
 *  et envoi vers gateway LoRa.
 *  
 *  @author Achille Dantz <achille@dantz.fr>
 *  Code disponible sur https://github.com/Anthex
 *  
 *  Requiert la bibliothèque LoRaRadio.h disponible sur
 *  https://github.com/GrumpyOldPizza/ArduinoCore-stm32l0
 *  
 *  Requiert la bibliothèque LedControl.h disponible sur
 *  https://github.com/wayoda/LedControl
 *  
 *  JSON de la board (B-L072Z-LRWAN1)
 *  https://grumpyoldpizza.github.io/ArduinoCore-stm32l0/package_stm32l0_boards_index.json
 */
 
#include "LoRaRadio.h"
#include "LedControl.h"

#define STATE_NONE        0
#define STATE_SCANNING    1
#define STATE_TX_MASTER   2
#define STATE_RX_MASTER   3
#define STATE_TX_SLAVE    4
#define STATE_RX_SLAVE    5

LedControl lc=LedControl(12,11,10,1);   // MAX7219EWG

unsigned long delaytime=1000;           //Délai entre 2 mesures/envois
unsigned long len;                      //Longueur mesurée par le module ultrason
const int pingPin = 7;                  //Pin data module Ping)))

/**
 * printNumber(int v)
 * Affiche un entier sur un array de 7-segments
 * @param int v - nombre à afficher
 */
void printNumber(int v) {  
    int ones,tens,hundreds;  
    ones=v%10;  
    v=v/10;  
    tens=v%10;  
    v=v/10; hundreds=v;  
    lc.setDigit(0,2,(byte)hundreds,false);
    lc.setDigit(0,1,(byte)tens,false); 
    lc.setDigit(0,0,(byte)ones,false); 
} 

void setup( void )
{
    Serial.begin(9600);

    // Initialisation 7-segments
    lc.shutdown(0,false);
    lc.setIntensity(0,8);
    lc.clearDisplay(0);

    // Configuration module LoRa
    LoRaRadio.begin(915000000);
    LoRaRadio.setFrequency(915000000);
    LoRaRadio.setTxPower(14);
    LoRaRadio.setBandwidth(LoRaRadio.BW_125);
    LoRaRadio.setSpreadingFactor(LoRaRadio.SF_7);
    LoRaRadio.setCodingRate(LoRaRadio.CR_4_5);
    LoRaRadio.setLnaBoost(true);
}

void loop( void )
{
    //  mesure de la distance
    pinMode(pingPin, OUTPUT);
    digitalWrite(pingPin, LOW);
    delayMicroseconds(2);
    digitalWrite(pingPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(pingPin, LOW);
    pinMode(pingPin, INPUT);
    len = pulseIn(pingPin, HIGH) / 58; // délai -> longueur

    //  temporaire : limite à un octet
    if (len > 254){
      len = 255;
    }
    
    Serial.println(len);
    printNumber(len);

    //envoi du paquet LoRa (bourrage+valeur)
    LoRaRadio.beginPacket();
    LoRaRadio.write('V');
    LoRaRadio.write('A');
    LoRaRadio.write('L');
    LoRaRadio.write(len);
    LoRaRadio.endPacket();
    
    delay(delaytime);
}
