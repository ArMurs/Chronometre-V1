/*
 * Projet Chrono 1V1.
 *    
 * Matériel :
 *    - 1 Arduino Uno ;
 *    
 *    - 1 Module LCD 16 × 2 ;
 *    - 1 Potentiomètre (Contraste pour le LCD)
 *    - 1 Résistance [1000 Ohm] ;
 *    
 *    - 1 LED rouge ; 1 LED verte ;
 *    - 2 Résistances 3
 *    
 */

 
#include <LiquidCrystal.h>

// Sortie : LCD
#define RS 12
#define EN 11

#define D4 5
#define D5 4
#define D6 3
#define D7 2

// Sortie : LEDs
#define LED_VERTE 8
#define LED_ROUGE 9

// Entree : Bouton Marche/Arret :
#define BOUTON 13
bool boutonPrecedementEnfonce = false ;

/*
 * Etats : 
 * > false : Stop ;
 * > true : Marche.
 */
 
bool etat = false ;


// Temps  : Temps (En ms)
unsigned long tempsMarche = millis()  ;
unsigned long tempsStop = millis() ;

// LCD :
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7) ;




void setup() {
  // put your setup code here, to run once:

  // Initialisation de l'écran LCD :
  lcd.begin(16,2) ;

  // Initialisation des LEDs
  pinMode(LED_VERTE, OUTPUT) ;
  pinMode(LED_ROUGE, OUTPUT) ;

  // Initialisation de l'interrupteur :
  pinMode(BOUTON, INPUT_PULLUP) ;

  // Premier affichage des données :
  changement(etat) ;
 
}




void changement(bool etat) {
/*
 *  Cette fonction est appelée à chaque fois que le bouton est appuyé.
 *    - On change l'affichage du LCD ;
 *    - On change l'allumage des LEDs ;
 *    - On met à jour les durées.
 */
  lcd.setCursor(0,0) ;

  // Test : Le cronomètre marche (etat = true) ou est-il stoppé (etat = false)
  if(etat) {

    // Si il marche : 

    // Mise à jour de tempsStop
    tempsStop = millis() - tempsMarche ;

    // Affichage des états sur le LCD :
    lcd.print("Play>") ;
    lcd.setCursor(0,1) ;
    lcd.print("Stop ") ;

    // Allumage de la LED verte, extinction de la LED rouge
    digitalWrite(LED_VERTE, true) ;
    digitalWrite(LED_ROUGE, false) ;
    
  } else {
    // Sinon : 

    // Mise à jour de tempsMarche
    tempsMarche = millis() - tempsStop ;

    // Affichage des états sur le LCD :
    lcd.print("Play ") ;
    lcd.setCursor(0,1) ;
    lcd.print("Stop>") ;

    // Extinction de la LED verte, allumage de la LED rouge
    digitalWrite(LED_VERTE, false) ;
    digitalWrite(LED_ROUGE, true) ;
    
  }

}





void affichageHMSd(unsigned long nbDixiemesSecondes) {
  /*
   * Conversion et affichage des durées sur l'écran LCD.
   */
  
  // Conversion des durees en millisecondes vers une durée en Heure / Minutes / Secondes.
  unsigned long nbHeures = nbDixiemesSecondes / 36000 ;
  unsigned long nbMinutes = (nbDixiemesSecondes % 36000) / 600 ;
  unsigned long nbSecondes = (nbDixiemesSecondes % 600) / 10;

  // Écriture sur le LCD :
  lcd.print(nbHeures);
  lcd.print(":");
  lcd.print(nbMinutes/10);
  lcd.print(nbMinutes%10);
  lcd.print(":");
  lcd.print(nbSecondes/10);
  lcd.print(nbSecondes%10);
  lcd.print(".");
  lcd.print(nbDixiemesSecondes%10);
}




void chrono(unsigned long tempsMarche, unsigned long tempsStop, bool etat) {
  /*
   * Affichage des durées sur l'écran LCD
   */
  if(etat) {
    lcd.setCursor(7,0);
    affichageHMSd((millis()-tempsStop)/100) ;
  } else {
    lcd.setCursor(7,1);
    affichageHMSd((millis()-tempsMarche)/100) ;
  }
}




void loop() {
  // put your main code here, to run repeatedly:

  // Si le bouton est appuyé ...
  if(not(digitalRead(BOUTON))) {
    
     // ... Et qu'il n'a pas été appuyé l'instant d'avant :
    if (not (boutonPrecedementEnfonce)) {

      // Changement d'état Marche/Stop
      etat = not(etat) ;

      // Bouton appuyé, on empêche le controleur de re-changer d'état juste après.
      boutonPrecedementEnfonce = true ;

      // Affichage des durées
      changement(etat) ;
    }
  } else {

    // Le bouton est relâché
    boutonPrecedementEnfonce = false ;
    
  }

  // Affichage des durées
  chrono(tempsMarche, tempsStop, etat) ;

  // Délai : 50ms
  delay(50) ;

}
