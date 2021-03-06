#include <TM1637Display.h>

#define BUTTON_1  5   //ON/OFF
#define BUTTON_2  6   //Affichage
#define BUTTON_3  7   //Reglage
#define PHOTO_R1  A0  //Analog Photoresistance 1
#define PHOTO_R2  A1  //Analog Photoresistance 2
#define LED 4
#define CLK 2
#define DIO 3

TM1637Display display(CLK,DIO);

uint8_t tabSeg[] = { 0xff, 0xff, 0xff, 0xff };

int Photoresistance1 = 0;
int Photoresistance2 = 0;
int ActiveLED = 0;
int active = 0;
int state = 0;

int previous_bouton1 = 0;
int actual_bouton1 = 0;
int previous_bouton2 = 0;
int actual_bouton2 = 0;
int previous_bouton3 = 0;
int actual_bouton3 = 0;

int sequenceur = 0;
int sequenceur2 = 0;
int sequenceur3 = 0;

int unite = 0;
int dizaine = 0;
int centaine = 0;

String Valeur1 = " PR1 : "; //Chaine de caractère pour faire afficher sur la console
String Valeur2 = " PR2 : ";

void setup() // fonction setup - début de l'exécution du programme
{
  
  init_hardware();
  Serial.begin(9600);
}

void loop() 
{
  loadButtons(); // Fonction qui charge les valeurs des boutons pour la future séquence
  
  if((actual_bouton1 == 1) && (previous_bouton1 == 0)) 
  {
    sequenceur = !sequenceur;
  }
 
  if (sequenceur)// Etat 0 -> 1
  {
   state = whichState(); // nous permet de savoir vers quel état on doit se diriger

   Serial.println(state);

   switch(state)
   {
   case 1 : // Dans cet état on veut juste allumer la led
    if (activate())
      digitalWrite(LED, HIGH);
    break;
    case 2 : // On affiche la valeur des photoresistances sur le 7seg
      affichage_7seg();
    break;
    case 3 : // On affiche 8888 sur le 7seg
      uint8_t tabSeg[] = { 0xff, 0xff, 0xff, 0xff };
      display.setBrightness(0x0f);
      display.setSegments(tabSeg);
    break;
    }
    delay(1000);
  }
  else
  {
    state0(); // On éteint notre système et on réinitialise les variables
  }
  updateVar(); // On met à jour les variables de sequence
}
