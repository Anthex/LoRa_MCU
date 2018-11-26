/* 
 *  gateway_reception_forwarding.ino : Lit et déchiffre les paquets LoRa reçus et
 *  envoie les données formattées vers un serveur web via script python
 *  
 *  @author Achille Dantz <achille@dantz.fr>
 *  Code disponible sur https://github.com/Anthex
 *  
 *  Requiert la biliothèque LoRa disponible sur 
 *  https://github.com/sandeepmistry/arduino-LoRa
 *  
 *  JSON de la gateway : 
 *  http://www.dragino.com/downloads/downloads/YunShield/package_dragino_yun_test_index.json
*/

#include <SPI.h>
#include <LoRa.h>
#include <Console.h>
#include <Process.h>
#include <Bridge.h>

//  contenu du paquet LoRa
byte message[256];

//  paramètres radio LoRa
uint32_t freq = 915000000;
int SF = 7,Denominator = 5;
long SBW = 125E3;

//  timer, délai d'actualisation
long old_time=millis();
long new_time;
long status_update_interval=5000;

/*
 * void receivepacker(void)
 * Lit le buffer LoRa et stocke le dernier message dans l'array message[]
 */
void receivepacket() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    Console.print("Paquet reçu : ");
    int i = 0;
    while (LoRa.available() && i < 256) {
      message[i]=LoRa.read();
      Console.print(message[i],DEC);
      i++;
    }  
    delay(1000);
  Console.print(" - Valeur a envoyer : ");
  //  temporaire : la valeur que l'on envoie est en position 4, après 3 octets de bourrage
  Console.println(message[3], DEC);
  send_data();
  }
}

/*
 * void send_data(void) (A MODIFIER)
 * envoie deux paramètres vers un serveur web à travers un script python
 */
void send_data() {
  digitalWrite(A2, HIGH); // LED de la gateway

  //  Création du processus <python sendrest.py param1 param2>
  Process sendREST;
  sendREST.begin("python");
  sendREST.addParameter("/root/rest.py");  //
  sendREST.addParameter(String((int)message[3]));  //param1
  sendREST.addParameter(String(0));  //param2
  sendREST.run();

  // On forwarde le stdout du script
  while (sendREST.available() > 0) {
   char c = sendREST.read();
   Console.print(c);
 }
  Console.println("envoi termine.");
  digitalWrite(A2, LOW);
}

void show_config()
{
  Console.print("The frequency is ");Console.print(freq);Console.println("Hz");
  Console.print("The spreading factor is ");Console.println(SF);
  Console.print("The coderate is ");Console.println(Denominator);
  Console.print("The single bandwith is ");Console.print(SBW);Console.println("Hz");
}
void setup() {
  pinMode(A2, OUTPUT);
  Bridge.begin(115200);
  Console.begin();
  Console.println("Démarrage du récepteur LoRa");
  if (!LoRa.begin(freq)) {
    Console.println("Erreur de configuration");
    while (1);
  }

  //  applications des paramètres radio
  LoRa.setSpreadingFactor(SF);
  LoRa.setSignalBandwidth(SBW);
  LoRa.setCodingRate4(Denominator);
  LoRa.setSyncWord(0x34); 
  LoRa.receive(0);    

  //  Verification de la configuration radio
  show_config();  
}

void loop() {
  receivepacket();
  new_time = millis();

  if(message[3]!=0){
    send_data();
    message[3] = 0;
  }
  if( (new_time-old_time) > status_update_interval){
      old_time = new_time;
  }
}
