#include <LiquidCrystal.h> // affichage LCD

LiquidCrystal lcd(12, 11, 6, 5, 4, 3); // broches LCD connectées à l'Arduino

int const triggHC = 10; // émetteur
int const echoPinHC = 9; // récepteur
int const buzzPin = 2; // contrôleur du buzzer
int const LEDTension = A0; // vérifie si le circuit est sous tension, allume LED verte
int const LEDBuzzer = A1; // allume LED rouge si obstacle
bool arret_portail = false;

// Variables globales
float derniere_distance = -2; // Valeur initiale invalide pour forcer une mise à jour

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);

  // configuration des broches entrees-sorties
  pinMode(triggHC, OUTPUT);
  pinMode(echoPinHC, INPUT);
  pinMode(buzzPin, OUTPUT);
  pinMode(LEDTension, OUTPUT);
  pinMode(LEDBuzzer, OUTPUT);

  // Allume la LEDTension au démarrage
  digitalWrite(LEDTension, HIGH);
}

void loop() {
  float duree;
  float distance;

  // Envoi d'une impulsion ultrasonique pour calculer la distance
  digitalWrite(triggHC, LOW);
  delayMicroseconds(2);
  digitalWrite(triggHC, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggHC, LOW);

  duree = pulseIn(echoPinHC, HIGH, 60000); // mesure la durée du retour via écho

  if (duree == 0) {
    distance = -1; // si pas de retour d'écho
  } else {
    distance = (duree / 2.0) / 29.1; // vitesse du son dans l'air (340 m/s)
  }

  // Mise à jour de l'écran LCD uniquement si la distance a changé
  if (distance != derniere_distance) {
    lcd.setCursor(0, 0);
    lcd.print("Capteur obstacle ");

    lcd.setCursor(0, 1);
    if (distance > 0 && distance <= 320) {
      lcd.print("Dist : ");
      lcd.print(distance, 1);
      lcd.print("cm   "); // Ajout d'espaces pour effacer les données précédentes
    } else {
      lcd.print("Hors portee !   ");
    }

    derniere_distance = distance; // Mettre à jour la distance précédente
  }
  
  // Contrôle des cas qui vont allumer la LED rouge si besoin
  if (distance > 0 && distance <= 320){
    digitalWrite(LEDBuzzer, HIGH);
  }else{
    digitalWrite(LEDBuzzer, LOW);
  }
  
  // Contrôle des cas qui vont déclencher le buzzer et allumer la LED rouge
  if (distance > 0 && distance <= 320) {
    digitalWrite(buzzPin, HIGH);
    //digitalWrite(LEDBuzzer, HIGH); // Allume une LED pour signaler l'obstacle
    arret_portail = true; // Si le buzzer sonne, il faut arrêter le mouvement du portail
  } else {
    digitalWrite(buzzPin, LOW);
    //digitalWrite(LEDBuzzer, LOW); // Éteint la LED sinon
    arret_portail = false; // On continue le mouvement du portail autrement
  }

  delay(200); // Délai réduit pour des mises à jour rapides
}
