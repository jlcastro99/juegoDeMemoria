#include <LiquidCrystal.h>
#define   Rojo         2
#define   Verde        3
#define   Amarillo     4
#define   Azul         5
#define   Campana      7
#define   In_Rojo      A0
#define   In_Verde     A1
#define   In_Amarillo  A2
#define   In_Azul      A3
int i;
int nivelActual = 1;
int velocidad = 500;
const int MaximiNivel = 50;
int secuencia[MaximiNivel];
int SecuenciaLeida[MaximiNivel];
LiquidCrystal lcd(7,6,5,4,3,2);                           //lcd

void setup(){
   pinMode(Amarillo, OUTPUT);
   pinMode(Azul, OUTPUT);
   pinMode(Rojo, OUTPUT);
   pinMode(Verde, OUTPUT);
   inicio();
}

void loop(){
   if(nivelActual == 1){
      generaSecuencia();
      muestraSecuencia();
      leeSecuencia();
   }
   if(nivelActual != 1){
      muestraSecuencia();
      leeSecuencia();
   }
}
// ------------------------  ------------------------
 void inicio(){
   mensajeDeInicio();
   for (i = 1; i <= 6; i++) {
   digitalWrite(Verde , HIGH);
   delay(100);
   digitalWrite(Verde , LOW);
   digitalWrite(Amarillo , HIGH);
   delay(100);
   digitalWrite(Amarillo, LOW);
   digitalWrite(Azul , HIGH);
   delay(100);
   digitalWrite(Azul, LOW);
   digitalWrite(Rojo , HIGH);
   delay(100);
   digitalWrite(Rojo , LOW);
 } 
}

void Marcador(){
   int unidad = (nivelActual / 4);      
   imp_unidad(unidad);
   int residuo = (nivelActual % 4);
   imp_residuo(residuo);
   delay(500);
}

void imp_unidad(int n){
  for(int x=1;x<=n;x++){
   digitalWrite(Verde ,   1);  
   digitalWrite(Amarillo, 1);
   digitalWrite(Azul,     1);
   digitalWrite(Rojo ,    1);
   delay(800);
   apagados();
   delay(300);  
  }
}

void imp_residuo(int n){
  apagados();
  if (n==1){
   digitalWrite(Verde ,   1);  
  }
  if (n==2){
   digitalWrite(Verde ,   1); 
   digitalWrite(Amarillo, 1); 
  }
  if (n==3){
   digitalWrite(Verde ,   1);
   digitalWrite(Amarillo, 1);
   digitalWrite(Azul,     1);  
  } 
   delay(800);
   apagados();
   delay(300);  
}

void apagados(){
   digitalWrite(Verde ,   LOW);  
   digitalWrite(Amarillo, LOW);
   digitalWrite(Azul,     LOW);
   digitalWrite(Rojo ,    LOW);
   delay(300);
}

// -------------------   ---------------   -------

void muestraSecuencia(){
   apagados();
   for(int i = 0; i < nivelActual; i++){
      if( secuencia[i] == Amarillo ){
         tone(Campana, 200);
         delay(200);
         noTone(Campana);
      }
      if( secuencia[i] == Azul ){
         tone(Campana, 300);
         delay(200);
         noTone(Campana);
      }
      if( secuencia[i] == Rojo ){
         tone(Campana, 400);
         delay(200);
         noTone(Campana);
      }
      if( secuencia[i] == Verde ){
         tone(Campana, 500);
         delay(200);
         noTone(Campana);
      }
      digitalWrite(secuencia[i], HIGH);
      delay(velocidad);
      digitalWrite(secuencia[i], LOW);
      delay(200);
   }
}

