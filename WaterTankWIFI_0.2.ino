/*
Arrosage automatique à heure fixe
 */

#include <ESP8266WiFi.h>
                
// Valeurs pour le WiFi
const char* ssid     = "Router";
const char* password = "pass";

// Valeurs pour le serveur Web
const char* host     = "siteweb.fr";
String url = String("/time/time.php");
const int httpPort = 80; // Le serveur Web attend tradionnellement sur le port 80
int check = 0;

// Variables du JSON
String keyword = String("\"heure\":"); // Chaîne que l'on recherche dans le JSON
int heure = 0; // Variable du fichier JSON reçu

// Variables de temps
int temps_arrosage = (0)*60 + 35; //minutes + secondes
int heure_arrosage = 17;
float micro = 1000000;
float temps_DeepSleep = (2)*60 + 0; // minutes + secondes
float ajust = 1.0929;

// Broche pour relais
const int relay = D1;

void setup() {
  Serial.begin(115200);
  delay(10);

  // Demarrage de la connexion au WiFi
  Serial.print("Connexion au WiFi ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);    // On se connecte
  
  while (WiFi.status() != WL_CONNECTED) { // On attend la réponse de la box
    delay(500);
    Serial.print(".");
  }

  Serial.println("");  // on affiche les paramètres 
  Serial.println("WiFi connecté");  
  Serial.print("Adresse IP du module EPC: ");  
  Serial.println(WiFi.localIP());
  Serial.print("Adresse IP de la box : ");
  Serial.println(WiFi.gatewayIP());

  // Broche pour relais
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);
}

/* 
   Drapeau indiquant pendant l'analyse la réponse du serveur
   si on est dans l'en-tête HTTP (false) ou dans le contenu de 
   la ressource.
*/
bool inBody = false;

void loop() {
  Serial.print("Connexion au serveur : ");
  Serial.println(host);
  
  // On se place dans le rôle du  client en utilisant WifiClient
  WiFiClient client;

  // Si la connexion échoue ca sera pour la prochaine fois
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // La connexion a réussie on forme le chemin 
  Serial.print("Demande URL: ");
  Serial.println(url);
  
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n"); // On récupére les données

  delay(1000); // On attend 1 seconde
  
  inBody = false; // On est dans l'en-tête
  
  // On lit les données reçues, s'il y en a
  while(client.available()){
    String line = client.readStringUntil('\r');
        
    if (line.length() == 1) inBody = true; // passer l'en-tête jusqu'à une ligne vide
    if (inBody) {  // ligne du corps du message, on cherche le mot clé
       int pos = line.indexOf(keyword);

       if (pos > 0) { 
          // mot clé trouvé
          // indexOf donne la position du début du mot clé, en ajoutant sa longueur
          // on se place à la fin.
          pos += keyword.length(); 

          Serial.println (&line[pos]); // Affiche la ligne du JSON qui nous intéresse

          heure = atoi(&line[pos]); // Récupère la chaine de caractère et la convertie en nombre entier

     } // fin récupération de la variable entière
    } // fin de la recherche du mot clé 
  } // fin data avalaible 
  
  Serial.printf ("Il est : %d heures\n",heure); // On affiche le résultat

  //S il est l heure on arrose 
  if ( heure == heure_arrosage ) {
     Serial.printf ("Arrosage !\n\n");
     for (int tempo = 0; tempo < temps_arrosage; tempo++) {
         digitalWrite(relay, HIGH);
         delay(1000);
         Serial.printf("%d s\n",tempo+1);
     }
     digitalWrite(relay, LOW);
     //Arrosage pendant le temps nécessaire
     //Patienter le temps après arrosage pour passer l heure
     //Deep sleep de 2 minutes
     ESP.deepSleep(temps_DeepSleep*micro*ajust);
     delay(100);
     //delay(20000);
     //check = 0;
  }
  else {
     if ( check < 10 ) {
          Serial.printf("Check %d\n",check+1);
          check++;
     }
     else {
          Serial.printf ("Ce n\'est pas l\'heure d\'arroser !\n\n",heure);
          check = 0;
          //Deep sleep de 2 minutes
          ESP.deepSleep(temps_DeepSleep*micro*ajust);
          delay(100);
          //delay(20000);

     }
  }
  
  Serial.println("Connexion fermée");
  Serial.println("-------------------------------------------\n\n");

  delay(5000); // On attend 5 secondes avant de reconsulter
}
