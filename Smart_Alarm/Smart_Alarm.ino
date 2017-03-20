/* 
Programa: Alarma basica primer ejemplo
Autor:	Osvaldo Renato Luzuriaga Luzuriaga

*/

//* Es para validar Password   
//# Es para Resetear el Password ingresado


#include <Password.h> //Incluimos la libreria Password
#include <Keypad.h> //Incluimos la libreria Keypad

Password password = Password( "1234" ); //Definimos nuestro Password

byte sensorpir = 10;
byte buzzer = 13;

boolean estado_alarma = false;
const byte ROWS = 4; // Cuatro Filas
const byte COLS = 3; // Cuatro Columnas

// Definimos el Keymap
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[ROWS] = { 12,11,6,5 };
byte colPins[COLS] = { 4,3,2};


// Creamos el Keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup(){

  Serial.begin(9600);
  keypad.addEventListener(keypadEvent);
  pinMode(sensorpir,INPUT);
  pinMode(buzzer, OUTPUT);
  
}



void loop(){
  keypad.getKey();
 
  if (digitalRead(sensorpir)== HIGH && estado_alarma== true){
    Serial.println("Intruso detectado");
    digitalWrite(buzzer, HIGH);  
    delay(9000);
  }
}




void keypadEvent(KeypadEvent eKey){
  switch (keypad.getState()){
    case PRESSED:
	Serial.print("Pressed: ");
	Serial.println(eKey);
	switch (eKey){
	  case '*': checkPassword(); break;
	  case '#': password.reset(); break;
	  default: password.append(eKey);
     }
  }
}



  
void checkPassword(){

  
  
  if (password.evaluate()){
    
        if(digitalRead(sensorpir) == LOW && estado_alarma == false){
              estado_alarma = true;
              digitalWrite(buzzer, HIGH ); 
              delay(500);
              Serial.println("Alarma Activada correctamente");
          
          }else if (estado_alarma == true){
               // apagamos alarma al estar el sensor pir activado
              estado_alarma = false;
              delay(500);
              digitalWrite(buzzer, LOW );
              Serial.println("Alarma desactivada  correctamente");
                
            }  
    
  }else{
    Serial.println("Codigo erroneo");
   //digitalWrite(buzzer, HIGH);  
    
  }

}




  
 

  