void leeSecuencia(){
   int flag = 0;
   for(int i = 0; i < nivelActual; i++){
      flag = 0;
      while(flag == 0){
         if(digitalRead(In_Verde) == LOW){
            digitalWrite(Verde, HIGH);
            tone(Campana, 500);
            delay(300);
            noTone(Campana);
            SecuenciaLeida[i] = Verde;
            flag = 1;
            delay(200);
            if(SecuenciaLeida[i] != secuencia[i]){
               secuenciaError();
               return;
            }
            digitalWrite(Verde, LOW);
         }
         if(digitalRead(In_Rojo) == LOW){
            digitalWrite(Rojo, HIGH);
            tone(Campana, 400);
            delay(300);
            noTone(Campana);
            SecuenciaLeida[i] = Rojo;
            flag = 1;
            delay(200);
            if(SecuenciaLeida[i] != secuencia[i]){
               secuenciaError();
               return;
            }
            digitalWrite(Rojo, LOW);
         }
         if(digitalRead(In_Azul) == LOW){
            digitalWrite(Azul, HIGH);
            tone(Campana, 300);
            delay(300);
            noTone(Campana);
            SecuenciaLeida[i] = Azul;
            flag = 1;
            delay(200);
            if(SecuenciaLeida[i] != secuencia[i]){
               secuenciaError();
               return;
            }
            digitalWrite(Azul, LOW);
         }
         if(digitalRead(In_Amarillo) == LOW){
            digitalWrite(Amarillo, HIGH);
            tone(Campana, 200);
            delay(300);
            noTone(Campana);
            SecuenciaLeida[i] = Amarillo;
            flag = 1;
            delay(200);
            if(SecuenciaLeida[i] != secuencia[i]){
               secuenciaError();
               return;
            }
            digitalWrite(Amarillo, LOW);
         }
      }
   }
   secuenciaCorrecta();
}

void generaSecuencia(){
   randomSeed(analogRead(5));   // randomSeed(millis());      //randomSeed(analogRead(5));
   for(int i = 0; i < MaximiNivel; i++){
      secuencia[i] = random(2,6);
   }
}

void TonoError(){
   int Tono[ ] = {260, 200, 200, 220, 200, 0, 250, 260};
   int duracionNotas[] = {4, 8, 8, 4, 4, 4, 4, 4};
   for(int i = 0; i < 8; i++){
      int duracionNota = 1000/duracionNotas[i];
      tone(Campana, Tono[i],duracionNotas);
      int pausaEntreNotas = duracionNota * 1.30;
      delay(pausaEntreNotas);
      noTone(Campana);
   }
}

void secuenciaError(){
   digitalWrite(Amarillo, HIGH);
   digitalWrite(Azul, HIGH);
   digitalWrite(Rojo, HIGH);
   digitalWrite(Verde, HIGH);
   delay(400);
   digitalWrite(Amarillo, LOW);
   digitalWrite(Azul, LOW);
   digitalWrite(Rojo, LOW);
   digitalWrite(Verde, LOW);
   delay(250);
   TonoError();
   mensajeError();
   menajeDeNivel();
   velocidad = 500;
   Marcador();
   nivelActual = 1;
   inicio();
}
void mensajeError(){
  lcd.begin(16,2);  
  lcd.setCursor(0,2);
  lcd.print("!!! you lost !!!");
  lcd.display();
  delay(2000);
  lcd.noDisplay();
  delay(1000);
}
void mensajeExitoso(){
  lcd.begin(16,2); 
  lcd.setCursor(2,0);             //COLUMNAS, FILAS//
  lcd.print("! VERY GOOD !");
  lcd.display();
  delay(2000);
  lcd.noDisplay();
  delay(2000);
}

void mensajeDeInicio(){
  lcd.begin(16,2);
//----------------------//
  lcd.setCursor(1,0);
  lcd.print("!!! welcome !!!");
  lcd.display();
  delay(10000);
  lcd.noDisplay();
  delay(2000);
}
void menajeDeNivel(){
  lcd.begin(16,2);
//----------------------//
  lcd.setCursor(1,0);
  lcd.print("NIVEL ALCANZADO: ");
  lcd.setCursor(7,1);
  lcd.print(nivelActual);
  lcd.display();
  delay(2000);
  lcd.noDisplay();
  delay(2000);
}
void secuenciaCorrecta(){
   if(nivelActual < MaximiNivel);
      nivelActual++;
      mensajeExitoso();
      velocidad -= 30;  // desmacar para aumentar velocidad del juego en cada nivel
   delay(350);
}
