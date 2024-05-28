#include <Servo.h> //Servo meter bibliotek
#include <Adafruit_NeoPixel.h> //Hente LED lys biblotekt

#define NEOPIXEL_PIN 5 //Pin til LED Stripe
#define NEOPIXEL_COUNT 6 //Antall LED brukt i stripen

//Bank
const int potPin = A3; //Analog pin il potensiometeret
const int addKnapp = 2; //Pin til sub og add knapper
const int subKnapp = 3;

Servo myservo; //Opprette de to servoene
Servo myservo2;

//Spinner
const int buttonPin = 6; //Pin tik knapp og motor
const int motorPin = 7;

int pengesum = 0; //pengesum som er i banken
int potnummer = 0; //Verdien til potensimeteret
int tilleggverdi = 0; //Verdien som skal legges til
int maxverdi = 1023; //Potensimeteret går opp til maks 1023

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800); //Definere LED stripen


void setup() {
  Serial.begin(9600);

  myservo.attach(4); //Setter servoene til pinene
  myservo2.attach(8);


  pinMode(buttonPin, INPUT_PULLUP); //Logikken for å skrup av og på knappene samt dc motoren

  pinMode(motorPin, OUTPUT);

  strip.begin();
  strip.show(); // slå av alle lys i begynnelsen

  pinMode(addKnapp, INPUT_PULLUP);
  pinMode(subKnapp, INPUT_PULLUP);
}

void loop() {


  //Metode for å skru på motoren som spinner
  if (digitalRead(buttonPin) == LOW) {
    snurrMotor();
  }
  else {
    // Slå av motoren
    digitalWrite(motorPin, LOW);
  }
 
  //Måte for å holde på verdiene til add og subknapp
  int addsjekk = digitalRead(addKnapp);
  int subsjekk = digitalRead(subKnapp);

  //Metode for å skru de forskjellige LED ene på, og sørge for at de andre er av
  if (pengesum >= 50 && pengesum < 200) {
    strip.setPixelColor(5, strip.Color(0, 255, 0)); // Grlnn
    strip.setPixelColor(1, strip.Color(0, 0, 0)); 
    strip.setPixelColor(2, strip.Color(0, 0, 0));
     strip.setPixelColor(3, strip.Color(0, 0, 0));
    strip.setPixelColor(4, strip.Color(0, 0, 0));
    strip.setPixelColor(1, strip.Color(0, 0, 0));
  } else if (pengesum >= 200 && pengesum < 300) {
    strip.setPixelColor(5, strip.Color(0, 255, 0));
    strip.setPixelColor(4, strip.Color(0, 255, 0)); 
    strip.setPixelColor(2, strip.Color(0, 0, 0));
     strip.setPixelColor(3, strip.Color(0, 0, 0));
    strip.setPixelColor(1, strip.Color(0, 0, 0));
    strip.setPixelColor(0, strip.Color(0, 0, 0));
  } 
  else if (pengesum >= 300 && pengesum < 400) {
    strip.setPixelColor(5, strip.Color(0, 255, 0));
    strip.setPixelColor(4, strip.Color(0, 255, 0));
    strip.setPixelColor(3, strip.Color(255, 255, 0)); //Gul
    strip.setPixelColor(2, strip.Color(0, 0, 0));
    strip.setPixelColor(1, strip.Color(0, 0, 0));
    strip.setPixelColor(0, strip.Color(0, 0, 0));

    }
    else if (pengesum >= 400 && pengesum < 500) {
      strip.setPixelColor(5, strip.Color(0, 255, 0));
    strip.setPixelColor(4, strip.Color(0, 255, 0));
    strip.setPixelColor(3, strip.Color(255, 255, 0)); 
    strip.setPixelColor(2, strip.Color(255, 255, 0));
    strip.setPixelColor(1, strip.Color(0, 0, 0));
    strip.setPixelColor(0, strip.Color(0, 0, 0));

    }
    else if (pengesum >= 500 && pengesum < 600) {
      strip.setPixelColor(5, strip.Color(0, 255, 0));
    strip.setPixelColor(4, strip.Color(0, 255, 0));
    strip.setPixelColor(3, strip.Color(255, 255, 0)); 
    strip.setPixelColor(2, strip.Color(255, 255, 0)); 
    strip.setPixelColor(1, strip.Color(255, 0, 0)); //Rød
    strip.setPixelColor(0, strip.Color(0, 0, 0));

    }
    else if (pengesum >= 600 && pengesum < 7000)  { //Lagt til maks 7000, men kan teknisk sett være evig
      strip.setPixelColor(5, strip.Color(0, 255, 0));
    strip.setPixelColor(4, strip.Color(0, 255, 0));
    strip.setPixelColor(3, strip.Color(255, 255, 0)); 
    strip.setPixelColor(2, strip.Color(255, 255, 0));
    strip.setPixelColor(1, strip.Color(255, 0, 0));
    strip.setPixelColor(0, strip.Color(255, 0, 0));

    }else {
      //Skrur alle av
    strip.setPixelColor(0, strip.Color(0, 0, 0)); 
    strip.setPixelColor(1, strip.Color(0, 0, 0)); 
    strip.setPixelColor(2, strip.Color(0, 0, 0));
     strip.setPixelColor(3, strip.Color(0, 0, 0));
    strip.setPixelColor(4, strip.Color(0, 0, 0));
    strip.setPixelColor(5, strip.Color(0, 0, 0));
  }

  strip.show(); // oppdater lysene

  

  potnummer = analogRead(potPin); //Hoode på verdien til potensimeteret
  tilleggverdi = kalkulerverdi(potnummer, maxverdi); //Metode for å gjøre en av verdiene på potensimeteret (0-1023) om til et annet tall
  

  if (addsjekk == LOW) { //Metode for å legge til penger. //Fysisk legger man det i hullet på prototypen. 
    leggTil();
  }

  if (subsjekk == LOW && tilleggverdi <= pengesum) { //metode for å fjerne pengene som er lagret, samt ta ut de fysiske pengene. Kan ikke ta ut mer enn det som er lagret.
    subtraher(); //Fjern verdien av penger som er lagret
    snurr(); // Metode for å fysisk åpen for stedet pengene er lagret i ved å snu på servomotorene.
  }
}

