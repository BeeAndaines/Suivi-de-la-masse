// memo quand on voit les ecriture du capteur de gauche a droite A0-GND-5V-V1??-V2??-Vext?? 


// définition des constantes entières
int tension1 ;
int pression ; 
// définition des constantes décimales
float seuil = 1.8;
float masse ; 

// définition des parametre pour compter 5minutes


/**
 * Tiny RTC module
 * module horloge
 *
 * DS1307 pour bus I2C
 * avec batterie au lithium CR1225
 *
 * Le port I2C de l'Arduino est situé
 * sur les pin A4 et A5
 *
 * Analog pin A5 <-> SCL
 * Analog pin A4 <-> SDA
 */

#include <Wire.h>
/**
 * Nous utilisons la librairie RTClib
 * cf: https://github.com/adafruit/RTClib/archive/master.zip
 * à installer dans le répertoire libraries
 * du répertoire de sketchs
 */
#include "RTClib.h"
RTC_DS1307 RTC;

// pour ajuster l'heure changer en : true
bool definirHeure = true;
/**
 * Lecture de température avec un seul
 * capteur DS18B20 (le premier)
 */

/** 
 *  Nous utilisons la librairie OneWire
 *  Elle doit être présente dans le répertoire libraries
 *  situé dans le répertoire des croquis/sketchs
 *  voir dans le menu Préférences
 *  cf: https://github.com/PaulStoffregen/OneWire/archive/master.zip
 */
#include <OneWire.h>

/**
 * Écriture sur une carte SD
 * 
 * SD card reliée au bus SPI :
 * MOSI       - pin 11 
 * MISO       - pin 12
 * CLK ou SCK - pin 13
 * CS         - pin 4
 *
 * SPI pour Serial Peripheral Interface
 * 
 * created  24 Nov 2010
 * modified 9 Apr 2012
 * by Tom Igoe
 * cf: https://www.arduino.cc/en/Tutorial/Datalogger
 */


#include <SPI.h>
#include <SD.h>
#define FIVEMIN (1000UL * 60 * 5);
// Arduino Uno pin 4
// cf: https://www.arduino.cc/en/Reference/SPI
const int chipSelect = 4;

unsigned long rolltime = millis() + FIVEMIN;




// boucle de démarrage
void setup() 
{
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) 
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.println("Initialisation de la carte SD ...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) 
  {
    Serial.println("Carte Sd inaccesible ou absente");
    // don't do anything more:
    return;
  }
  Serial.println("Carte OK");
  Serial.println("Demo de la librairie Dallas Temperature");

 
   Wire.begin();
    RTC.begin();
  if (! RTC.isrunning()) 
  {
    Serial.println("RTC ne fonctionne pas  je ne comprends pas cettte ligne !");
  }

  if(definirHeure)
  {
      // la ligne suivante permet d'ajuster la date & l'heure
      // à la date de compilation du sketc
      RTC.adjust(DateTime(__DATE__, __TIME__));
  }
}




// lancement de la boucle qui se repetera 9600 fois par seconde
void loop() 
{
  if((long)(millis() - rolltime) >= 0) 
   {
    DateTime now = RTC.now();
    
//lecture de la valeur en numérique entre 0 et 1025 de la tension reçu par A0
  tension1= analogRead (A0) ;
  // calcul de la masse
  masse= tension1*0.1 ; 
/* 
    Serial.print(now.year(), DEC);
    Serial.print(';');
    Serial.print(now.month(), DEC);
    Serial.print(';');
    Serial.print(now.day(), DEC);
    Serial.print(';');
    Serial.print(now.hour(), DEC);
    Serial.print(';');
    Serial.print(now.minute(), DEC);
    Serial.print(';');
    Serial.print(now.second(), DEC);
    Serial.print(';'); 
    Serial.print("tension=");
    
    Serial.print(tension1);
    Serial.print("---------");
    Serial.print("masse=");
    Serial.print(masse);
    Serial.println(';');
   
    delay(5000);
    */
  //Zone SD
      // nous créons une chaîne de caractères pour
      // concaténer les données à écrire :
    String dataString = "";

    // nous convertissons la valeur 
    // avec l'objet String() afin de pouvoir 
    // l'écrire sur la carte
    dataString += String(now.year(), DEC);
    dataString += ";";
    dataString += String(now.month(), DEC);
    dataString += ";";
    dataString += String(now.day(), DEC);
    dataString += ";";
    dataString += String(now.hour(), DEC);
    dataString += ";";
    dataString += String(now.minute(), DEC);
    dataString += ";";
    dataString += String(now.second(), DEC);
    dataString += ";";
    dataString += String("_la_masse_est_de_ ");
    dataString += ";";    
    dataString += String(masse);
    dataString +=  ";";
      
  /** lecture de trois capteurs reliés de A0 à A2:
  for (int analogPin = 0; analogPin < 3; analogPin++) 
  {

    int sensor = analogRead(analogPin);
    dataString += String(sensor);
    if (analogPin < 2) 
    {
      // Nous ajoutons un caractère de séparation
      dataString += ";";
    }
    
  }
*/

  /**
   * nous ouvrons le fichier
   * Nb: un seul fichier peut être ouvert à la fois
   * le fichier se nomme : journal.csv
   */
  File dataFile = SD.open("journal.csv", FILE_WRITE);

  // si le fichier est disponnible, nous écrivons dedans :
      if (dataFile) 
        {
          dataFile.println(dataString);
          dataFile.close();

          // nous affichons aussi notre chaîne sur le port série :
          Serial.println(dataString);
        }
    // if the file isn't open, pop up an error:
    // Si le fichier n'est pas ouvert nous affichons
    // un message d'erreur
     else {
            Serial.println("nous ne pouvons ouvrir journal.csv");

  

      }

  
  // calcul de la masse la realtion est issue de la courbe d'étalonnage
 // float masse = tension1*0.1086-5.319 ; 
 // Serial.print("la masse a pour valeur");
 // Serial.print(masse);
//  Serial.println("kg");
rolltime += FIVEMIN;
   }
 

                                                  
}
 