void leggTil() {
  pengesum += tilleggverdi; //Legger til tillegverdien i pengesum
  Serial.print("Addert: ");
  Serial.print(tilleggverdi);
  Serial.print(" Summen er nå: ");
  Serial.println(pengesum);
  delay(1000); //Delay for å sørge for at det ikke skjer flere knappetrykk samtidig

  
}

void subtraher() {
  pengesum -= tilleggverdi; //fjerner tillegveriden fra pengesum
  
  if (pengesum < 0) { //Summen kan ikke være mindre enn 0
    pengesum = 0;
  }

  Serial.print("Subtrahert: ");
  Serial.print(tilleggverdi);
  Serial.print(" Summen er nå: ");
  Serial.println(pengesum);

  while (digitalRead(subKnapp) == HIGH) {
    delay(10);
  }
}

void snurr() {
  //Metode for å snup på servometerne for å åpne lokket og ta ut penger, siden den ene Servometeren er plassert speilet må den snus speilet
  for (int pos = 0; pos <= 90; pos += 1) {
    myservo.write(pos);
    myservo2.write(90 - pos); //ande servo snus speilet
    delay(15);
  }
  //Bruker har ti sekunder på å ta ut penger
  delay(10000);
  for (int pos = 90; pos >= 0; pos -= 1) {
    myservo.write(pos);
    myservo2.write(90 - pos);
    delay(15);
  }

}

void snurrMotor(){
  Serial.println("Den spinner");
    // Slå på motoren
    digitalWrite(motorPin, HIGH);

}

//Metode for å gjøre om potensimeter verdien om til en av åtte verdier verdiene potensimeteret skal kunne snus til
int kalkulerverdi(int value, int maxValue) {
  int relativeValue = value * 9 / maxValue;
  int tilleggverdi[] = {1, 25, 50, 100, 150, 200, 250, 300, 350};

  // Sjekk for gyldig relativeValue
  if (relativeValue < 0 || relativeValue >= sizeof(tilleggverdi) / sizeof(tilleggverdi[0])) {
    return 0; // Ugyldig relativeValue
  }

  return tilleggverdi[relativeValue];
}